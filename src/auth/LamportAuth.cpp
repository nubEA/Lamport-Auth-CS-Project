#include "LamportAuth.hpp"

void LamportAuth::initChain(const std::string& seed, int len)
{
    chain = CryptoUtils::genHashChain(seed,len);
}

std::string LamportAuth::getOTPForChallenge(int c)
{
    return chain[chain.size() - c - 1];
}

void LamportAuth::setLastHash(const std::string& hash)
{
    lastVerifiedHash = hash;
}

bool LamportAuth::verifyOTP(const std::string& response)
{
    bool isCorrect = (CryptoUtils::genHash(response) == lastVerifiedHash);
    if(isCorrect) LamportAuth::setLastHash(response);
    return isCorrect;
}