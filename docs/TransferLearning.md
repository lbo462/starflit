# Transfer learning and fine-tuning

> Note that this part exist because we made research and tried to fine-tune a model,
> but we couldn't be able to finish the process in the given time.
> This documentation contains some information that could be useful for future projects.

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

Theses docs contains all the knowledge you need to continue reading this.
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

## HuggingFace 🤗

In order to use a custom dataset for the transfer learning, we use
[HuggingFace 🤗](https://huggingface.co/),
which is pretty similar to GitHub, but fitted for the AI.

Here, we'll only use it to host our dataset.

> WARNING! If you plan on using HuggingFace to host dataset for Tensorflow/Keras,
> be aware that you should be using only lower case characters (and no `-`)
> in your user name and the dataset's name.
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

## Running model training

Training a model is quite costly in terms of computation power.
That's why the Starflit crew decided to run this bit of the project on
[Google Colab](https://colab.research.google.com/).

Time ran out and we could be able to create our own notebook.
Still, we would have made a one pretty similar to the one from
[Keras](https://keras.io/guides/transfer_learning/).

> You may want to write your custom notebook.
> To do so, you're encouraged to use Google Colab because of performance issues.
> Still, you're also highly encouraged to export the final notebook
> to a `.ipynb` file that you'll put in you Github repository.
> This way, next groups will become independent of your Google Colab and
> will be able to import the notebook in their own environment.

After you've run your notebook code, you should be able to download the model in the `.tflite` format:

```python
with open('model.tflite', 'wb') as f:
  f.write(tflite_model)
```

Now, one just have to input this model file into the project to run the model on the strandbeest.

Just for the record, note that the tfds documentation was quite incomplete concerning importing hugging face datasets,
and that a [PR](https://github.com/tensorflow/datasets/pull/5462) was opened to add a bit of documentation.
Hopefully, it was merged by the time you're reading this.
