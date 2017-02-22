
#pragma once

#ifndef _INTS_KEY_H
#define _INTT_KEY_H

#include <endian.h>

/*
 * class IntsKey - Compact representation of integers of different length
 * 
 * This class is used for storing multiple integral fields into a compact
 * array representation. This class is largely used as a static object, 
 * because special storage format is used to ensure a fast comparison 
 * implementation.
 *
 * Integers are stored in a big-endian and sign-magnitute format. Big-endian
 * favors comparison since we could always start comparison using the first few
 * bytes. This gives the compiler opportunities to optimize comparison
 * using advanced techniques such as SIMD or loop unrolling. 
 *
 * For details of how and why integers must be stored in a big-endian and 
 * sign-magnitude format, please refer to adaptive radix tree's key format
 *
 * Note: IntsKey is always word-aligned on x86-64. KeySize is the number of 
 * 64 bit words inside the key, not byte size
 */
template <size_t KeySize>
class IntsKey {
 private:
  // This is the actual byte size of the key
  static constexpr size_t key_size_byte = KeySize * 8UL;
  
  // This is the array we use for storing integers
  unsigned char key_data[key_size_byte];
 
 private:
  
  /*
   * TwoBytesToBigEndian() - Change 2 bytes to big endian
   *
   * This function could be achieved using XCHG instruction; so do not write
   * your own
   *
   * i.e. MOV AX, WORD PTR [data]
   *      XCHG AH, AL
   */
  inline static uint16_t TwoBytesToBigEndian(uint16_t data) {
    return htobe16(data);  
  }
  
  /*
   * FourBytesToBigEndian() - Change 4 bytes to big endian format
   *
   * This function uses BSWAP instruction in one atomic step; do not write
   * your own
   *
   * i.e. MOV EAX, WORD PTR [data]
   *      BSWAP EAX
   */
  inline static uint32_t FourBytesToBigEndian(uint32_t data) {
    return htobe32(data); 
  }
  
  /*
   * EightBytesToBigEndian() - Change 8 bytes to big endian format
   *
   * This function uses BSWAP instruction
   */
  inline static uint64_t EightBytesToBigEndian(uint64_t data) {
    return htobe64(data);
  }  
  
  /*
   * TwoBytesToHostEndian() - Converts back two byte integer to host byte order
   */
  inline static uint16_t TwoBytesToHostEndian(uint16_t data) {
    return be16toh(data); 
  }
  
  /*
   * FourBytesToHostEndian() - Converts back four byte integer to host byte order
   */
  inline static uint32_t FourBytesToHostEndian(uint32_t data) {
    return be32toh(data); 
  }
  
  /*
   * EightBytesToHostEndian() - Converts back eight byte integer to host byte order
   */
  inline static uint64_t EightBytesToHostEndian(uint64_t data) {
    return be64toh(data); 
  }
  
  /*
   * ToBigEndian() - Overloaded version for all kinds of integral data types
   */
  
  inline static uint8_t ToBigEndian(uint8_t data) {
    return data;
  }
  
  inline static uint8_t ToBigEndian(int8_t data) {
    return static_cast<uint8_t>(data);
  }
  
  inline static uint16_t ToBigEndian(uint16_t data) {
    return TwoBytesToBigEndian(data);
  }
  
  inline static uint16_t ToBigEndian(int16_t data) {
    return TwoBytesToBigEndian(static_cast<uint16_t>(data));
  }
  
  inline static uint32_t ToBigEndian(uint32_t data) {
    return FourBytesToBigEndian(data);
  }
  
  inline static uint32_t ToBigEndian(int32_t data) {
    return FourBytesToBigEndian(static_cast<uint32_t>(data));
  }
  
  inline static uint64_t ToBigEndian(uint64_t data) {
    return EightBytesToBigEndian(data);
  }
  
  inline static uint64_t ToBigEndian(int64_t data) {
    return EightBytesToBigEndian(static_cast<uint64_t>(data));
  }
  
  /*
   * ToHostEndian() - Converts big endian data to host format
   */
  
  static inline uint8_t ToHostEndian(uint8_t data) {
    return data; 
  }
  
  static inline uint8_t ToHostEndian(int8_t data) {
    return static_cast<uint8_t>(data); 
  }
  
  static inline uint16_t ToHostEndian(uint16_t data) {
    return TwoBytesToHostEndian(data);
  }
  
  static inline uint16_t ToHostEndian(int16_t data) {
    return TwoBytesToHostEndian(static_cast<uint16_t>(data));
  }
  
  static inline uint32_t ToHostEndian(uint32_t data) {
    return FourBytesToHostEndian(data);
  }
  
  static inline uint32_t ToHostEndian(int32_t data) {
    return FourBytesToHostEndian(static_cast<uint32_t>(data));
  }
  
  static inline uint64_t ToHostEndian(uint64_t data) {
    return EightBytesToHostEndian(data);
  }
  
  static inline uint64_t ToHostEndian(int64_t data) {
    return EightBytesToHostEndian(static_cast<uint64_t>(data));
  }
  
