# Control engineering

Our beloved strandbeests are so special, because they don't have wheels, but
... things ? Because of this, they don't go very straight. And, in order to
have cleaner and less random moving, we decided to implement some control
engineering.

> __Control engineering__ or __control systems engineering__ is an
> engineering discipline that deals with control systems, applying control
> theory to design equipment and systems with desired behaviors in control
> environments.[^1]

[^1]: engineering.case.edu. Case Western Reserve University.
20 November 2015. Retrieved 27 June 2017.

One will notice that control engineering isn't that useful for random
exploration. But we actually knew that we would need this later on, for stuff
like mapping, or any other features that require non-random moves.

# Implementation

The implementation is quite simple to explain to someone having a vague idea
of what a [PID controller](https://en.wikipedia.org/wiki/Proportional%E2%80%93integral%E2%80%93derivative_controller) is.
If you don't, please follow the link above to understand its principle.

We retrieve the acceleration data from the accelerometer and the gyroscope.
- The accelerometer gives linear acceleration, on the x, y and z axis ;
- The gyroscope gives the rotational speed:
[pitch, yaw and roll](https://en.wikipedia.org/wiki/Aircraft_principal_axes).

In order to get the absolute angle of our robot, we just have to integrate the
angle speed over time.

> The accelerometer and the gyroscope are part of a same sensor.

All of this reading and integrating takes place in a class we called
`AxelGyroSensor`.

For the PID controller, as we're pretty lazy, we used a pre-built library:
[https://www.arduino.cc/reference/en/libraries/pid_v2/](https://www.arduino.cc/reference/en/libraries/pid_v2/).

## SmartMotors class

The `SmartMotors` class is the one holding all the logic behind what we
discussed so far.

It holds an instance of `Motors`, and an instance of `AxelGyroSensor`. It uses
the `Motors` instance to actually control the motors, and the `AxelGyroSensor`
instance to feed its PID controller, which modifies the control of the motors.

From outside, it just looks like a `Motors` class that goes straight and is
able to do the moves we actually want it to do. In other words, this the class
implementing control engineering. Our `RescueBot` class is only fed with an
instance of `SmartMotors` to control the motors. Check the UML diagram of this
class to have a visual overview of this.

> You can find an UML diagram for every class of this project, in the section
> `Modules` or `Class List`.

You might notice that the class also holds an instance of `Communication<radioModule>`.
It's only used for debugging issues, like saying when the sensors are calibrating.

# Problems left

Well ... it's not that perfect, in fact. Our strandbeest doesn't walk much
straight when asked. Maybe it's because we badly chose our PID coefficients
or maybe that our sensors aren't that great after all. The best it can do is
go straight for some time (about 5 meters at its best) when going fast
enough. At slow speed, our control engineering sucks.

On top of that, the angle retrieved from integration is not very precise, and
drifts with time, which leads us to the next section ...

# How to do better

The problem with the gyroscope sensor is that it's drifting through time.
This problem doesn't come from our implementation, but from the sensor itself.
To counter this, one can implement a complementary filter or a
[Kalman filter](https://en.wikipedia.org/wiki/Kalman_filter).

You might notice that we do have a complementary filter lost in our code. We
actually tried to implement this, with no success. _We have to say that we
haven't tried that much ..._ Still, you're free to take back this half
implementation and fix it to have no drifting over time. And if you have some
mathematician around you, you could even try to implement a Kalman filter.
