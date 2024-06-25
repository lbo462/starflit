# Miscellaneous features

Welcome to the misc. features. Here, you'll find every code related
features that we couldn't sort, but felt the need to talk about.
You're encouraged to use the sidebar in order to navigate here.

## LEDs

The invention of the LED (Light Emmiting Diode) was quite a breakthrough in the history of science. LEDs were first developed in 1907 when Henry Joseph Round discovered electroluminescence. However, the first practical visible-spectrum LED was created in 1962 by Nick Holonyak Jr. while working at General Electric Company. The first visible LED was red, and it wasn't until the 1990s that we were able to create white light with LEDs thanks to the invention of the blue LED, courtesy of Shuji Nakamura.

If you haven't already, you should really take the time to watch this video about the inventor of the [blue LED](https://youtu.be/AF8d72mA41M?si=4Z5RsmfHclT4VkNN).

I've written this needlessly long introduction mostly because I find LEDs to be quite an interesting subject, but  also because there really isn't a lot to say about our implementation of the LEDs on the robot 🙃.

Anyway...

### The one where we discuss the goal

The work that has been done for the LEDs for this project was mostly a side project since the robot doesn't depend on them to work. However, they are really useful when it comes to debugging and providing visual feedback about what is going on under the hood. It makes the robots interactive in a way.

That being said, the goal was to create a color code that would reflect the state that the robot was in in any given time.

The code that has been implemented is the following:

 - magenta "loading like" animation while the robot is waiting for the RPi to initialize
 - blue slow blinking when the robot is in an exploring state
 - red fast blinking when the robot is in a scanning state
 - green fast blinking when the robot has found the specified object
 - rainbow animation whe the robot receives the information that the specified object has been detected.


### The one where we discuss the implementation

The implementation was quite easy really. One small hurdle was the need to implement the blinking animation without using delays. Why can't we use delays you might ask?
Well, when the `delay()` function is called, the program pauses until the delay is over. This prevents the robot from continuing to explore its environement. Since we want the LEDs to be visual feedback WHILE the robot explores, this isn't a good thing.

> This is quite a popular problem on Arduino Uno boards since the microprocessor only has one core, thus one thread.

That being said, the workaround is quite easy.

Every time the blinking function is called, we can check if the elapsed time between the last blink and the current time is superior to the interval we have set. Here is an example:

```cpp
if (currentMillis - previousLedMillis >= interval)
```

> `currentMillis` is the time since the program has started in ms, `previousMillis` the last time le LEDs blinked in ms and `interval` the desired time in between blinks, in ms as well. 

This will allow the program to continue to run normally until enough time has passed to change the state of the LEDs. This is the same logic that has been implemented in the `update()` method of `RescueBot` to have the `scan()` method run periodically.

If you need more detail about the specific implementation of each method, you are encouraged to go take a look at the code documentation for the `Leds` class. 

### The one where we discuss potential paths of improvement

Other than implement additional animations, I don't see what could be improved since there really isn't a lot of complicated logic. 

---

## Communication Raspberry - Arduino

The strandbeest is composed of an Arduino card, and a Raspberry. The
Arduino holds all the logic, and the Raspberry is the one watching
the camera, observing and searching what it's asked to search. Once
the Raspberry has found the object, it should alert the Arduino that
will take action. Hence, the Raspberry is sending data to the Arduino,
but the Arduino does not need to send anything.

### Implementation

In order to achieve such a thing, we chose to make use of the serial
communication between the two devices, by plugging a USB cable on
the two.

A frame object is defined on both devices: using Python on the RPi
and using C++ on the Arduino. This frame class in called
`OutGoingFrame` in the RPi, and `RPIFrame` on the Arduino. The RPi
builds an `OutGoingFrame` and send it through the serial, using an
instance of the `SerialInterface` class.

> The `SerialInterface` class allows to send `OutGoingFrame` to the
> serial port. The `OutGoingFrame` instance is parsed and transformed
> to bytes. It's then squished between an STX and an ETX and finally,
> wrote to serial.
>
> > The STX and ETX bytes of used to detect start and end of a frame
> > during the parsing.

The Arduino reads constantly the reception buffer of its serial port
using the `Communication<serialModule>` class. When it detects a
frame, it passes it to a parser of type `FrameParser` that parses
the char array into a usable instance of `RPIFrame` holding all the
information sent by the RPi.

### Current problems

For some reason, the communication seems unstable at this point. The
fact is that the Arduino doesn't always receive the same data as the
one sent by the RPi. It's correct __most of the time__, but not
always. That has become a real problem for booleans having big
impacts on the behavior on the robot. This arose with the attribute
`objectDetected`:

> When the strandbeest finds an object, it stops and alert the
> others. We definitely do not want the Arduino to receive the
> information that its Raspberry found an object if it did not!

### Ameliorations

To solve the discussed problem, one could implement some channel
encoding, with data duplication.

### Encountered problems

At the start, we thought that having the communication between the
two using serial communication was a poorly chosen option, and that
because it would occupy the serial dev port on the Arduino, making
the development a bit more painful since it would require to unplug
the cable many times, and that it would prevent developers from
having the serial monitor for debugging.

Our first option, was bluetooth communication. Even if it would
have required a bit more complexity due to the pairing process, it
could have avoided the problem described above. Turns out, that a
non-negligible problem came to our line of sight:

__The bluetooth module is wired on the serial bus.__

In other words, one can't use the serial and the bluetooth
simultaneously. Thus, our initial problem will remain with the
bluetooth module.

> Several tests were made by us in order to work on the serial
> bus at different speeds, but always resulted with interferences.

One could have created a virtual switch in order to allow writing
on the bus in certain time slot for serial and bluetooth, but we
preferred not to do so, and just use the serial between the RPi
and the Arduino, despite the problem described above ...

> Note that we didn't explore the option of Wi-Fi communication,
> but it also seems that it's wired on the serial bus ...
