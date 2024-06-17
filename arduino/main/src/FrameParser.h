/// @addtogroup Arduino
/// @{
#ifndef frame_parser_h
#define frame_parser_h

/**
 * Expected frame length in reception from the RPI.
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
 * `int` have a length of 2 bytes, so each int attributes counts for two.
 * This goes for other attributes as well.
 * Use `sizeof()` to know the size of each variable type.
 * 
 * Also note that this variable is not used by the parser, but used by the one using the parser,
 * as a length parameter
 */
#define RECEIVED_RPI_FRAME_LENGTH 4

/**
 * Length of a frame received from an other strandbeest.
 * For more details about frame lengths, check `RECEIVED_RPI_FRAME_LENGTH`.
 */
#define RECEIVED_STRAND_FRAME_LENGTH 1

/**
 * Defines the parsed content a frame from received the RPI shell.
 * Should match the python side.
 */
struct RPIFrame
{
    /** Is the RPI initialized ? */
    bool initialized;

    /** Was an object detected ? */
    bool objectDetected;

    /** Position of the detected object. */
    signed int xObjectPosition;
    signed int yObjectPosition;
};

/**
 * Defines the parsed content of a frame received from an other strandbeest.
 */
struct StrandFrame
{
    /** Was the searched object found ? */
    bool objectFound;
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
         * Parse a frame received from the RPI and return an instance of RPIFame.
         * Here, we're not demanding a length since the frame should have the expected length
         * defined by `RECEIVED_RPI_FRAME_LENGTH`.
         * @param frame An "parsable" array of raw bytes.
         * "parsable" means that it should be filled with the correct data
         * to build a RPIFrame object.
         * @return A fully usable RPIFrame object!
         */
        RPIFrame parseRPI(char *frame);

        /**
         * Parse a frame received from an other strandbeest and return an instance of StrandFrame.
         * Here, we're not demanding a length since the frame should have the expected length
         * defined by `RECEIVED_STRAND_FRAME_LENGTH`.
         * @param frame An "parsable" array of raw bytes.
         * "parsable" means that it should be filled with the correct data
         * to build a StrandFrame object.
         * @return A fully usable StrandFrame object!
         */
        StrandFrame parseStrand(char *frame);
};

#endif
/// @}