/// @addtogroup Arduino
/// @{

/** Start of text */
#define STX 0x02

/** End of text */
#define ETX 0x03

/**
 * How much times the bytes contained in serial frame for
 * the RPi are duplicated. Do not change this without
 * thinking! It would have unexpected impacts.
 * 
 * And for the one with faith, note that this would mainly
 * impact the parsing in the `FrameParser`. The number 3
 * is nice because it's odd, and helps have a consensus
 * among the received bytes. It can't be changed anyhow
 * because this is halfly hardcoded in the `FrameParser`,
 * and there's not much one could do about it.
 * > This is because we check the i, i+1 and i+2 of the
 * > indexes of the frame.
 * If you really have problems with channel stability, you
 * will need to implement more complex logic. Just changing
 * this value won't help you much.
 */
#define DATA_DUPLICATION_FACTOR 3

/// @}