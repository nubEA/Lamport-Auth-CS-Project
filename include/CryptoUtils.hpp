#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <string>
#include <vector>

/**
 * @namespace CryptoUtils
 * @brief A collection of utility functions for cryptographic operations.
 *
 * This namespace provides functions for generating SHA-256 hashes, creating
 * Lamport hash chains, and generating random data, leveraging the Crypto++ library.
 */
namespace CryptoUtils{

    /**
     * @brief Generates a SHA-256 hash of a given input string.
     * @param input The string to be hashed.
     * @return The resulting hash as a hexadecimal string.
     */
    std::string genHash(const std::string& input);

    /**
     * @brief Generates a Lamport hash chain from a seed value.
     * @param seed The initial value (h_0) for the chain.
     * @param len The desired length (n) of the hash chain.
     * @return A vector of strings representing the hash chain [h_1, h_2, ..., h_n].
     */
    std::vector<std::string> genHashChain(const std::string& seed, int len);

    /**
     * @brief Generates a cryptographically secure random seed.
     * @param size The desired size of the seed in bytes.
     * @return The generated seed as a hexadecimal string.
     */
    std::string generateRandomSeed(int size);

    /**
     * @brief Converts a raw byte string into its hexadecimal representation.
     * @param input The raw string to convert.
     * @return The hexadecimal string representation.
     */
    std::string convertToHex(const std::string& input);
}

#endif