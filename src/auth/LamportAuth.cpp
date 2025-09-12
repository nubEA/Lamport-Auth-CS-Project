#include "LamportAuth.hpp"

/**
 * @brief Initializes the Lamport scheme by generating a hash chain.
 * @param seed The initial secret seed (h_0).
 * @param len The length of the hash chain (n).
 */
void LamportAuth::initChain(const std::string& seed, int len)
{
    // Generate the entire chain h_1, h_2, ..., h_n from the seed
    chain = CryptoUtils::genHashChain(seed,len);
}

/**
 * @brief Retrieves the one-time password (OTP) for a given challenge number.
 * The chain is stored as [h_1, h_2, ..., h_n].
 * For challenge c, the required OTP is h_{n-c}.
 * @param c The challenge number (1-based index).
 * @return The corresponding OTP string (h_{n-c}).
 */
std::string LamportAuth::getOTPForChallenge(int c)
{
    // Index is calculated as (size - 1) - (c - 1) = size - c.
    // However, the prompt says for challenge c, we send h_{n-c}.
    // Our chain is [h_1, ... h_{n-c}, ... h_n].
    // The index for h_k is k-1. So for h_{n-c}, the index is n-c-1.
    // chain.size() is n. So index is chain.size() - c - 1.
    return chain[chain.size() - c - 1];
}

/**
 * @brief Sets or updates the last successfully verified hash.
 * @param hash The hash value (h_i) to store.
 */
void LamportAuth::setLastHash(const std::string& hash)
{
    lastVerifiedHash = hash;
}

/**
 * @brief Verifies a received one-time password (OTP).
 * Checks if H(response) == lastVerifiedHash.
 * @param response The received OTP (h_{i-1}).
 * @return True if verification is successful, false otherwise.
 */
bool LamportAuth::verifyOTP(const std::string& response)
{
    // The received response should be h_{i-1}. Hashing it should yield h_i.
    bool isCorrect = (CryptoUtils::genHash(response) == lastVerifiedHash);
    // If correct, update the last verified hash to the new, lower-index hash
    if(isCorrect) LamportAuth::setLastHash(response);
    return isCorrect;
}

/**
 * @brief Gets the last successfully verified hash (h_i).
 * @return The last verified hash string.
 */
std::string LamportAuth::getLastVerifiedHash()
{
    return lastVerifiedHash;
}

/**
 * @brief Gets the last hash in the chain (h_n).
 * @return The final hash string.
 */
std::string LamportAuth::getLastHash(){
    return chain.back();
}