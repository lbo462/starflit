# Transfer learning and fine-tuning

You might already have heard of _fine-tuning_ to train a pre-existing model on a more specific dataset.
In fact, _fine-tuning_ is an optional step to increase your model's performance during __transfer learning__.

In brief, transfer learning is:
- _feature extraction_ is the modifying of the model architecture by adding layers to an existing model,
and train this new model on the new layers only, by freezing the other layers.
- _fine-tuning_ is an optional step that consists of unfreezing (almost) all the layers
and training the model with a pretty low learning rate (to avoid overfitting).

For our needs, we first tried to follow the documentation from TensforFlow at
[https://www.tensorflow.org/tutorials/images/transfer_learning](https://www.tensorflow.org/tutorials/images/transfer_learning),
but at the time I'm writing this, this documentation seems to have an issue (check it
[here](https://github.com/tensorflow/tensorflow/issues/69480)).

Instead, we followed the Keras documentation at
[https://keras.io/guides/transfer_learning/](https://keras.io/guides/transfer_learning/).

These docs contain all the knowledge you need to continue reading this.
Please, take a bit of your time to read at least one of the twos
in order to understand the process of transfer learning and fine-tuning.

## Starflit's transfer learning objectives

Our save-and-rescue little animated strandbeest have the objective of
recognizing an object in two different states : _healthy_ or _unhealthy_.
Here, we chose that this object will be leaves 🍃.

> As you might know, there's no much use of recognizing healthy leaves from unhealthy ones.
> This whole project is a first step for more ambitious projects.
> Showing how we can fine tune a model and implement it on the strandbeest
> will allow the next groups to do the same with different datasets,
> for different uses.

## HuggingFace 🤗 (obsolete)

In order to use a custom dataset for the transfer learning, we tried using
[HuggingFace 🤗](https://huggingface.co/),
which is pretty similar to GitHub, but fitted for the AI.

Here, we'll only use it to host our dataset.

> WARNING! If you plan on using HuggingFace to host dataset for Tensorflow/Keras,
> be aware that you should be using only lower case characters (and no `-`)
> in your username and the dataset's name.
> This comes from the fact that
> [tfds](https://www.tensorflow.org/datasets/overview) parses camel case to turn them into snake case.
> See [https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794](https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794)
> and [https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794](https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794)
> to check for updates.

### Creating your dataset

In order to create a dataset, there's two docs we could have followed:
- The one from [Tensorflow](https://www.tensorflow.org/datasets/add_dataset)
- The one from [HuggingFace](https://huggingface.co/docs/datasets/image_dataset)

The tensorflow documentation tells us to use their `tfds` cli tool to generate a dataset.
This method is better for big and professional projects,
but we'll prefer to go with a simpler solution, given by HuggingFace.

This solution allows us to have our images in a folder and generate a fully labelled dataset with a single line of python code.
Everything is described in the documentation above.

### Why is this obsolete ?

We truly tried to host our custom dataset on Hugging Face,
but we couldn't manage to import it through `tfds.load()`.

Instead, we chose the option of hosting our dataset locally (or on Google Drive).
Even if this is not the best option, this one actually works for our needs.

Feel free not to do as we did, and host your dataset on Hugging Face or Kaggle.

Just for the record, note that the tfds documentation was quite incomplete concerning importing hugging face datasets,
and that a [PR](https://github.com/tensorflow/datasets/pull/5462) was opened to add a bit of documentation.
Hopefully, it was merged by the time you're reading this.

## Running model training and trained model

Training a model is quite costly in terms of computation power.
That's why the Starflit crew decided to try to run this bit of the project on
[Google Colab](https://colab.research.google.com/).
But without a GPU, the training took very long, and we thus used a computer from the CITI lab.

In the `python/transfer-learning` folder of this repository, you'll find a notebook (and its `.py` version)
implementing some basic documentation, inspired from the Keras documentation.
In order to run your transfer learning on your custom dataset, you should definitely take a look at this notebook,
and arrange it in the way you need.
Note that this notebook can easily be read on VSCode or Pycharm, but can also be uploaded on Google Colab.

> To run the model training, we copied the dataset and the python version of the notebook on the CITI computer via `rsync`.

To use the fine-tuned model on the strandbeest,
download the file on the strandbeest and select it while running the main script.

> Note that the python version of the notebook was generated using
> `jupyter nbconvert --to python notebook.ipynb`,
> and that the conversion isn't perfect regarding the non-python code.
> 
> You're free to edit the generated python version and run the code in a virtual env,
> installing all the dependencies in it.
> 
> You should remove (or comment) :
> - The Google colab part
> - The non-python commands
> - If no GUI's available, the plotting with matplotlib