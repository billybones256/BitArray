/*
 bit_array.h
 project: bit array C library
 url: https://github.com/noporpoise/BitArray/
 Adapted from: http://stackoverflow.com/a/2633584/431087
 author: Isaac Turner <turner.isaac@gmail.com>

 Copyright (c) 2012, Isaac Turner
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef BIT_ARRAY_HEADER_SEEN
#define BIT_ARRAY_HEADER_SEEN

#include <stdio.h>
#include <inttypes.h>

typedef struct BIT_ARRAY BIT_ARRAY;

// 64 bit words
typedef uint64_t word_t, word_addr_t, bit_index_t;
typedef uint8_t word_offset_t; // Offset within a 64 bit word

extern const bit_index_t BIT_INDEX_MIN, BIT_INDEX_MAX;

//
// Basics: Constructor, destructor, get length, resize
//

// Constructor - create a new bit array of length nbits
BIT_ARRAY* bit_array_create(bit_index_t nbits);

// Destructor - free the memory used for a bit array
void bit_array_free(BIT_ARRAY* bitarray);

// Get length of bit array
bit_index_t bit_array_length(const BIT_ARRAY* bit_arr);

// Change the size of a bit array. Enlarging an array will add zeros
// to the end of it. Returns 1 on success, 0 on failure (e.g. not enough memory)
char bit_array_resize(BIT_ARRAY* bitarr, bit_index_t new_num_of_bits);

// If bitarr length < num_bits, resizes to num_bits
char bit_array_ensure_size(BIT_ARRAY* bitarr, bit_index_t ensure_num_of_bits);

// Get underlying data structure
const uint64_t* bit_array_get_data(BIT_ARRAY *bitarr, uint64_t *arrlen);

//
// Get, set, clear, assign and toggle individual bits
//

// Get the value of a bit (returns 0 or 1)
char bit_array_get_bit(const BIT_ARRAY* bitarr, bit_index_t b);

// set a bit (to 1) at position b
void bit_array_set_bit(BIT_ARRAY* bitarr, bit_index_t b);

// clear a bit (to 0) at position b
void bit_array_clear_bit(BIT_ARRAY* bitarr, bit_index_t b);

// If bit is 0 -> 1, if bit is 1 -> 0.  Same as complement.
void bit_array_toggle_bit(BIT_ARRAY* bitarr, bit_index_t b);

// If char c != 0, set bit; otherwise clear bit
void bit_array_assign_bit(BIT_ARRAY* bitarr, bit_index_t b, char c);


//
// Set, clear and toggle several bits at once
//

// Set multiple bits at once. 
// e.g. set bits 1, 20 & 31: bit_array_set_bits(bitarr, 3, 1,20,31);
void bit_array_set_bits(BIT_ARRAY* bitarr, size_t n, ...);

// Clear multiple bits at once.
// e.g. clear bits 1, 20 & 31: bit_array_clear_bits(bitarr, 3, 1,20,31);
void bit_array_clear_bits(BIT_ARRAY* bitarr, size_t n, ...);

// Toggle multiple bits at once
// e.g. toggle bits 1, 20 & 31: bit_array_toggle_bits(bitarr, 3, 1,20,31);
void bit_array_toggle_bits(BIT_ARRAY* bitarr, size_t n, ...);

//
// Set, clear and toggle all bits in a region
//

// Set all the bits in a region
void bit_array_set_region(BIT_ARRAY* bitarr,
                          bit_index_t start, bit_index_t length);

// Clear all the bits in a region
void bit_array_clear_region(BIT_ARRAY* bitarr,
                            bit_index_t start, bit_index_t length);

// Toggle all the bits in a region
void bit_array_toggle_region(BIT_ARRAY* bitarr,
                             bit_index_t start, bit_index_t length);

//
// Set, clear and toggle all bits at once
//

// Set all bits in this array to 1
void bit_array_set_all(BIT_ARRAY* bitarr);

// Set all bits in this array to 0
void bit_array_clear_all(BIT_ARRAY* bitarr);

// Set all 1 bits to 0, and all 0 bits to 1
void bit_array_toggle_all(BIT_ARRAY* bitarr);

//
// Get / set a word of a given size
//

// First bit is in the least significant bit position
// start index must be within the range of the bit array (0 <= x < length)
uint64_t bit_array_get_word64(const BIT_ARRAY* bitarr, bit_index_t start);
uint32_t bit_array_get_word32(const BIT_ARRAY* bitarr, bit_index_t start);
uint16_t bit_array_get_word16(const BIT_ARRAY* bitarr, bit_index_t start);
uint8_t  bit_array_get_word8 (const BIT_ARRAY* bitarr, bit_index_t start);

// Set 64 bits at once from a particular start position
void bit_array_set_word64(BIT_ARRAY* bitarr, bit_index_t start, uint64_t word);
void bit_array_set_word32(BIT_ARRAY* bitarr, bit_index_t start, uint32_t word);
void bit_array_set_word16(BIT_ARRAY* bitarr, bit_index_t start, uint16_t word);
void bit_array_set_word8(BIT_ARRAY* bitarr, bit_index_t start, uint8_t byte);

//
// Number of bits set
//

// Get the number of bits set (hamming weight)
bit_index_t bit_array_num_bits_set(const BIT_ARRAY* bitarr);

// Get the number of bits set in on array and not the other.  This is equivalent
// to hamming weight of the XOR when the two arrays are the same length.
// e.g. 10101 vs 00111 => hamming distance 2 (XOR is 10010)
bit_index_t bit_array_hamming_distance(const BIT_ARRAY* arr1,
                                       const BIT_ARRAY* arr2);

// Get the number of bits not set (length - hamming weight)
bit_index_t bit_array_num_bits_cleared(const BIT_ARRAY* bitarr);

// Find the index of the first bit that is set.  
// Returns 1 if a bit is set, otherwise 0
// Index of first set bit is stored in the integer pointed to by `result`
// If no bit is set result is not changed
char bit_array_find_first_set_bit(const BIT_ARRAY* bitarr, bit_index_t* result);

// Find the index of the last bit that is set.  
// Returns 1 if a bit is set, otherwise 0
// Index of last set bit is stored in the integer pointed to by `result`
// If no bit is set result is not changed
char bit_array_find_last_set_bit(const BIT_ARRAY* bitarr, bit_index_t* result);

// Parity - returns 1 if odd number of bits set, 0 if even
char bit_array_parity(const BIT_ARRAY* bitarr);


//
// Sorting
//

// Put all the 0s before all the 1s
void bit_array_sort_bits(BIT_ARRAY* bitarr);

// Put all the 1s before all the 0s
void bit_array_sort_bits_rev(BIT_ARRAY* bitarr);


//
// String and printing methods
//

// Construct a BIT_ARRAY from a string. 
void bit_array_from_str(BIT_ARRAY* bitarr, const char* bitstr);

// Construct a BIT_ARRAY from a substring with given on and off characters.
void bit_array_from_substr(BIT_ARRAY* bitarr, bit_index_t offset,
                           const char* str, size_t len,
                           const char *on, const char *off, char left_to_right);

// Takes a char array to write to.  `str` must be bitarr->num_of_bits+1 in
// length. Terminates string with '\0'
char* bit_array_to_str(const BIT_ARRAY* bitarr, char* str);

// Get a string representations for a given region, using given on/off
// characters. 
void bit_array_to_substr(const BIT_ARRAY* bitarr,
                         bit_index_t start, bit_index_t length,
                         char* str, char on, char off, char left_to_right);

// Print this array to a file stream.  Prints '0's and '1'.  Doesn't print
// newline.
void bit_array_print(const BIT_ARRAY* bitarr, FILE* fout);

// Print a string representations for a given region, using given on/off
// characters. Reverse prints from highest to lowest -- this is useful for
// printing binary numbers
void bit_array_print_substr(const BIT_ARRAY* bitarr,
                            bit_index_t start, bit_index_t length,
                            FILE* fout, char on, char off, char left_to_right);

// Hexidecimal

// Loads array from hex string
// Returns the number of bits loaded (will be chars rounded up to multiple of 8)
// (0 on failure)
bit_index_t bit_array_from_hex(BIT_ARRAY* bitarr, bit_index_t offset,
                               const char* str, size_t len);

// Returns number of characters written
size_t bit_array_to_hex(const BIT_ARRAY* bitarr,
                        bit_index_t start, bit_index_t length,
                        char* str, char uppercase);

// Print bit array as hex
size_t bit_array_print_hex(const BIT_ARRAY* bitarr,
                           bit_index_t start, bit_index_t length,
                           FILE* fout, char uppercase);

//
// Clone and copy
//

// Copy a BIT_ARRAY struct and the data it holds - returns pointer to new object
BIT_ARRAY* bit_array_clone(const BIT_ARRAY* bitarr);

// Copy bits from one array to another
// Destination and source can be the same bit_array and
// src/dst regions can overlap
void bit_array_copy(BIT_ARRAY* dst, bit_index_t dstindx,
                    const BIT_ARRAY* src, bit_index_t srcindx,
                    bit_index_t length);


//
// Logic operators
//

// BIT_ARRAYs can all be different or the same object
// dest array will be resized if it is too short
// 
void bit_array_and(BIT_ARRAY* dest, const BIT_ARRAY* src1, const BIT_ARRAY* src2);
void bit_array_or (BIT_ARRAY* dest, const BIT_ARRAY* src1, const BIT_ARRAY* src2);
void bit_array_xor(BIT_ARRAY* dest, const BIT_ARRAY* src1, const BIT_ARRAY* src2);
void bit_array_not(BIT_ARRAY* dest, const BIT_ARRAY* src);

// `Flip' the bits in a particular regions -- apply `not`
void bit_array_complement_region(BIT_ARRAY* dst,
                                 bit_index_t start, bit_index_t len);

//
// Comparisons
//

// Note: (bit_array_cmp(a,b) == 0) <=> (bit_array_other_endian_cmp(a,b) == 0)

// comparison functions return:
//   1 iff bitarr1 > bitarr2
//   0 iff bitarr1 == bitarr2
//  -1 iff bitarr1 < bitarr2

// Compare two bit arrays by value stored, with index 0 being the Least
// Significant Bit (LSB). Arrays do not have to be the same length.
// Example: ..0101 (5) > ...0011 (3) [index 0 is LSB at right hand side]
int bit_array_cmp(const BIT_ARRAY* bitarr1, const BIT_ARRAY* bitarr2);

// Compare two bit arrays by value stored, with index 0 being the Most
// Significant Bit (MSB). Arrays do not have to be the same length.
// Example: 10.. > 01.. [index 0 is MSB at left hand side]
int bit_array_other_endian_cmp(const BIT_ARRAY* bitarr1, const BIT_ARRAY* bitarr2);


//
// Shift, interleave, reverse
//

// Shift array left/right.  If fill is zero, filled with 0, otherwise 1
void bit_array_shift_right(BIT_ARRAY* bitarr, bit_index_t shift_dist, char fill);
void bit_array_shift_left (BIT_ARRAY* bitarr, bit_index_t shift_dist, char fill);

// Cyclic shift
void bit_array_cycle_right(BIT_ARRAY* bitarr, bit_index_t dist);
void bit_array_cycle_left(BIT_ARRAY* bitarr, bit_index_t dist);

// Interleave
// dst cannot point to the same bit array as src1 or src2
// src1, src2 may point to the same bit array
// abcd 1234 -> a1b2c3d4
// 0011 0000 -> 00001010
// 1111 0000 -> 10101010
// 0101 1010 -> 01100110
void bit_array_interleave(BIT_ARRAY* dst,
                          const BIT_ARRAY* src1, const BIT_ARRAY* src2);

// Reverse the whole array or part of it
void bit_array_reverse(BIT_ARRAY* bitarr);
void bit_array_reverse_region(BIT_ARRAY* bitarr,
                              bit_index_t start, bit_index_t length);

//
// Arithmetic
//

// Returns 1 on sucess, 0 if value in array is too big
char bit_array_as_num(BIT_ARRAY* bitarr, unsigned long* result);

// 1 iff bitarr > value
// 0 iff bitarr == value
// -1 iff bitarr < value
int bit_array_compare_num(BIT_ARRAY* bitarr, unsigned long value);

// bitarr will be extended if needed
void bit_array_add(BIT_ARRAY* bitarr, unsigned long value);

// If value is greater than bitarr, bitarr is not changed and 0 is returned
// Returns 1 on success, 0 if value > bitarr
char bit_array_subtract(BIT_ARRAY* bitarr, unsigned long value);

// Add `add` to `bitarr` at `pos` -- same as:
//   bitarr + (add << pos)
// where pos can be bigger than the length of the array (bitarr will be resized)
void bit_array_add_word(BIT_ARRAY *bitarr, bit_index_t pos, uint64_t add);

// Add `add` to `bitarr` at `pos`
void bit_array_add_words(BIT_ARRAY *bitarr, bit_index_t pos, BIT_ARRAY *add);

// Multiply by some value
void bit_array_multiply(BIT_ARRAY *bitarr, uint64_t multiplier);

//
// Arithmetic between arrays
//

// dst = src1 + src2
// src1, src2 and dst can all be the same BIT_ARRAY
// If dst is shorter than either of src1, src2, it is enlarged
void bit_array_sum(BIT_ARRAY* dst, const BIT_ARRAY* src1, const BIT_ARRAY* src2);

// dst = src1 - src2
// src1, src2 and dst can all be the same BIT_ARRAY
// If dst is shorter than src1, it will be extended to be as long as src1
// src1 must be greater than or equal to src2 (src1 >= src2)
void bit_array_difference(BIT_ARRAY* dst,
                          const BIT_ARRAY* src1,
                          const BIT_ARRAY* src2);

// dst = src1 * src2
// Pointers cannot all point to the same BIT_ARRAY
void bit_array_product(BIT_ARRAY *dst, BIT_ARRAY *src1, BIT_ARRAY *src2);

//
// Read/Write bit_array to a file
//
// File format is [8 bytes: for number of elements in array][data]
// Number of bytes of data is: (int)((num_of_bits + 7) / 8)
//

// Saves bit array to a file
// returns the number of bytes written
bit_index_t bit_array_save(const BIT_ARRAY* bitarr, FILE* f);

// Reads bit array from a file. bitarr is resized and filled.
// Returns 1 on success, 0 on failure
char bit_array_load(BIT_ARRAY* bitarr, FILE* f);


//
// Hash function
//

// Pass seed as 0 on first call, pass previous hash value if rehashing due
// to a collision
// Using bob jenkins hash lookup3
uint64_t bit_array_hash(const BIT_ARRAY* bitarr, uint64_t seed);

//
// Randomness
//

// Set bits randomly with probability prob : 0 <= prob <= 1
void bit_array_random(BIT_ARRAY* bitarr, float prob);

// Shuffle the bits in an array randomly
void bit_array_shuffle(BIT_ARRAY* bitarr);

// Get the next permutation of an array with a fixed size and given number of
// bits set.  Also known as next lexicographic permutation.
// Given a bit array find the next lexicographic orginisation of the bits
// Number of possible combinations given by (size choose bits_set) i.e. nCk
// 00011 -> 00101 -> 00110 -> 01001 -> 01010 ->
// 01100 -> 10001 -> 10010 -> 10100 -> 11000 -> 00011 (back to start)
void bit_array_next_permutation(BIT_ARRAY* bitarr);

//
// Coming soon
//

// dst = src1 / src2
// void bit_array_divide(BIT_ARRAY *dst, const BIT_ARRAY *div);

// dst = src1 % src2
// void bit_array_mod(BIT_ARRAY *dst, const BIT_ARRAY *mod);

// Get bit array as decimal str (e.g. 0b1101 -> "13")
// char* bit_array_to_decimal(const BIT_ARRAY *bitarr, char *str);

// uint64_t bit_array_crc(const BIT_ARRAY *bitarr, uint64_t crc);

#endif
