#include "FrameParser.h"

FrameParser::FrameParser() {}

FrameParser::~FrameParser() {}

RPIFrame FrameParser::parse(char *frame)
{
    RPIFrame rpiFrame;

    // The form `frame[i] << 8 + frame[i+1]` rebuilds an integer
    // from the two bytes `frame[i]` and `frame[i+1]`.
    // The order is important, and we're doing big endian here.
    rpiFrame.xObjectPosition = (int)(frame[0] << 8) + (int)frame[1];
    rpiFrame.yObjectPosition = (int)(frame[2] << 8) + (int)frame[3];

    return rpiFrame;
}