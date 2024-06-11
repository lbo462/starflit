# Transfer learning and fine-tuning

You might already have heard of _fine-tuning_ to train a pre-existing model on a more specific dataset.
In fact, _fine-tuning_ is an optional step to increase your model's performance during __transfer learning__.

In brief, transfer learning is:
- _feature extraction_ is the modifying of the model architecture by adding layers to an existing model, and train this new model on the new layers only, by freezing the other layers.
- _fine-tuning_ is an optional step that consists of unfreezing (almost) all the layers and training the model with a pretty low learning rate (to avoid overfitting).

For our needs, we first tried to follow the documentation from TensforFlow at [https://www.tensorflow.org/tutorials/images/transfer_learning](https://www.tensorflow.org/tutorials/images/transfer_learning), but at the time I'm writting this, this documentation seems to have an issue (check it [here](https://github.com/tensorflow/tensorflow/issues/69480)).

Instead, we followed the Keras documentation at [https://keras.io/guides/transfer_learning/](https://keras.io/guides/transfer_learning/).

Theses docs contains all the knowledge you need to continue reading this.
Please, take a bit of your time to read at least one of the twos in order to understand the process of transfer learning and fine-tuning.

## Starflit's transfer learning objectives

Our save-and-rescue little animated strandbeest have the objective of recognizing an object in two different states : _healthy_ or _unhealthy_.
Here, we chose that this object will be leaves 🍃.

> As you might know, there's no much use of recognizing heathly leaves from unhealthy ones.
> This whole project is a first step for more ambitious projects.
> Showing how we can fine tune a model and implement it on the strandbeest will allow the next groups to do the same with different datasets, for different uses.

### Running model training

Training a model is quite costly in terms of computation power.
That's why the Starflit crew decided to run this bit of the project on [Google Colab](https://colab.research.google.com/).

In the GitHub repository of the project, you'll a Python notebook to import into Google Colab.
This Python notebook contains a bit of documentation and the code to generate a fine-tuned model to detect leaves.
Download the file, import it, read the documentation it contains and run the code.
Feel free to tune this notebook for your own needs and to generate different models.

After you've run the code, you should be able to download the model in the `.tflite` format.
Now, one just have to input this model file into the project to run the model on the strandbeest.

### HuggingFace 🤗

In order to use a custom dataset for the transfer learning, we use [HuggingFace 🤗](https://huggingface.co/), which is pretty similar to GitHub for the AI.

Here, we'll only use it to host our dataset.

> WARNING! If you plan on using HuggingFace to host dataset for Tensorflow/Keras,
> be aware that you should be using only lower case characters (and no `-`) in your user name and the dataset's name.
> This comes from the fact that [tfds](https://www.tensorflow.org/datasets/overview) parses camel case to turn them into snake case.

Check the starflit's python notebook to see how to retrieve one of your dataset from HuggingFace to fine-tune a pre-trained model.

Just for the record, note that the tfds documentation was quite incomplete concerning this, and that a [PR](https://github.com/tensorflow/datasets/pull/5462) was opened to add a bit of documentation.
Hopefully, it was merged by the time you're reading this.

<!-- TODO : Detail the formatting of the dataset to upload. -->
