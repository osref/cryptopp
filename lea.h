// lea.h - written and placed in the public domain by Kim Sung Hee and Jeffrey Walton
//         Based on "LEA: A 128-Bit Block Cipher for Fast Encryption on Common
//         Processors" by Deukjo Hong, Jung-Keun Lee, Dong-Chan Kim, Daesung Kwon,
//         Kwon Ho Ryu, and Dong-Geon Lee.

/// \file lea.h
/// \brief Classes for the LEA block cipher
/// \since Crypto++ 7.1

#ifndef CRYPTOPP_LEA_H
#define CRYPTOPP_LEA_H

#include "config.h"
#include "seckey.h"
#include "secblock.h"
#include "algparam.h"

#if (CRYPTOPP_BOOL_X64 || CRYPTOPP_BOOL_X32 || CRYPTOPP_BOOL_X86 || CRYPTOPP_BOOL_ARM32 || CRYPTOPP_BOOL_ARM64)
# define CRYPTOPP_LEA_ADVANCED_PROCESS_BLOCKS 1
#endif

// Define this if you want to pre-splat the round key table
// for NEON and Aarch64. Pre-splatting the round key increases
// performance by about 0.7 cpb on ARM server boards like an
// AMD Opteron A1100. However, it crushes performance on ARM
// dev-boards like LeMaker HiKey and Pine64. HiKey and Pine64
// run about 8 cpb slower when pre-splatting the round keys.
// # define CRYPTOPP_LEA_ARM_SPLAT_ROUNDKEYS 1

NAMESPACE_BEGIN(CryptoPP)

/// \brief LEA block cipher information
/// \since Crypto++ 7.1
struct LEA_Info : public FixedBlockSize<16>, VariableKeyLength<16,16,32,8>
{
    static const std::string StaticAlgorithmName()
    {
        // Format is Cipher-Blocksize
        return "LEA-128";
    }
};

/// \brief LEA 128-bit block cipher
/// \details LEA provides 128-bit block size. The valid key size is 128-bits, 192-bits and 256-bits.
/// \note Crypto++ provides a byte oriented implementation
/// \sa <a href="http://www.cryptopp.com/wiki/LEA">LEA</a>, <a href=
///   "https://seed.kisa.or.kr/html/egovframework/iwt/ds/ko/ref/LEA%20A%20128-Bit%20Block%20Cipher%20for%20Fast%20Encryption%20on%20Common%20Processors-English.pdf">
///   LEA: A 128-Bit Block Cipher for Fast Encryption on Common Processors</a>
/// \since Crypto++ 7.1
class CRYPTOPP_NO_VTABLE LEA : public LEA_Info, public BlockCipherDocumentation
{
public:
    /// \brief LEA block cipher transformation functions
    /// \details Provides implementation common to encryption and decryption
    /// \since Crypto++ 7.1
    class CRYPTOPP_NO_VTABLE Base : public BlockCipherImpl<LEA_Info>
    {
    protected:
        void UncheckedSetKey(const byte *userKey, unsigned int keyLength, const NameValuePairs &params);

        SecBlock<word32> m_rkey;
        mutable SecBlock<word32> m_temp;
        unsigned int m_rounds;
    };

    /// \brief Provides implementation for encryption transformation
    /// \details Enc provides implementation for encryption transformation. All key and block
    ///   sizes are supported.
    /// \since Crypto++ 7.1
    class CRYPTOPP_NO_VTABLE Enc : public Base
    {
    public:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;

#if CRYPTOPP_LEA_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    /// \brief Provides implementation for encryption transformation
    /// \details Dec provides implementation for decryption transformation. All key and block
    ///   sizes are supported.
    /// \since Crypto++ 7.1
    class CRYPTOPP_NO_VTABLE Dec : public Base
    {
    public:
        void ProcessAndXorBlock(const byte *inBlock, const byte *xorBlock, byte *outBlock) const;

#if CRYPTOPP_LEA_ADVANCED_PROCESS_BLOCKS
        size_t AdvancedProcessBlocks(const byte *inBlocks, const byte *xorBlocks, byte *outBlocks, size_t length, word32 flags) const;
#endif
    };

    typedef BlockCipherFinal<ENCRYPTION, Enc> Encryption;
    typedef BlockCipherFinal<DECRYPTION, Dec> Decryption;
};

typedef LEA::Encryption LEAEncryption;
typedef LEA::Decryption LEADecryption;

NAMESPACE_END

#endif  // CRYPTOPP_LEA_H