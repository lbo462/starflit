#!/usr/bin/env python
# coding: utf-8

# # Starflit's transfer learning
# 
# Welcome to the Starflit's transfer learning notebook!
# 
# > Before diving into this, you're highly encouraged to take a look at the documentation about transfer learning on GitHub.
# 
# This notebook was highly inspired by the [documentation from Keras](https://keras.io/guides/transfer_learning/).
# 
# As said is the documentation, transfer learning are performed is two steps:
# - _feature extraction_, that will output a model that we'll call __tuned__;
# - _fine-tuning_, that will output a model that we'll call __fine-tuned__;

# ## Notebook configuration
# 
# This notebook might run:
# - Standalone in an IDE
# - Imported in Google Colab
# - In a pythonic version
# 
# In order for this notebook to work properly in all these configuration,
# please take a second or two to change the variables below:

# In[ ]:


running_on_colab = False     # Set to True iff you see this in Google Colab!

running_as_notebook = False  # Should be True if running in an IDE or on Google Colab

gui_available = False        # Running this as a notebook probably means that you're having a GUI.
                             # But if you're running this as a pythonic version under ssh, you should set this to False

previous_training_interrupted = False  # Sets to True iff there exist a checkpoint file from a previous interrupted run.


# > Note that the default parameters are set to run under a pythonic version, over SSH.

# ## Creating and importing a dataset
# 
# This part, was probably the most difficult one, unexpectedly.
# 
# As transfer learning is really nicely documented, the creation and the import of a dataset is not.
# We tried several approach, like using HuggingFace, but most of these approach failed.
# 
# You'll surely find a bit more details about this in the documentation about transfer learning on GitHub.
# 
# What we finally came up with was to import our dataset locally in a folder `dataset/` having the following structure:
# 
# ```
# dataset/
#     class1/
#         image1.JPG
#         image2.JPG
#         ...
#     class2/
#         image1.JPG
#         image2.JPG
#         ...
#     ...
# ```
# 
# Not that terrifying after all!

# ### Import the dataset
# 
# Now, one have to import this dataset.
# 
# __If you're trying to run this notebook on Google Colab__,
# you'll need to mount your personal drive and change the current directory.
# 
# > This implies that you've uploaded your dataset to your personal drive.
# 
# In the following codeblock, edit the path to match the path to your dataset.
# You can check the files structures by clicking files and searching for the folder named content at the root of the file system.
# Here, I placed my `dataset` folder in a folder named `Colab Notebooks` :

# In[ ]:


if running_on_colab:

    import os
    from google.colab import drive

    drive.mount('/content/drive')
    os.chdir('/content/drive/MyDrive/Colab Notebooks/dataset')


# ## Dependencies
# 
# If we're working in Google Colab, so we don't have to bother with virtual environment.
# 
# This also means that we don't have a clear list of dependencies such as `requirements.txt`.
# 
# Hence, we install the dependencies as such:

# In[ ]:


# uncomment this if running in Google Colab
# !pip install tensorflow


# Do not hesitate to edit the list of dependencies.
# 
# And just for the record, here are the dependencies we used we actually trained the model, installed in a virtual environment:
# 
# ```
# absl-py==2.1.0
# astunparse==1.6.3
# certifi==2024.6.2
# charset-normalizer==3.3.2
# flatbuffers==24.3.25
# gast==0.5.4
# google-pasta==0.2.0
# grpcio==1.64.1
# h5py==3.11.0
# idna==3.7
# keras==3.3.3
# libclang==18.1.1
# Markdown==3.6
# markdown-it-py==3.0.0
# MarkupSafe==2.1.5
# mdurl==0.1.2
# ml-dtypes==0.3.2
# namex==0.0.8
# numpy==1.26.4
# nvidia-cublas-cu12==12.3.4.1
# nvidia-cuda-cupti-cu12==12.3.101
# nvidia-cuda-nvcc-cu12==12.3.107
# nvidia-cuda-nvrtc-cu12==12.3.107
# nvidia-cuda-runtime-cu12==12.3.101
# nvidia-cudnn-cu12==8.9.7.29
# nvidia-cufft-cu12==11.0.12.1
# nvidia-curand-cu12==10.3.4.107
# nvidia-cusolver-cu12==11.5.4.101
# nvidia-cusparse-cu12==12.2.0.103
# nvidia-nccl-cu12==2.19.3
# nvidia-nvjitlink-cu12==12.3.101
# opt-einsum==3.3.0
# optree==0.11.0
# packaging==24.1
# pillow==10.3.0
# protobuf==4.25.3
# Pygments==2.18.0
# requests==2.32.3
# rich==13.7.1
# scipy==1.13.1
# six==1.16.0
# tensorboard==2.16.2
# tensorboard-data-server==0.7.2
# tensorflow==2.16.1
# tensorflow-io-gcs-filesystem==0.37.0
# termcolor==2.4.0
# typing_extensions==4.12.2
# urllib3==2.2.2
# Werkzeug==3.0.3
# wrapt==1.16.0
# ```

