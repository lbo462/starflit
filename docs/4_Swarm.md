# Swarm

As you have noticed, we actually have multiple strandbeest, and we'll try to 
make them work as a team or ... as a fleet 🚀

This means that they'll have to communicate. Have could we have achieved such 
a thing ? Using the RF24 radio antennas !

## Implementation

We first wrote a `Radio` class that handled the radio communication, so we
could have some debugging through this radio. Later on, we actually used it to
send data to the other strandbeest.

This data is represented as a frame squished between an STX and ETX byte. It
can contain all the data you want, but for now, it contains a single byte, a
boolean indicating if an object was found.

> The strandbeest was made to look for an object. When it finds it, it should
> alert its mates.

In order to build and parse these frame, we wrote the class `FrameParser`. It
also handles other kind of frames, like the ones exchanged from the RPi and 
the Arduino controller.

To explain in simple terms, the `RescueBot` that found the object creates 
a frame using the `FrameParser`, and sends it with the
`Communication<radioModule>` through radio.

Every strandbeest are constantly listening to the radio and will trigger the
`FrameParser` if and only if they received a parsable frame from their
`Communication<radioModule>` instance. Once parsed, the frame tells if an
object was found (or not).

### Debugging

To be able to debug the bugs one might encounter, we left a `rfReceiver` 
script in the `Arduino` folder of our git repository, that is to be upload on
an Arduino card on which RF24 antennas are plugged.

This will print to serial everything that is received on the radio, like a
huge man in the middle.

## How to do better

Well, the radio communication works pretty well and there's no much more to
do, besides adding some data to the frame, if needed to, which might be
required when implementing any form of mapping.

If, for some reason, you're having troubles having a reliable channel between
the strandbeests, you're free to implement some
[channel coding](https://en.wikipedia.org/wiki/Noisy-channel_coding_theorem) 
,but, it was no problem to us. If you do, implement it in the `FrameParser`
class.