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
 */
#define DATA_DUPLICATION_FACTOR 3

/// @}