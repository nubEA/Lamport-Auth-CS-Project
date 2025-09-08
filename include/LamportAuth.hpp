#ifndef LAMPORT_AUTH_HPP
#define LAMPORT_AUTH_HPP

#include "CryptoUtils.hpp"
#include <vector>
#include <string>
class LamportAuth{
    private:
        std::vector<std::string> chain; //For Bob
        std::string lastVerifiedHash;   //For Alice
    public:
        //Functions for Bob's usecase
        void initChain(const std::string& seed, int len);              
        std::string getOTPForChallenge(int c);

        //Functions for Alice's usecase
        bool verifyOTP(const std::string& response);
        void setLastHash(const std::string& hash);
};

#endif