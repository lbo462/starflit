import torch
from torchvision import models, transforms
from PIL import Image
import cv2
import numpy as np

# Charger le modèle
model_path = 'mobilenetv3_large_finetuned_second_phase.pth'
model = models.mobilenet_v3_large()
model.classifier[-1] = torch.nn.Linear(in_features=1280, out_features=2)
model.load_state_dict(torch.load(model_path, map_location=torch.device('cpu')))
model.eval()

# Transformer l'image pour l'inférence
transform = transforms.Compose([
   transforms.Resize(256),
   transforms.CenterCrop(224),
   transforms.ToTensor(),
   transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225]),
])

# Fonction pour capturer des images à partir de la caméra
def capture_camera():
   cap = cv2.VideoCapture(0)  # Utilisation de la première caméra disponible
   if not cap.isOpened():
       print("Erreur: Impossible d'ouvrir la caméra.")
       return None

   while True:
       ret, frame = cap.read()
       if not ret:
           print("Erreur: Problème lors de la capture de l'image.")
           break

       # Convertir l'image en format PIL et appliquer la transformation
       pil_image = Image.fromarray(cv2.cvtColor(frame, cv2.COLOR_BGR2RGB))
       image_tensor = transform(pil_image).unsqueeze(0)

       # Effectuer une prédiction
       with torch.no_grad():
           outputs = model(image_tensor)
           _, predicted = torch.max(outputs, 1)
           confidence = torch.softmax(outputs, 1)[0] * 100
           predicted_class = predicted.item()

       # Afficher les résultats de la prédiction dans la console
       class_names = ['healthy', 'diseased']  # Assurez-vous que l'ordre correspond à votre modèle
       print(f'Predicted class: {class_names[predicted_class]} with confidence: {confidence[predicted_class]:.2f}%')

   cap.release()

# Appel de la fonction pour capturer des images de la caméra
capture_camera()