# And we import all we need here:

# In[ ]:


from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.applications import MobileNetV3Large
from tensorflow.keras.layers import Dense, GlobalAveragePooling2D
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import Adam
from tensorflow.keras.callbacks import EarlyStopping, ModelCheckpoint
from tensorflow import lite as tflite

if gui_available:
    import matplotlib.pyplot as plt


# ## Feature extraction
# 
# Feature extraction is the first step of transfer learning.
# 
# Here, we'll preprocess our images and split them into different splits.

# In[ ]:


# Preprocessing and data augmentation
train_datagen = ImageDataGenerator(
    rescale=1./255,
    rotation_range=40,
    width_shift_range=0.2,
    height_shift_range=0.2,
    shear_range=0.2,
    zoom_range=0.2,
    horizontal_flip=True,
    fill_mode='nearest',
    validation_split=0.5  # Keep only a part of the dataset, represented as a percent (between 0 and 1)
)

# Extract train dataset
train_generator = train_datagen.flow_from_directory(
    './dataset',
    target_size=(224, 224),
    batch_size=32,
    class_mode='binary',
    subset='training'
)

# Extract validation dataset
validation_generator = train_datagen.flow_from_directory(
    './dataset',
    target_size=(224, 224),
    batch_size=32,
    class_mode='binary',
    subset='validation'
)


# Now, we have two splits, data augmented.
# 
# Our next objective is to load the model we have to train and add some layers.

# In[ ]:


# Load the MobileNetV3Large model with the imagenet weights as initial weights
base_model = MobileNetV3Large(weights='imagenet', include_top=False, input_shape=(224, 224, 3))

# Now, we add our custom layers
x = base_model.output
x = GlobalAveragePooling2D()(x)
x = Dense(1024, activation='relu')(x)
predictions = Dense(1, activation='sigmoid')(x)
model = Model(inputs=base_model.input, outputs=predictions)


# > Note that `include_top=False` means that we're not including the classification layer of the base model.
# > That's exactly what allows us to have our customs classes as output.
# 
# As described in the Keras documentation, one have to freeze all the layers of the base model (but not the layers we just added).
# 
# When performing feature extraction, we only train the layers we added !

# In[ ]:


# Freeze the base model
for layer in base_model.layers:
    layer.trainable = False

# Compile the model to take the freeze into account
model.compile(optimizer=Adam(), loss='binary_crossentropy', metrics=['accuracy'])


# Now, we prepare an early stopping, so that the training will stop if no improvement is to occur between three consecutive epochs.
# 
# We also prepare a [model checkpoint](https://www.tensorflow.org/tutorials/keras/save_and_load#save_checkpoints_during_training) callback
# so that we will be able to continue our training even the execution stops randomly.

# In[ ]:


early_stopping = EarlyStopping(monitor="loss", patience=3)
model_checkpoint = ModelCheckpoint(
    filepath='checkpoints/cp.weights.keras',
    monitor='val_accuracy',
    mode='max',
    save_best_only=True,
    save_weights_only=True,
)


# IF the previous training was interrupted, you can load the previous weights with this:

# In[ ]:


if previous_training_interrupted:
    model.load_weights('checkpoints/cp.weights.keras')

    print("Model restored, evaluating ...")
    loss, accuracy = model.evaluate(validation_generator)

    print(f'Validation loss: {loss}')
    print(f'Validation accuracy: {accuracy}')


# And now is the time we actually train the model.
# 
# The following codeblock will probably last for long, and that's why we have trouble running this notebook where we want :'(

# In[ ]:


