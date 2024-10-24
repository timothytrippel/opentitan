// Copyright lowRISC contributors (OpenTitan project).
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0

#ifndef OPENTITAN_SW_DEVICE_SILICON_CREATOR_LIB_BASE_UTIL_H_
#define OPENTITAN_SW_DEVICE_SILICON_CREATOR_LIB_BASE_UTIL_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Rounds up the passed value to get it aligned to the requested number of bits.
 *
 * @param input Value to be rounded up.
 * @param align_bits Number of bits to round the input value to.
 * @return Rounded up value.
 */
uint32_t util_round_up_to(uint32_t input, uint32_t align_bits);

/**
 * Compute the number of 32-bit words required to store a number of bytes.
 *
 * @param bytes Buffer of four bytes that represents the ASN1 header.
 * @return Size (in 32-bit words) required to store the input size in bytes.
 */
uint32_t util_size_to_words(uint32_t bytes);

/**
 * Reverses a buffer of bytes in place.
 *
 * @param[inout] buf Buffer in little endian order.
 * @param num_bytes Number of bytes in the buffer above.
 */
void util_reverse_bytes(void *buf, size_t num_bytes);

/**
 * Fills hexdump of the byte (lowercase).
 *
 * @param byte Byte to convert to a hex string.
 * @param[out] str String buffer (always 2 bytes) to place hex string encoded
 * byte in.
 */
void util_hexdump_byte(uint8_t byte, uint8_t *str);

#ifdef __cplusplus
}
#endif

#endif  // OPENTITAN_SW_DEVICE_SILICON_CREATOR_LIB_BASE_UTIL_H_
