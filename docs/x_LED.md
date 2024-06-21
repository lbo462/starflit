# LEDs

The invention of the LED (Light Emmiting Diode) was quite a breakthrough in the history of science. LEDs were first developed in 1907 when Henry Joseph Round discovered electroluminescence. However, the first practical visible-spectrum LED was created in 1962 by Nick Holonyak Jr. while working at General Electric Company. The first visible LED was red, and it wasn't until the 1990s that we were able to create white light with LEDs thanks to the invention of the blue LED, courtesy of Shuji Nakamura.

If you haven't already, you should really take the time to watch this video about the inventor of the [blue LED](https://youtu.be/AF8d72mA41M?si=4Z5RsmfHclT4VkNN).

I've written this needlessly long introduction mostly because I find LEDs to be quite an interesting subject, but  also because there really isn't a lot to say about our implementation of the LEDs on the robot 🙃.

Anyway...

## The one where we discuss the goal

The work that has been done for the LEDs for this project was mostly a side project since the robot doesn't depend on them to work. However, they are really useful when it comes to debugging and providing visual feedback about what is going on under the hood. It makes the robots interactive in a way.

That being said, the goal was to create a color code that would reflect the state that the robot was in in any given time.

The code that has been implemented is the following:

 - magenta "loading like" animation while the robot is waiting for the RPi to initialize
 - blue slow blinking when the robot is in an exploring state (cf. **ADD LINK**)
 - red fast blinking when the robot is in a scanning state (cf. **ADD LINK**)
 - green fast blinking when the robot has found the specified object
 - rainbow animation whe the robot receives the information that the specified object has been detected.


## The one where we discuss the implementation

The implementation was quite easy really. One small hurdle was the need to implement the blinking animation without using delays. Why can't we use delays you might ask?
Well, when the `delay()` function is called, the program pauses until the delay is over. This prevents the robot from continuing to explore its environement. Since we want the LEDs to be visual feedback WHILE the robot explores, this isn't a good thing.

> This is quite a popular problem on Arduino Uno boards since the microprocessor only has one core, thus one thread.

That being said, the workaround is quite easy.

Every time the blinking function is called, we can check if the elapsed time between the last blink and the current time is superior to the interval we have set. Here is an example:

```cpp
if (currentMillis - previousLedMillis >= interval)
```

> `currentMillis` is the time since the program has started in ms, `previousMillis` the last time le LEDs blinked in ms and `interval` the desired time in between blinks, in ms as well. 

This will allow the program to continue to run normally until enough time has passed to change the state of the LEDs. This is the same logic that has been implemented in the `update()` method of `RescueBot.cpp` to have the `scan()` method run periodically.

If you need more detail about the specific implementation of each method, you are encouraged to go take a look at the code documentation available [here](). 

## The one where we discuss potential paths of improvement

Other than implement additional animations, I don't see what could be improved since there really isn't a lot of complicated logic. 