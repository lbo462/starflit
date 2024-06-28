# Transfer learning and fine-tuning

## The objective 
The objective of this part of the project, is to use a model tht already exist, and to use it's already intelligent structure to use it to do something else.

After the benchmark, we decided that MobileNetv3Large was the most adapted model for our needs. MobileNet is a model that has 1000 classes, so it can recognize 1000 different objects. In our case, we want it to be able to detect people in distress/wounded and people that are safe. So only 2 classes. 

We chose to use leafs to represent people, for there are few datasets available of wounded people.


## Fine-tuning with Pytorch

During the last days of the project, we manage to finetune MobileNetv3Large with pytorch. After getting the finetuned model, we managed to make it run on the Raspberry Pi. 

The model had poor results, but the POC is here, and your mission, if you accept it, is to improve and adjust the training.

### Training

First, you will have to download the script files that are in `misc/finetuning/finetuning.py` on the repo on the machine you want to do the training. They might be :  [here](https://www.youtube.com/watch?v=dQw4w9WgXcQ) 


Once you have enjoyed downloading them, you will have to create a virtual environnement :
```
python3 -m venv venv
source venv/bin/activate
```

Then install the dependencies :
```
pip install torch torchvision tqdn
```
And finally start the training in the background :
```
nohup python3 finetuning.py > training.log &
``` 

*N.B. :
If you are connected with ssh on the machine where the training is happening, you can disconnect yourself, and check the results in real time on the training.log file*

At the end of the training, you will have a .pth file : this is your fine tuned model.


### Running the model on Raspberry Pi

First, transfer the model from the machine you made the training to the Raspberry Pi. If you are using a remote server, use rsync from the raspberry pi : 
```
rsync -Pru login@ip:/path/to/the/model .
```

Then once you have it, same as before, you create your virtual environnement :
```
python3 -m venv venv
source venv/bin/activate
```

Then install the dependencies :
```
pip install torch torchvision tqdn
```

Once it's done, you can download the script files that are in `misc/finetuning/objectRecognition.py` on the repo, and start the script (assuming you configured correctly your Raspberry Pi).


Okay, so now you have a script that give's you a prediction of what the camera see's with a certitude percentage.

### What's left

#### The training

The model, as mentionned before, has poor results. We didn't spend much time on improving the training, but this is something you can do. 

I would advise that you don't rely too much on the scripts that were made. There were just POCs, and can be largely improved.

#### The implementation

The script is not implemented in the rest of the project. If you want to use it on the Strandbeests, you would have to implement the code in the actual project.

Good luck soldier!



## Fine-tuning with TensorFlow
> Note that this part exists because we made research and tried to fine-tune
> a model, but we weren't able to finish the process in the given time.
> This documentation contains some information that could be useful for
> future projects.

You might already have heard of _fine-tuning_ to train a pre-existing model 
on a more specific dataset. In fact, _fine-tuning_ is an optional step to 
increase your model's performance during __transfer learning__.

In brief, transfer learning is:
- _feature extraction_ is the modifying of the model architecture by adding 
layers to an existing model, and train this new model on the new layers only, 
by freezing the other layers.
- _fine-tuning_ is an optional step that consists of unfreezing (almost) all
the layers and training the model with a pretty low learning rate (to avoid
overfitting).

For our needs, we first tried to follow the documentation from TensforFlow at 
[https://www.tensorflow.org/tutorials/images/transfer_learning](https://www.tensorflow.org/tutorials/images/transfer_learning),
but at the time I'm writting this, this documentation seems to have an issue
(check it [here](https://github.com/tensorflow/tensorflow/issues/69480)).

Instead, we followed the Keras documentation at
[https://keras.io/guides/transfer_learning/](https://keras.io/guides/transfer_learning/).

Theses docs contains all the knowledge you need to continue reading this.
Please, take a bit of your time to read at least one of the two in order to
understand the process of transfer learning and fine-tuning.

### Starflit's transfer learning objectives

Our save-and-rescue little animated strandbeest have the objective of 
recognizing an object in two different states: _healthy_ or _unhealthy_.
Here, we chose that this object will be leaves 🍃.

> As you might know, there's no much use of recognizing heathly leaves from 
> unhealthy ones. But showing how we can fine tune a model and implement it 
> on the strandbeest will allow the next groups to do the same with different
> datasets, for different uses.

### HuggingFace 🤗

In order to use a custom dataset for the transfer learning, we tryed using 
[HuggingFace 🤗](https://huggingface.co/), which is pretty similar to
GitHub, but fitted for AI.

Here, we'll only use it to host our dataset.

> WARNING! If you plan on using HuggingFace to host a dataset for Tensorflow/
> Keras, be aware that you should be using only lower case characters (and no 
> `-`) in your user name and the dataset's name. This comes from the fact 
> that [tfds](https://www.tensorflow.org/datasets/overview) parses camel case
> to turn them into snake case. See
> [https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794](https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794)
> and
> [https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794](https://github.com/tensorflow/datasets/issues/5275#issuecomment-1929182794)
> to check for updates.

### Creating your dataset

In order to create a dataset, there's two docs we could have followed:
- The one from [Tensorflow](https://www.tensorflow.org/datasets/add_dataset)
- The one from [HuggingFace](https://huggingface.co/docs/datasets/image_dataset)

The tensorflow documentation tells us to use their `tfds` cli tool to 
generate a dataset. This method is better for big and professional projects, 
but we'll prefer to go with a simpler solution, given by HuggingFace.

This solution allows us to have our images in a folder and generate a fully 
labelled dataset with a single line of python code. Everything is described 
in the documentation above.

### Running model training

Training a model is quite costly in terms of computation power.
That's why the Starflit crew decided to run this bit of the project on 
[Google Colab](https://colab.research.google.com/).

Time ran out and we could be able to create our own notebook.
Still, we would have made a one pretty similar to the one from
[Keras](https://keras.io/guides/transfer_learning/).

> You may want to write your custom notebook. To do so, you're encouraged to 
> use Google Colab because of performance issues. Still, you're also highly 
> encouraged to export the final notebook to a `.ipynb` file that you'll put 
> in you Github repository. This way, next groups will become independant of
> your Google Colab and will be able to import the notebook in their own
> environment.

After you've run your notebook code, you should be able to download the
model in the `.tflite` format:

```python
with open('model.tflite', 'wb') as f:
  f.write(tflite_model)
```

Now, one just have to input this model file into the project to run the 
model on the strandbeest.

Just for the record, note that the tfds documentation was quite incomplete
concerning importing hugging face datasets, and that a
[PR](https://github.com/tensorflow/datasets/pull/5462) was opened to add a 
bit of documentation. Hopefully, it was merged by the time you're reading 
this.

---

# Mapping

__SLAM__ : Simultaneous localization and mapping

*Notabene* :

*We didn't get this far in our project. We had the time to make some research and to explore the subject, but that's all. What we are writing here wasn't tested, and we are simply reporting the results of our exploration of the subject.*

*Before we start, we strongly recommend that you* **explore the subject yourself**, *and that you do not only use the information that is written here. We are discovering the subject and* **we are in no case references in the matter.**

## The objective

This part of the project has for objective to implement a shared map for the Strandbeests.

 We want the strandbeests to:
 - explore an unknown environment
 - create a map of this environment
 - share their map with the swarm
 - use their map to navigate from A to B
 - use the swarm map to navigate from A to B

The subject is very large, and is called SLAM for: simultaneous localization and mapping.

The navigation part should be its very own, but we haven't had the time to explore this part so we integrated it here.

## What is SLAM

SLAM is not a specific algorithm, nor an application. It's a term to designate the technological process that allows robots to navigate in an autonomous way, in an unknown environment.

SLAM asks to simultaneously localize the robot in its environment and to create the map of this environment. It is a complex problem, because in order to know the position of the robot, one needs to have the map of the environment.

**There exists many types and methods of SLAM implementation:** 

- **Bayesian Filters**: Used to estimate the robot's position while accounting for uncertainties. The main filters include the Extended Kalman Filter (EKF) and the Particle Filter 
  - Extended Kalman Filter (EKF-SLAM)
  - Particle Filter (FastSLAM)

- **Graph-Based Algorithms**: These algorithms model the SLAM problem as a graph where nodes represent the robot's positions and edges represent relative measurements between these positions
  - g2o 
  - GTSAM 

- **Visual SLAM**: Uses cameras for mapping and localization, often in combination with computer vision techniques such as feature extraction and tracking
  - MonoSLAM
  - ORB-SLAM

### Landmarks
We can use landmarks to locate ourselves in the environment. 

Landmarks are essential elements of SLAM, enabling the robot to localize itself and map its environment effectively. The appropriate selection, detection, and utilization of landmarks are crucial aspects for the success of any SLAM system.

There can be different types of landmarks:
- Natural Landmarks: These are intrinsic features of the environment, such as corners of walls, edges of objects, or distinctive points detected by sensors (e.g., visual features like window corners or textured patterns).

- Artificial Landmarks: These are specially placed markers in the environment to facilitate SLAM tasks, such as AR markers, reflective targets for laser sensors, or RFID tags.

### Why is SLAM a hard problem

In order to map the environment, we need to know where we are on our map, which we are creating at the same time. It can be compared to the problem of "the chicken or the egg".

**The first thing is to estimate how well (or badly) we are moving.**

For example, imagine we want to move like this:
- the round is the robot
- the stars represent landmarks

![](assets/SLAMIntendedMovement.png)
*[Youtube : SLAM-course-1](https://www.youtube.com/watch?v=wVsfCnyt5jA&list=PLgnQpQtFTOGQrZ4O5QzbIHgl3b1JHimN_&index=3)*

The problem is that we have a lot of uncertainty:
- Sensor Noise 
    - Sensor data (such as LiDAR, cameras, or GPS) contains noise, complicating precise localization and mapping
- Movement noise
    - The motors don't always move as intended
- Data Association Ambiguity
    - Correctly identifying previously observed features of the environment (landmarks) is challenging
- Dynamic Environment
    - Changes in the environment (e.g., moving people) can disrupt measurements

### Deviation

Our robot doesn't move as intended and doesn't describe exactly the reality. So the real movement of the robot compared to the intended movement looks like this:

![](assets/SLAMDeviation.png)
*[Youtube : SLAM-course-1](https://www.youtube.com/watch?v=wVsfCnyt5jA&list=PLgnQpQtFTOGQrZ4O5QzbIHgl3b1JHimN_&index=3)*

It leads the robot to miss the landmark, and therefore, to miscreate the map.

*Your robot will most likely always deviate, even a little bit, except in very specific situations (on a rail for example).*

One way to represent the map is to evaluate the level of uncertainty of the position and of landmarks:
![](assets/SLAMHardProblem.png)
*[Slides : SLAM-fastslam](http://ais.informatik.uni-freiburg.de/teaching/ss13/robotics/slides/14-slam-fastslam.pdf)*

### Misinterpretation 

Another problem is that our robot can misinterpret the landmarks:

![](assets/SLAMHardProblem2.png)
*[Slides : SLAM-fastslam](http://ais.informatik.uni-freiburg.de/teaching/ss13/robotics/slides/14-slam-fastslam.pdf)*

## Our case

In our case, we have strandbeests that have legs:

![Photo Strandbeest](assets/strandbeest.png)

The major inconvenience with this is that it brings randomness in the movement. For example, when we start the Strandbeest and its right leg is in a specific position, the whole robot will deviate to the right. In the context of SLAM, where the more precise you are the better, this is a major problem.

![](assets/SLAMLandmarks.png)
*[Slides : SLAM-fastslam](http://ais.informatik.uni-freiburg.de/teaching/ss13/robotics/slides/14-slam-fastslam.pdf)*

We didn't accomplish perfect control engineering. 

One solution would be to do better than us:

- For example, by using the Kalman filter (not going to be easy, but it can be interesting). Another solution would be to have a better gyroscope and accelerometer. With a more precise sensor, the drifts would be detected, and could be corrected.

- Another thing to take into account is that we only have the ultrasonic sensors to do the mapping. We could use the camera, but it's already used by the object detection.

In our case, we don't want to have a map on each robot. We want to have a shared map of the environment that the swarm shares.

To do that you would have to implement **multi-agent SLAM** :
- [Research paper : C-SAM Multi-Robot SLAM ](https://www.researchgate.net/figure/Two-robots-rendezvous-after-exploration-The-individually-created-maps-are-shared-and_fig1_224318651)
- [Research paper: Multi-agent SLAM](https://rpg.ifi.uzh.ch/docs/thesis_Cieslewski_final.pdf)


## What to do 

In the Links section, you can inspire yourself with the Github repositories and other links. We would recommend starting by researching the subject, and then trying the [Github: Arduino SLAM using ultrasonic sensors](https://github.com/PatelVatsalB21/Ultrasonic-SLAM).

## Links 
- [Research paper: SLAM in weak environment information applications using Swarm robots](https://www.researchgate.net/publication/379134679_SLAM_in_Weak_Environment_Information_Applications_using_Swarm_Robots)
- [Github: Arduino SLAM using ultrasonic sensors](https://github.com/PatelVatsalB21/Ultrasonic-SLAM)
- [Youtube: SLAM-course-1](https://www.youtube.com/watch?v=wVsfCnyt5jA&list=PLgnQpQtFTOGQrZ4O5QzbIHgl3b1JHimN_&index=3)
- [Slides: SLAM-fastslam](http://ais.informatik.uni-freiburg.de/teaching/ss13/robotics/slides/14-slam-fastslam.pdf)
- [Research paper: Multi-agent SLAM](https://rpg.ifi.uzh.ch/docs/thesis_Cieslewski_final.pdf)
- [Github: SLAM on Raspberry PI](https://github.com/AdroitAnandAI/SLAM-on-Raspberry-Pi?tab=readme-ov-file)
- [Research paper : C-SAM Multi-Robot SLAM ](https://www.researchgate.net/figure/Two-robots-rendezvous-after-exploration-The-individually-created-maps-are-shared-and_fig1_224318651)