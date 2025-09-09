#ifndef LAMPORT_AUTH_HPP
#define LAMPORT_AUTH_HPP

#include "CryptoUtils.hpp"
#include <vector>
#include <string>
class LamportAuth{
    private:
    std::string lastVerifiedHash;   //For Alice
    // int currentIndex; 
    public:
    //Functions for Bob's usecase
        std::vector<std::string> chain; //For Bob
        void initChain(const std::string& seed, int len);              
        std::string getOTPForChallenge(int c);
        // void setCurrentIndex(int c);
        // int getCurrentIndex();
        std::string getLastHash();

        //Functions for Alice's usecase
        bool verifyOTP(const std::string& response);
        void setLastHash(const std::string& hash);
        std::string getLastVerifiedHash();
};

#endif