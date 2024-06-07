/// @addtogroup Arduino
/// @{
#ifndef frame_parser_h
#define frame_parser_h

/**
 * Expected frame length in reception.
 * If a received does not match this size, you might be missing something.
 * If it's too short, the parser will fill the RPIFrame with misleading values.
 * But IMHO, it should be no problem to have it longer, but that's not an excuse to be lazy!
 * 
 * Also note that this length might not match the number of attributes of the RPIFrame.
 * Indeed, one might want to fill the RPIFrame with values from the parser itself, such as
 * - was the frame correctly read ?
 * - was it full ?
 * - is today a beautiful day ?
 * ... or else
 * 
 * ON TOP ON THE HAT,
 * (int) have a length of 2 bytes, so each int attributes counts for two.
 * This goes for other attributes as well.
 * Use `sizeof()` to know the size of each variable type.
 * 
 * Also note that this variable is not used by the parser, but used by the one using the parser,
 * as a length parameter
 */
#define RECEIVED_FRAME_LENGTH 4

/**
 * Defines the parsed content a received frame.
 * Should match the python side.
 */
struct RPIFrame
{
    int xObjectPosition;
    int yObjectPosition;
};

/**
 * Parses the frame coming from the RPi.
 * This class do not own its instance of Communication<serial> !
 * It only parses a frame given to it, regardless of its provenance.
 * 
 * For now, we don't need to send data to the RPI, but we might one day.
 * If so, one can either :
 * - Implement a frame builder inside this class with an other frame struct ;
 * - Create a new class FrameBuilder, similar as this one.
 */
class FrameParser
{
    public:
        FrameParser();
        ~FrameParser();

        /**
         * Parse a frame and return an instance of RPIFame.
         * Here, we're not demanding a length since the frame should have the expected length,
         * defined by 
         * @param frame An "parsable" array of raw bytes.
         * "parsable" means that it should be filled with the correct data
         * to build a RPIFrame object.
         * @return A fully usable RPIFrame object!
         */
        RPIFrame parse(char *frame);

    private:
};

#endif
/// @}