#ifndef LAMPORT_AUTH_HPP
#define LAMPORT_AUTH_HPP

#include "CryptoUtils.hpp"
#include <vector>
#include <string>

/**
 * @class LamportAuth
 * @brief Implements the core logic for the Lamport one-time password scheme.
 *
 * This class provides functionalities for both the client (Bob) and the server (Alice).
 * Bob uses it to generate the hash chain and provide OTPs.
 * Alice uses it to verify the received OTPs against the previously stored hash.
 */
class LamportAuth {
private:
    // --- Server-side (Alice) variable ---
    std::string lastVerifiedHash;   ///< Stores the last successfully verified hash (h_i).

public:
    // --- Client-side (Bob) variables and functions ---
    std::vector<std::string> chain; ///< Stores the generated hash chain [h_1, h_2, ..., h_n].

    /**
     * @brief Initializes the hash chain from a given seed.
     * @param seed The initial secret value (h_0).
     * @param len The length of the chain (n).
     */
    void initChain(const std::string& seed, int len);

    /**
     * @brief Retrieves the correct one-time password (OTP) for a given challenge.
     * @param c The challenge number from the server.
     * @return The corresponding OTP (h_{n-c}).
     */
    std::string getOTPForChallenge(int c);

    /**
     * @brief Gets the last hash in the chain (h_n).
     * @return The final hash value.
     */
    std::string getLastHash();


    // --- Server-side (Alice) functions ---

    /**
     * @brief Verifies a received OTP.
     * It checks if H(response) == lastVerifiedHash.
     * @param response The OTP (h_{i-1}) received from the client.
     * @return True if the OTP is valid, false otherwise.
     */
    bool verifyOTP(const std::string& response);

    /**
     * @brief Sets the last verified hash. Used for initialization (with h_n) and updates.
     * @param hash The hash value to set.
     */
    void setLastHash(const std::string& hash);

    /**
     * @brief Gets the last successfully verified hash.
     * @return The last verified hash value (h_i).
     */
    std::string getLastVerifiedHash();
};

#endif