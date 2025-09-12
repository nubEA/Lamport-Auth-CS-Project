#include "CryptoUtils.hpp"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>   // For AutoSeededRandomPool

/**
 * @brief Generates a SHA-256 hash of a given string.
 * @param input The string to hash.
 * @return The resulting hash as an uppercase hexadecimal string.
 */
std::string CryptoUtils::genHash(const std::string& input)
{
    std::string destination{};
    CryptoPP::SHA256 hash;

    // A Crypto++ pipeline to hash the input and then hex-encode the result.
    CryptoPP::StringSource ss(input, true,
        new CryptoPP::HashFilter(hash, 
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(destination)
            )));

    return destination;
}

/**
 * @brief Generates a Lamport hash chain of a specified length from a seed.
 * @param seed The initial value (h_0) for the chain.
 * @param len The number of hashes to generate (n).
 * @return A vector of strings containing the hash chain [h_1, h_2, ..., h_n].
 */
std::vector<std::string> CryptoUtils::genHashChain(const std::string& seed, int len)
{
    std::vector<std::string> chain;
    std::string input{seed};
    
    // Iteratively generate n hashes
    for(int i = 0; i < len; ++i)
    {
        // The hash of the previous value becomes the next value in the chain
        chain.push_back(CryptoUtils::genHash(input));
        input = chain.back();
    }

    return chain;
}

/**
 * @brief Generates a cryptographically secure random seed.
 * @param size The desired size of the seed in bytes.
 * @return The generated seed as a hexadecimal string.
 */
std::string CryptoUtils::generateRandomSeed(int size)
{
    // Use the operating system's random number generator
    CryptoPP::AutoSeededRandomPool rng;

    // Generate a block of random bytes
    std::string seed;
    seed.resize(size);
    rng.GenerateBlock(reinterpret_cast<CryptoPP::byte*>(&seed[0]),size);

    // Convert the raw bytes to a hex string for easier handling
    return convertToHex(seed);
}

/**
 * @brief Converts a raw string (byte sequence) to its hexadecimal representation.
 * @param input The raw string.
 * @return The hexadecimal representation of the input string.
 */
std::string CryptoUtils::convertToHex(const std::string& input)
{
    std::string hexOutput;
    
    // Crypto++ pipeline for hex encoding
    CryptoPP::StringSource ss(input, true, 
        new CryptoPP::HexEncoder(
            new CryptoPP::StringSink(hexOutput)
            )
        );
    
        return hexOutput;
}