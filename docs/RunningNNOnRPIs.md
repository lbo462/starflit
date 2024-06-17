# Everything you need to know about running neural networks on a RPi

## Introduction

Running a neural network (referred to as **NN** from now on) on a RaspberryPi is quite an interesting project which involves many challenges mainly related to the VERY limited computing capacity of these nano computers.

However, these challenges can be overcome, leading to very interesting results, and makes us realize that a RPi is in fact a quite capable little computer.

For this project, our goal was to run an object detection NN in order for our Strandbeest to detect a specific object (a leaf in our case) when exploring a room.

> **Note:** The words _"NN"_ (Neural Network) and _"model"_ will be used interchangeably from now on, as one commonly refers to the mathematical representation and the latter, the code representation.

### Table of Contents

In this article, we will go over EVERYTHING (probably not but I'll try) there is to know about running an object detection NN on a RPi:

- [What is a Neural Network?](#What-is-a-neural-network)
- [Frameworks](#Frameworks)
- [What is a model?](#What-is-a-model)
- [Setting up a dev environment](#Setting-up-a-dev-environment)
- [Training a model](#Training-a-model)
- [Transforming a model from one format to another](#Transforming-a-model-from-one-format-to-another)
- [Setting up the RPi tu run a model](#Setting-up-the-rpi)
- [Running inference](#Running-inference)
- [Benchmarking](#Benchmarking)

As a bonus, I will also provide a quick tutorial on running a model and seeing its output via SSH so that you don't need to connect a monitor to your RPi. **ENFAITE NON** (lol)

Bonus 2: work on running on the GPU of the RPi for much better performance.

## What is a Neural Network?

<!--TODO-->

### Types of NN

<!--TODO-->

## Frameworks

In order to program a NN, you will need to use a framework. There are many to choose from, but the most popular ones are:

- TensorFlow
- PyTorch
- Keras (library for TensorFlow)
- NCNN (for mobile).

A framework allows you to build, train and run NNs. For this project, we chose TensorFlow Lite, which is the lightweight version of the TensorFlow framework meant to run on embedded devices, smartphones or any other computationally limited device.

## What is a model?

A model is a file that contains all of the information of a model necessary to run inference on it. This includes:

- the model architecture (number and type of layers, number of neurons...) in the form of `json` or `yaml` files
- the weights and biases determined during the learning process
- metadata.

The three main most popular model types are:

- `.pt` or `.pth` for PyTorch
- `.pb` or `.tflite` for TensorFlow
- `.onnx` for ONNX models (we'll talk more about them later).

## Setting up a dev environment

Before you go any further, you need to setup a python environment with the necessary packages so that you can start programming. Programming directly on the RPi directly isn't a very good idea for many reasons, one of which being that training the NN will take A LOT of time.

> Disclaimer: this tutorial has been tested on `Ubuntu 24.04 LTS` on an `x86_64` machine, with `Python 3.12.3`. Some steps might differ and some packages might not be available/up to date on other plateforms/distributions.


### Installing Tensorflow

In order to train a tensorflow model, you will need the full TensorFlow package.

### Installing `tflite-runtime`

The `tflite-runtime` `pip` package isn't available for `x86_64` machines running `python 3.12`, so we need to build it from source 🙃.

> Don't worry, we won't be reinventing the wheel, we'll just be building it 😀 (cause we're building a python wheel, get it 😀😀😀🔫).

> Wow, tes blagues Boris c'est chaud :/

All of the detailed instructions are available [here](https://www.tensorflow.org/lite/guide/build_cmake_pip), but here's the gist:

1. Install `Cmake`:

```bash
sudo apt-get update
sudo apt-get install cmake
```
2. Clone the TensorFlow repository:

```bash
git clone https://github.com/tensorflow/tensorflow.git tensorflow_src
```

3. Run the build script:

```bash
PYTHON=python3 tensorflow/lite/tools/pip_package/build_pip_package_with_cmake.sh native
```

> You might get errors about missing packages. If it's the case, simply `pip install` them. 

It takes quite a while to run, so go make yourself a coffee ☕.

4. `pip install` the generated wheel, preferably in a `venv`:

```bash
pip install tflite_runtime-2.17.0-cp312-cp312-linux_x86_64.whl
```

If you run the `pip list` command, you should now see `tflite-runtime` listed.

### Installing `picamera2` (for development only ⚠)

The `picamera2` dependency is natively installed  on RPis. Thus, you don't need to install it. **However**, in order to develop on another plateform than a RPi, you will need to install the `picamera2` `pip` package and the libcamera package (a verifier).

To be able to use the `picamera2` package in your python virtual env, you will need to pass the `--system-site-packages` argument when you create it:

```bash
virtualenv venv --system-site-packages
```

## Training a model

## Transforming a model from one format to another

This is actually quite easy. You first need to convert your model to the ONNX format.

> ONNX (Open Neural Network eXchange) is an open standard used to represent machine learning algorithms.

You can do it by following these steps:

<!--TODO-->

For the folowing specific models, follow these links : 

- yolov8 : https://docs.ultralytics.com/integrations/tflite/#installation 

- yolov5 : https://docs.ultralytics.com/yolov5/tutorials/model_export/#colab-pro-cpu. 

If you have an error like this : 
```bash
export yolov5 to tflite error: missing attribute 'value' LLVM ERROR: Failed to infer result type(s). Aborted
```
You may encounter some problems with tensorflow 2.16. So you need to downgrade to tensorflow 2.15 : 

```bash
pip uninstall tensorflow
pip install "tensorflow==2.15.0"
```
- Mobilenetv3 : https://www.kaggle.com/models/google/mobilenet-v3/tfLite/small-075-224-classification. You will find all the MobilenetV3 models downloadable in tflite. 

## Setting up the RPi


### Installing the necessary dependencies/packages

<!-- Install via requirements but also explain all the trouble shooting steps -->

## Running inference

<!--TODO describe the process of running inference on raspberry pi with tflite runtime-->

## Benchmarking

### What to benchmark?

When running NNs on edge devices (devices that don't have a lot of processing power), the most important metrics to optimize are the inference time (aka FPS when talking about objection detection) and accuracy. In order to understand the results of our benchmarks, we must first dive into how these metrics are calculated and what they represent.

If you already know all this or you just don't care, [click here](https://www.youtube.com/watch?v=dQw4w9WgXcQ) to jump directly to our results.

### Types of metrics

#### Inference (FPS)

This metric is analog FPS (frames per seconds). Basically, it's how many images the NN can process per second. Higher is obviously better.

#### AP / mAP

The Average Precision or mean Average Precision denotes as the name suggests the precision. It takes into account two metrics: the **precision** and **recall**.

##### Precision

$$\frac{t_p}{t_p + f_p}$$

$t_p$: true positive, number of correctly detected objects by the model.
$\\f_p$: false positive, number of falsely detected objects by the model.

Simply put, this is the ratio of the number of all the objects the model could have detected over the number of objects it has actually detected (correctly or not).

##### Recall

$$\frac{t_p}{t_p + f_n}$$

$t_p:$ true positive, number of correctly detected objects by the model.
$\\f_p:$ false positive, number of falsely detected objects by the model.

The recall is the ratio of the number of all the objects the model could have detected over the number of objects it has actually detected **AND** hasn't detected when it should have.

 #### Precision-Recall Curve
 
 The AP is the area under the PR curve.
![](https://miro.medium.com/v2/resize:fit:750/format:webp/0*IIP5_lTXigXMViiu.png)


#### IoU

IoU (Intersection over Union) is a very important metric when talking about object detection.
Here is an illustration to understand the concept : 
![](https://huggingface.co/datasets/huggingface/documentation-images/resolve/main/blog/object-detection-leaderboard/iou.png)

When the 2 boxes perfectly overlap, the IoU = 1.
When the 2 boxes have no shared pixels, the IoU = 0.

- Ap75 means that a detection is considered true when the IoU is superior as 0.75
- AP50 means that a detection is considered true when the IoU is superior as 0.5
- AP is the average precision for different IoU, usually from 0.5 to 0.95 with a 0.05 pa

The higher AP, the better, the max being 100%.


*Recall that the AP depends of the context and of the dataset that the model has been trained on. 
For example, for a model trained on the COCO dataset, a very good AP is (currently) 40. In opposition, for the dataset PASCAL VOC, which is less complex, the best models can go to scores of 70.*


#### FLOPs (Floating-point Operations per second)

FLOPS means floating point operations per second. FLOPS represents the number of floating-point operations performed by a model during inference. It’s a measure of computational complexity and efficiency.

It tells you how many “calculations” the model needs to do to understand an image and find objects in it.

It impact greatly your inference time.

### Our benchmark

Caracteristics of the tested models:
- Model's type: CNN
- Trained dataset: [coco](https://cocodataset.org)
- Framework: tflite

| Name               | Size (Mb) | Parameters (Millions) | Accuracy  (AP)     | Inference (s) | Flops |
| ------------------ | --------- | --------------------- | -------------- | ------------- | ----- |
| MobileNetV2        | 6.1       | 4.3                   | 22.1         | 0.08          | 0.32  |
| MobileNetV3 S      | 8.21      | 1.5                   | 16.1         | 0.02          | 0.02  |
| MobileNetV3 L      | 16        | 5                     | 22           | 0.055         | 0.23  |
| YoloV5 Nano        | 3.85      | 1.9                   | 28    | 0.52          | 4.5   |
| YoloV5 S           | 14.57     | 7.2                   | 37.4  | 3.4           | 16.5  |
| Yolov8 N float16   | 6.4       |                       | 37    | 0.8           | 8.7   |
| Yolov8 N float32   | 12.86     |                       | 37    | 0.97          | 8.7   |
| EfficientDet-lite0 | 4.4       |      3.2                 | 26           | 0.19          |       |


### How to get your **Inference Time**

The tool we are using for benchmarking the inference of the different models is the following:

https://www.tensorflow.org/lite/performance/measurement

It allows us to quickly benchmark the inference time of a model on a raspberry pi.

### How to get your **Accuracy**




## BONUS

<!--TODO-->
