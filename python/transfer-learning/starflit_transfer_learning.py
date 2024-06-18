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
# Here, I placed my dataset in a folder named Colab Notebooks :

# In[ ]:


import os
from google.colab import drive

drive.mount("/content/drive")
os.chdir("/content/drive/MyDrive/Colab Notebooks/LeavesDataset")


# ## Dependencies
#
# We're working in a notebook here, so we don't have to bother with virtual environment.
#
# This also means that we don't have a clear list of dependencies such as `requirements.txt`.
#
# Hence, we install the dependencies as such:

# In[ ]:


# get_ipython().system('pip install tensorflow')


# Do not hesitate to edit the list of dependencies.
#
# And just for the record, we output here the list of dependencies, so one know what version we installed:

# In[ ]:


# get_ipython().system('pip freeze')


# And we import all we need here:

# In[ ]:


from tensorflow.keras.preprocessing.image import ImageDataGenerator
from tensorflow.keras.applications import MobileNetV3Large
from tensorflow.keras.layers import Dense, GlobalAveragePooling2D
from tensorflow.keras.models import Model
from tensorflow.keras.optimizers import Adam

import matplotlib.pyplot as plt


# ## Feature extraction
#
# Feature extraction is the first step of transfer learning.
#
# Here, we'll preprocess our images and split them into different splits.

# In[ ]:


# Preprocessing and data augmentation
train_datagen = ImageDataGenerator(
    rescale=1.0 / 255,
    rotation_range=40,
    width_shift_range=0.2,
    height_shift_range=0.2,
    shear_range=0.2,
    zoom_range=0.2,
    horizontal_flip=True,
    fill_mode="nearest",
    validation_split=0.001,  # Keep only a part of the dataset, represented as a percent (between 0 and 1)
)

# Extract train dataset
train_generator = train_datagen.flow_from_directory(
    "./", target_size=(224, 224), batch_size=32, class_mode="binary", subset="training"
)

# Extract validation dataset
validation_generator = train_datagen.flow_from_directory(
    "./",
    target_size=(224, 224),
    batch_size=32,
    class_mode="binary",
    subset="validation",
)


# Now, we have two splits, data augmented.
#
# Our next objective is to load the model we have to train and add some layers.

# In[ ]:


# Load the MobileNetV3Large model with the imagenet weights as initial weights
base_model = MobileNetV3Large(
    weights="imagenet", include_top=False, input_shape=(224, 224, 3)
)

# Now, we add our custom layers
x = base_model.output
x = GlobalAveragePooling2D()(x)
x = Dense(1024, activation="relu")(x)
predictions = Dense(1, activation="sigmoid")(x)
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
model.compile(optimizer=Adam(), loss="binary_crossentropy", metrics=["accuracy"])


# And now is the time we actually train the model.
#
# The following codeblock will probably last for long, and that's why we have trouble running this notebook where we want :'(

# In[ ]:


history = model.fit(train_generator, epochs=1000, validation_data=validation_generator)


# And now, we have our trained model, tuned to our dataset.
# Feature extraction is over, and we'll proceed to _fine-tuning_ is few seconds,
# just after a quick model save!

# In[ ]:


model.save("mobilenetv3_tuned.h5")


# ### Testing tuned model
#
# Note that the model is actually tuned for our dataset.
# Hence, now is a good timing to have some testing.

# In[ ]:


loss, accuracy = model.evaluate(validation_generator)

print(f"Validation loss: {loss}")
print(f"Validation accuracy: {accuracy}")

acc = history.history["accuracy"]
val_acc = history.history["val_accuracy"]
loss = history.history["loss"]
val_loss = history.history["val_loss"]

epochs_range = range(len(acc))

plt.figure(figsize=(8, 8))
plt.subplot(1, 2, 1)
plt.plot(epochs_range, acc, label="Training Accuracy")
plt.plot(epochs_range, val_acc, label="Validation Accuracy")
plt.legend(loc="lower right")
plt.title("Training and Validation Accuracy")

plt.subplot(1, 2, 2)
plt.plot(epochs_range, loss, label="Training Loss")
plt.plot(epochs_range, val_loss, label="Validation Loss")
plt.legend(loc="upper right")
plt.title("Training and Validation Loss")
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
model.compile(optimizer=Adam(1e-5), loss="binary_crossentropy", metrics=["accuracy"])


# And now we re-train the model.
# This is also kind of long to run, same as previously.

# In[ ]:


history_fine = model.fit(
    train_generator, epochs=1000, validation_data=validation_generator
)


# And let's re-save our model a second time, but fine-tuned this time.
# This is the model we'll be using in our beloved Strandbeest.

# In[ ]:


model.save("mobilenetv3_fine_tuned.h5")


# ### Testing fine-tuned model
#
# Now, everything is set to have some little performance tests, using the validation dataset.

# In[ ]:


loss, accuracy = model.evaluate(validation_generator)

print(f"Validation loss: {loss}")
print(f"Validation accuracy: {accuracy}")

acc = history_fine.history["accuracy"]
val_acc = history_fine.history["val_accuracy"]
loss = history_fine.history["loss"]
val_loss = history_fine.history["val_loss"]

epochs_range = range(len(acc))

plt.figure(figsize=(8, 8))
plt.subplot(1, 2, 1)
plt.plot(epochs_range, acc, label="Training Accuracy")
plt.plot(epochs_range, val_acc, label="Validation Accuracy")
plt.legend(loc="lower right")
plt.title("Training and Validation Accuracy")

plt.subplot(1, 2, 2)
plt.plot(epochs_range, loss, label="Training Loss")
plt.plot(epochs_range, val_loss, label="Validation Loss")
plt.legend(loc="upper right")
plt.title("Training and Validation Loss")
plt.show()