history = model.fit(
    train_generator,
    epochs=1000,
    validation_data=validation_generator,
    callbacks=[early_stopping, model_checkpoint]
)


# And now, we have our trained model, tuned to our dataset.
# Feature extraction is over, and we'll proceed to _fine-tuning_ is few seconds,
# just after a quick model save!

# In[ ]:


model.save('mobilenetv3_tuned.keras')


# ### Testing tuned model
# 
# Note that the model is actually tuned for our dataset.
# Hence, now is a good timing to have some testing.

# In[ ]:


loss, accuracy = model.evaluate(validation_generator)

print(f'Validation loss: {loss}')
print(f'Validation accuracy: {accuracy}')

if gui_available:
    acc = history.history['accuracy']
    val_acc = history.history['val_accuracy']
    loss = history.history['loss']
    val_loss = history.history['val_loss']

    epochs_range = range(len(acc))

    plt.figure(figsize=(8, 8))
    plt.subplot(1, 2, 1)
    plt.plot(epochs_range, acc, label='Training Accuracy')
    plt.plot(epochs_range, val_acc, label='Validation Accuracy')
    plt.legend(loc='lower right')
    plt.title('Training and Validation Accuracy')

    plt.subplot(1, 2, 2)
    plt.plot(epochs_range, loss, label='Training Loss')
    plt.plot(epochs_range, val_loss, label='Validation Loss')
    plt.legend(loc='upper right')
    plt.title('Training and Validation Loss')
    plt.show()


# ## Fine-tuning
# 
# This step is optional, but we actually want to do a round of fine-tuning on the whole model.
# 
# This is done via unfreezing every layer of the model and re-run a training with a small learning rate.
# 
# > It's important to keep a low learning rate!
# > The danger is overfitting the model here!

# In[ ]:


# Unfreeze the base model layers
for layer in base_model.layers:
    layer.trainable = True

# Re-compile the model, with a low learning rate (1e-5 here)
model.compile(optimizer=Adam(1e-5), loss='binary_crossentropy', metrics=['accuracy'])


# And now we re-train the model.
# This is also kind of long to run, same as previously.

# In[ ]:


history_fine = model.fit(
    train_generator,
    epochs=1000,
    validation_data=validation_generator,
    callbacks=[early_stopping, model_checkpoint]
)


# And let's re-save our model a second time, but fine-tuned this time.
# This is the model we'll be using in our beloved Strandbeest.

# In[ ]:


model.save('mobilenetv3_fine_tuned.keras')


# ### Testing fine-tuned model
# 
# Now, everything is set to have some little performance tests, using the validation dataset.

# In[ ]:


loss, accuracy = model.evaluate(validation_generator)

print(f'Validation loss: {loss}')
print(f'Validation accuracy: {accuracy}')

if gui_available:
    acc = history_fine.history['accuracy']
    val_acc = history_fine.history['val_accuracy']
    loss = history_fine.history['loss']
    val_loss = history_fine.history['val_loss']

    epochs_range = range(len(acc))

    plt.figure(figsize=(8, 8))
    plt.subplot(1, 2, 1)
    plt.plot(epochs_range, acc, label='Training Accuracy')
    plt.plot(epochs_range, val_acc, label='Validation Accuracy')
    plt.legend(loc='lower right')
    plt.title('Training and Validation Accuracy')

    plt.subplot(1, 2, 2)
    plt.plot(epochs_range, loss, label='Training Loss')
    plt.plot(epochs_range, val_loss, label='Validation Loss')
    plt.legend(loc='upper right')
    plt.title('Training and Validation Loss')
    plt.show()


# ## Convert the model to tflite
# 
# All the models we have exported are under the Keras models format : `.h5`.
# 
# But our strandbeest should be fed with `.tflite` files.
# Thus, we convert our model here:

# In[ ]:


# Convert the tuned model
converter = tflite.TFLiteConverter.from_keras_model_file('mobilenetv3_tuned.keras')
model = converter.convert()
with open('mobilenetv3_tuned.tflite' , 'wb') as f:
    f.write(model)


# In[ ]:


# Convert the fine-tuned model
converter = tflite.TFLiteConverter.from_keras_model_file('mobilenetv3_fine_tuned.keras')
model = converter.convert()
with open('mobilenetv3_fine_tuned.tflite' , 'wb') as f:
    f.write(model)

