#include <iostream>
#include <vector>
#include "CryptoUtils.hpp"

int main() {
    // Test 1: genHash with known value
    std::string input = "hello";
    std::string hash = cryptoUtils::genHash(input);
    std::cout << "SHA256(\"" << input << "\") = " << hash << std::endl;
    std::cout << "Expected (uppercase): "
              << "2CF24DBA5FB0A30E26E83B2AC5B9E29E1B161E5C1FA7425E73043362938B9824"
              << std::endl;

    // Test 2: genHashChain
    std::vector<std::string> chain = cryptoUtils::genHashChain("seed", 5);
    std::cout << "\nHash chain (len=5) starting from seed:\n";
    for (size_t i = 0; i < chain.size(); ++i) {
        std::cout << "h" << i+1 << " = " << chain[i] << std::endl;
    }

    // Internal consistency check
    bool consistent = true;
    for (size_t i = 1; i < chain.size(); ++i) {
        if (cryptoUtils::genHash(chain[i-1]) != chain[i]) {
            consistent = false;
            break;
        }
    }
    std::cout << "\nChain consistency: " << (consistent ? "OK" : "FAILED") << std::endl;

    return 0;
}
