#include "FrameParser.h"

FrameParser::FrameParser() {}

FrameParser::~FrameParser() {}

RPIFrame FrameParser::parse(char *frame)
{
    RPIFrame rpiFrame;

    // The form `frame[i] << 8 + frame[i+1]` rebuilds an integer
    // from the two bytes `frame[i]` and `frame[i+1]`.
    // The order is important!
    rpiFrame.xObjectPosition = frame[0];
    rpiFrame.yObjectPosition = frame[2];

    return rpiFrame;
}