  /*
   * SignFlip() - Flips the highest bit of a given integral type
   *
   * This flip is logical, i.e. it happens on the logical highest bit of an 
   * integer. The actual position on the address space is related to endianess
   * Therefore this should happen first.
   *
   * It does not matter whether IntType is signed or unsigned because we do
   * not use the sign bit
   */
  template <typename IntType>
  inline static IntType SignFlip(IntType data) {
    // This sets 1 on the MSB of the corresponding type
    // NOTE: Must cast 0x1 to the correct type first
    // otherwise, 0x1 is treated as the signed int type, and after leftshifting
    // if it is extended to larger type then sign extension will be used
    IntType mask = static_cast<IntType>(0x1) << (sizeof(IntType) * 8UL - 1);
    
    return data ^ mask;
  }
  
 public:
  
  /*
   * Constructor
   */
  IntsKey() {
    ZeroOut();
    
    return;
  }
  
  /*
   * ZeroOut() - Sets all bits to zero
   */
  inline void ZeroOut() {
    memset(key_data, 0x00, key_size_byte);
    
    return;
  }
  
  /*
   * AddInteger() - Adds a new integer into the compact form
   *
   * Note that IntType must be of the following 8 types:
   *   int8_t; int16_t; int32_t; int64_t
   * Otherwise the result is undefined
   */
  template <typename IntType>
  inline void AddInteger(IntType data, size_t offset) {
    IntType sign_flipped = SignFlip<IntType>(data);
    
    // This function always returns the unsigned type
    // so we must use automatic type inference
    auto big_endian = ToBigEndian(sign_flipped);
    
    // This will almost always be optimized into single move
    memcpy(key_data + offset, &big_endian, sizeof(IntType));
    
    return;
  }
  
  /*
   * AddUnsignedInteger() - Adds an unsigned integer of a certain type
   *
   * Only the following unsigned type should be used:
   *   uint8_t; uint16_t; uint32_t; uint64_t
   */
  template <typename IntType>
  inline void AddUnsignedInteger(IntType data, size_t offset) {
    // This function always returns the unsigned type
    // so we must use automatic type inference
    auto big_endian = ToBigEndian(data);
    
    // This will almost always be optimized into single move
    memcpy(key_data + offset, &big_endian, sizeof(IntType));
    
    return;
  }
  
  /*
   * GetInteger() - Extracts an integer from the given offset
   *
   * This function has the same limitation as stated for AddInteger()
   */
  template <typename IntType>
  inline IntType GetInteger(size_t offset) {
    const IntType *ptr = reinterpret_cast<IntType *>(key_data + offset);
    
    // This always returns an unsigned number
    auto host_endian = ToHostEndian(*ptr);
    
    return SignFlip<IntType>(static_cast<IntType>(host_endian));
  }
  
  /*
   * GetUnsignedInteger() - Extracts an unsigned integer from the given offset
   *
   * The same constraint about IntType applies
   */
  template <typename IntType>
  inline IntType GetUnsignedInteger(size_t offset) {
    const IntType *ptr = reinterpret_cast<IntType *>(key_data + offset);
    auto host_endian = ToHostEndian(*ptr);
    return static_cast<IntType>(host_endian);
  }
  
  /*
   * Compare() - Compares two IntsType object of the same length
   *
   * This function has the same semantics as memcmp(). Negative result means 
   * less than, positive result means greater than, and 0 means equal
   */
  static inline int Compare(const IntsKey<KeySize> &a, 
                            const IntsKey<KeySize> &b) {
    return memcmp(a.key_data, b.key_data, IntsKey<KeySize>::key_size_byte); 
  }
  
  /*
   * LessThan() - Returns true if first is less than the second
   */
  static inline bool LessThan(const IntsKey<KeySize> &a, 
                              const IntsKey<KeySize> &b) {
    return Compare(a, b) < 0;
  }
  
  /*
   * Equals() - Returns true if first is equivalent to the second
   */
  static inline bool Equals(const IntsKey<KeySize> &a, 
                            const IntsKey<KeySize> &b) {
    return Compare(a, b) == 0;
  }
  
 public:
  
  /*
   * PrintRawData() - Prints the content of this key
   *
   * All contents are printed to stderr
   */
  void PrintRawData() {
    // This is the current offset we are on printing the key
    uint64_t offset = 0;
    
    fprintf(stderr, "IntsKey<%lu> - %lu bytes\n", KeySize, key_size_byte);
    
    while(offset < key_size_byte) {
      constexpr int byte_per_line = 16;
      
      fprintf(stderr, "0x%08lX    ", offset);
      
      for(int i = 0;i < byte_per_line;i++) {
        if(offset >= key_size_byte) {
          break;
        } 
        
        fprintf(stderr, "%.2X ", key_data[offset]);
        
        // Add a delimiter on the 8th byte
        if(i == 7) {
          fprintf(stderr, "   "); 
        }
        
        offset++; 
      }
      
      fprintf(stderr, "\n");
    }
    
    return;
  }
};

#endif 
