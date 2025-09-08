#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <string>
#include <vector>

namespace CryptoUtils{

        //Generate Hash using SHA256 from the given input and returns the hex-encoded result
        std::string genHash(const std::string& input);
        //Generates Hash chain and returns a vector of string
        std::vector<std::string> genHashChain(const std::string& seed, int len);
}

#endif