#include "CryptoUtils.hpp"

#include <cryptopp/sha.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>
#include <cryptopp/files.h>

std::string cryptoUtils::genHash(const std::string& input)
{
    std::string destination{};
    CryptoPP::SHA256 hash;

    //No try catch assuming valid input since it is mentioned we can even precompute the hash chain

    //String processing pipeline, the input goes through the hash filter to get hashed
    //then gets hex encoded (by default upper case) and further the output gets stored in the sink
    CryptoPP::StringSource ss(input, true,
        new CryptoPP::HashFilter(hash, 
            new CryptoPP::HexEncoder(
                new CryptoPP::StringSink(destination) //false if lowercase output needed
            )));

    return destination;
}

std::vector<std::string> cryptoUtils::genHashChain(const std::string& seed, int len)
{
    std::vector<std::string> chain;
    std::string input{seed};
    
    //Generating n hashes 
    for(int i = 0; i < len; ++i)
    {
        //each hash is hex encoded
        chain.push_back(cryptoUtils::genHash(input));
        input = chain.back();
    }

    return chain;
}