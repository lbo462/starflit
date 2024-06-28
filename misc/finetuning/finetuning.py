
# This file is used to finetune the model MobileNetv3Large with a customed dataset.


import os
import copy
import torch
import torch.nn as nn
import torch.optim as optim
from torchvision import datasets, transforms, models
from torch.utils.data import DataLoader, random_split
from tqdm import tqdm

# Hyperparameters
batch_size = 32
learning_rate = 0.001
learning_rate_ft2 = 0.0001  # Learning rate pour la deuxième phase de fine-tuning
num_epochs = 1000  # Nombre maximal d'époques pour chaque phase
train_val_split = 0.8
patience = 5  # Patience pour l'early stopping

# Data Transforms
data_transforms = {
    'train': transforms.Compose([
        transforms.RandomResizedCrop(224),
        transforms.RandomHorizontalFlip(),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
    'val': transforms.Compose([
        transforms.Resize(256),
        transforms.CenterCrop(224),
        transforms.ToTensor(),
        transforms.Normalize([0.485, 0.456, 0.406], [0.229, 0.224, 0.225])
    ]),
}

def train_model(model, criterion, optimizer, scheduler, max_epochs=1000, patience=5):
    best_model_wts = copy.deepcopy(model.state_dict())
    best_acc = 0.0
    epochs_no_improve = 0  # Compteur pour le nombre d'époques sans amélioration
    early_stop = False

    for epoch in range(max_epochs):
        print('Epoch {}/{}'.format(epoch, max_epochs - 1))
        print('-' * 10)

        for phase in ['train', 'val']:
            if phase == 'train':
                model.train()
            else:
                model.eval()

            running_loss = 0.0
            running_corrects = 0

            # Add tqdm progress bar
            progress_bar = tqdm(dataloaders[phase], desc=f'{phase} Epoch {epoch+1}/{max_epochs}', leave=False)

            for inputs, labels in progress_bar:
                inputs = inputs.to(device)
                labels = labels.to(device)

                optimizer.zero_grad()

                with torch.set_grad_enabled(phase == 'train'):
                    outputs = model(inputs)
                    _, preds = torch.max(outputs, 1)
                    loss = criterion(outputs, labels)

                    if phase == 'train':
                        loss.backward()
                        optimizer.step()

                running_loss += loss.item() * inputs.size(0)
                running_corrects += torch.sum(preds == labels.data)

                # Update tqdm description
                progress_bar.set_postfix(loss=running_loss / dataset_sizes[phase], acc=running_corrects.double() / dataset_sizes[phase])

            if phase == 'train':
                scheduler.step()

            epoch_loss = running_loss / dataset_sizes[phase]
            epoch_acc = running_corrects.double() / dataset_sizes[phase]

            print('{} Loss: {:.4f} Acc: {:.4f}'.format(phase, epoch_loss, epoch_acc))

            if phase == 'val':
                if epoch_acc > best_acc:
                    best_acc = epoch_acc
                    best_model_wts = copy.deepcopy(model.state_dict())
                    epochs_no_improve = 0  # Réinitialiser le compteur
                else:
                    epochs_no_improve += 1

            if epochs_no_improve >= patience:
                print(f'Early stopping after {patience} epochs without improvement.')
                early_stop = True
                break

        print()

        if early_stop:
            break

    model.load_state_dict(best_model_wts)
    return model

if __name__ == '__main__':
    # Load Data
    data_dir = 'dataset'
    full_dataset = datasets.ImageFolder(root=data_dir, transform=data_transforms['train'])

    # Split dataset into training and validation sets
    train_size = int(train_val_split * len(full_dataset))
    val_size = len(full_dataset) - train_size
    train_dataset, val_dataset = random_split(full_dataset, [train_size, val_size])

    # Apply different transforms to train and validation datasets
    train_dataset.dataset.transform = data_transforms['train']
    val_dataset.dataset.transform = data_transforms['val']

    dataloaders = {
        'train': DataLoader(train_dataset, batch_size=batch_size, shuffle=True, num_workers=4),
        'val': DataLoader(val_dataset, batch_size=batch_size, shuffle=False, num_workers=4)
    }

    dataset_sizes = {'train': len(train_dataset), 'val': len(val_dataset)}
    class_names = full_dataset.classes

    # Load Pretrained MobileNetV3-Large
    model_ft = models.mobilenet_v3_large(pretrained=True)

    # Freeze all layers
    for param in model_ft.parameters():
        param.requires_grad = False

    # Replace the classifier (last layer) for fine-tuning
    num_ftrs = model_ft.classifier[3].in_features
    model_ft.classifier[3] = nn.Linear(num_ftrs, len(class_names))

    # Move model to GPU if available
    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    model_ft = model_ft.to(device)

    # Loss function and optimizer for the first phase of fine-tuning
    criterion = nn.CrossEntropyLoss()
    optimizer_ft = optim.SGD(model_ft.classifier.parameters(), lr=learning_rate, momentum=0.9)  # Optimizer only updates classifier parameters
    exp_lr_scheduler = optim.lr_scheduler.StepLR(optimizer_ft, step_size=7, gamma=0.1)

    # Training the model for the first phase
    model_ft = train_model(model_ft, criterion, optimizer_ft, exp_lr_scheduler, max_epochs=num_epochs, patience=patience)

    # Save the model after the first phase of fine-tuning
    torch.save(model_ft.state_dict(), 'mobilenetv3_large_finetuned_first_phase.pth')

    # Reset model and optimizer for the second phase of fine-tuning
    model_ft = models.mobilenet_v3_large(pretrained=True)  # Reload model with pretrained weights
    num_ftrs = model_ft.classifier[3].in_features
    model_ft.classifier[3] = nn.Linear(num_ftrs, len(class_names))
    model_ft = model_ft.to(device)

    optimizer_ft2 = optim.SGD(model_ft.parameters(), lr=learning_rate_ft2, momentum=0.9)

    # Training the model for the second phase of fine-tuning
    model_ft = train_model(model_ft, criterion, optimizer_ft2, exp_lr_scheduler, max_epochs=num_epochs, patience=patience)

    # Save the model after the second phase of fine-tuning
    torch.save(model_ft.state_dict(), 'mobilenetv3_large_finetuned_second_phase.pth')