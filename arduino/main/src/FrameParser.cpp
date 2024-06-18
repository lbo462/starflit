#include "FrameParser.h"

FrameParser::FrameParser() {}

FrameParser::~FrameParser() {}

RPIFrame FrameParser::parseRPI(char *frame)
{
    RPIFrame rpiFrame;

    rpiFrame.initialized = (bool)frame[0];
    rpiFrame.objectDetected = (bool)frame[1];

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
     * The form `frame[i] << 8 + frame[i+1]` rebuilds an integer
     * from the two bytes `frame[i]` and `frame[i+1]`.
     * The order is important, and we're doing big endian here.
     */
    rpiFrame.xObjectPosition = (signed int)(frame[2] << 8) + (unsigned char)frame[3];
    rpiFrame.yObjectPosition = (signed int)(frame[4] << 8) + (unsigned char)frame[5];

    return rpiFrame;
}

StrandFrame FrameParser::parseStrand(char *frame)
{
    StrandFrame strandFrame;
    strandFrame.objectFound = (bool)frame[0];

    return strandFrame;
}

void FrameParser::buildStrand(char *buf, bool objectFound)
{
    buf[0] = (char)STX;
    buf[1] = objectFound ? (char)0x1 : (char)0x0;
    buf[2] = (char)ETX;
}