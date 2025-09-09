#include "LamportAuth.hpp"

void LamportAuth::initChain(const std::string& seed, int len)
{
    chain = CryptoUtils::genHashChain(seed,len);
    // setCurrentIndex(chain.size()-1);
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

std::string LamportAuth::getLastVerifiedHash()
{
    return lastVerifiedHash;
}

// int LamportAuth::getCurrentIndex(){
//     return currentIndex;
// }

// void LamportAuth::setCurrentIndex(int c){
//     currentIndex = c;
// }

std::string LamportAuth::getLastHash(){
    return chain.back();
}