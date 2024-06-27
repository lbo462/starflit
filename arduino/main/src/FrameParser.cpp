#include "FrameParser.h"

FrameParser::FrameParser() {}

FrameParser::~FrameParser() {}

RPIFrame FrameParser::parseRPI(char *frame)
{
    RPIFrame rpiFrame;
    rpiFrame.isValid = true;  // true while not proven otherwise
    
    radio.sendString("Got you. Parsing this:");
    radio.send(frame, RECEIVED_RPI_FRAME_LENGTH);

    // Decoded frame, without data duplication
    char decodedFrame[RECEIVED_RPI_FRAME_LENGTH / DATA_DUPLICATION_FACTOR];

    // Decode frame, keep only the non-duplicated values
    for(int i = 0; i < RECEIVED_RPI_FRAME_LENGTH; i += DATA_DUPLICATION_FACTOR)
    {
        // Index of the final frame to parse
        int realIndex = i / DATA_DUPLICATION_FACTOR;

        // Check the frame integrity and fix it if possible
        if(frame[i] != frame[i+1] || frame[i] != frame[i+2])
        {
            // Check majority
            if(frame[i] == frame[i+1] || frame[i] == frame[i+2])
                decodedFrame[realIndex] = frame[i];

            else if(frame[i+1] == frame[i+2])
                decodedFrame[realIndex] = frame[i+1];

            // All three values are different ...
            else
                rpiFrame.isValid = false;  // dismiss frame
                return rpiFrame;
        }
        else 
        {
            decodedFrame[realIndex] = frame[i];  // Everything is perfect!
        }
    }

    radio.sendString("Here's the decoded version bro:");
    radio.send(decodedFrame, sizeof(decodedFrame));

    rpiFrame.initialized = (bool)decodedFrame[0];
    rpiFrame.objectDetected = (bool)decodedFrame[1];

    /*
     * Hey, you!
     * It's been a long time since we met and we haven't even talked about how low level languages
     * can hurt your health and mental condition.
     * 
     * Here, we're converting the right hand side of the integer into `unsigned char`,
     * aka `byte` in Arduino.
     * Actually, it's an half integer, hence not an integer.
     * If you change `char` for `int` here, you'll have the surprise to discover that
     * the `(unsigned int)` cast is not static and will modify the content of your variable.
     * Tried with `static_cast<unsigned int>(var)` but same shit happened.
     * 
     * But, as you can see, the left hand side is indeed a signed int.
     * Yeah, we can deal with negative numbers but the sign is only carried by the left hand side.
     * 
     * Do the binary math, you'll understand that it works, and this is the way we're supposed
     * to be doing it in C++. Or at least, that's my conclusion after too much time spent on this.
     * 
     * Hope this helped you understand the type casts in the following code ...
     * Now, enjoy this weird cat:
     * 
     *  ( \
     *   \ \
     *   / /                |\\
     *  / /     .-`````-.   / ^`-.
     *  \ \    /         \_/  {|} `o
     *   \ \  /   .---.   \\ _  ,--'
     *    \ \/   /     \,  \( `^^^
     *     \   \/\      (\  )            Good luck during your Starflit's dive!
     *      \   ) \     ) \ \
     *       ) /__ \__  ) (\ \___
     *       (___)))__))(__))(__)))
     * 
     * Last note,
     * The form `decodedFrame[i] << 8 + decodedFrame[i+1]` rebuilds an integer
     * from the two bytes `decodedFrame[i]` and `decodedFrame[i+1]`.
     * The order is important, and we're doing big endian here.
     */
    rpiFrame.xObjectPosition = (signed int)(decodedFrame[2] << 8) + (unsigned char)decodedFrame[3];
    rpiFrame.yObjectPosition = (signed int)(decodedFrame[4] << 8) + (unsigned char)decodedFrame[5];

    radio.sendString("I have " + String((bool)decodedFrame[1]));

    return rpiFrame;
}

StrandFrame FrameParser::parseStrand(char *frame)
{
    StrandFrame strandFrame;
    strandFrame.objectFound = (bool)frame[0];

    return strandFrame;
}

void FrameParser::buildStrand(char* buf, bool objectFound)
{
    buf[0] = STX;
    buf[1] = objectFound ? (char)0x1 : (char)0x0;
    buf[2] = ETX;
}

int FrameParser::getStrandFrameLen()
{
    // We add 2 bytes, corresponding to the STX and ETX bytes.
    return RECEIVED_STRAND_FRAME_LENGTH + 2;
}