/*! \file md5.h */
/* ////////////////////////////////////////////////////////////////////// */
// MD5.cpp
// Implementation file for MD5 class
/* / This C++ Class implementation of the original RSA Data Security, Inc. */
// MD5 Message-Digest Algorithm is copyright (c) 2002, Gary McNickle.
// All rights reserved.  This software is a derivative of the "RSA Data
//  Security, Inc. MD5 Message-Digest Algorithm"
/* / You may use this software free of any charge, but without any */
// warranty or implied warranty, provided that you follow the terms
// of the original RSA copyright, listed below.
/* / Original RSA Data Security, Inc. Copyright notice */
/* ////////////////////////////////////////////////////////////////////// */
/* / Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All */
// rights reserved.
/* / License to copy and use this software is granted provided that it */
// is identified as the "RSA Data Security, Inc. MD5 Message-Digest
// Algorithm" in all material mentioning or referencing this software
// or this function.
// License is also granted to make and use derivative works provided
// that such works are identified as "derived from the RSA Data
// Security, Inc. MD5 Message-Digest Algorithm" in all material
// mentioning or referencing the derived work.
// RSA Data Security, Inc. makes no representations concerning either
// the merchantability of this software or the suitability of this
// software for any particular purpose. It is provided "as is"
// without express or implied warranty of any kind.
// These notices must be retained in any copies of any part of this
// documentation and/or software.
/* ////////////////////////////////////////////////////////////////////// */
#ifndef MD5_H
#define MD5_H

#include <string>

/** Datatype uint4 */
typedef unsigned int uint4;
/** Datatype uint2 */
typedef unsigned short int uint2;
/** Datatype uchar */
typedef unsigned char uchar;

/** namespace suri */
namespace suri {
/** Converts a completed md5 digest into a char* string. */
std::string PrintMD5(uchar md5Digest[16]);
/** Performs the MD5 algorithm on a char* string */
std::string MD5String(const char* szString);
/** Performs the MD5 algorithm on a file (binar or text) */
std::string MD5File(const char* szFilename);
/** Performs the MD5 algorithm on the first maxBytesToRead bytes of a file */
std::string MD5File(const char* szFilename, int maxBytesToRead);

/** C++ Class implementation of the original RSA Data Security, Inc. */
class md5 {
// Methods
public:
   /** Ctor. */
   md5() {
      Init();
   }
   /** Initializes a new context. */
   void Init();
   /** MD5 block update operation. */
   void Update(uchar* chInput, uint4 nInputLen);
   /** MD5 finalization. */
   void Finalize();
   /** Return the digest */
   uchar* Digest() {
      return m_Digest;
   }

private:
   /** Transforms state based on block. */
   void Transform(uchar* block);
   /** Encodes input (uint4) into output (uchar) */
   void Encode(uchar* dest, uint4* src, uint4 nLength);
   /** Decodes input (uchar) into output (uint4) */
   void Decode(uint4* dest, uchar* src, uint4 nLength);

   /** Rotates x left n bits */
   inline uint4 rotate_left(uint4 x, uint4 n) {
      return ((x << n) | (x >> (32 - n)));
   }
   /** Basic MD5 functions: selection */
   inline uint4 F(uint4 x, uint4 y, uint4 z) {
      return ((x & y) | (~x & z));
   }
   /** Basic MD5 functions: majority */
   inline uint4 G(uint4 x, uint4 y, uint4 z) {
      return ((x & z) | (y & ~z));
   }
   /** Basic MD5 functions: parity */
   inline uint4 H(uint4 x, uint4 y, uint4 z) {
      return (x ^ y ^ z);
   }
   /** Basic MD5 functions */
   inline uint4 I(uint4 x, uint4 y, uint4 z) {
      return (y ^ (x | ~z));
   }
   /** Transformations for rounds 1 */
   inline void FF(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
      a += F(b, c, d) + x + ac;
      a = rotate_left(a, s);
      a += b;
   }
   /** Transformations for rounds 2 */
   inline void GG(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
      a += G(b, c, d) + x + ac;
      a = rotate_left(a, s);
      a += b;
   }
   /** Transformations for rounds 3 */
   inline void HH(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
      a += H(b, c, d) + x + ac;
      a = rotate_left(a, s);
      a += b;
   }
   /** Transformations for rounds 4 */
   inline void II(uint4& a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac) {
      a += I(b, c, d) + x + ac;
      a = rotate_left(a, s);
      a += b;
   }

// Data
private:
   uint4 m_State[4]; /*! state */
   uint4 m_Count[2]; /*! count */
   uchar m_Buffer[64]; /*! buffer */
   uchar m_Digest[16]; /*! digest */
   uchar m_Finalized; /*! finalized */
};
}  // namespace suri

#endif // MD5_H
