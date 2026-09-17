#ifndef FASTENGINE_NOCIPHER_H
#define FASTENGINE_NOCIPHER_H

#include "ICipher.h"

class NoCipher : public ICipher {
public:
    void Encrypt(std::vector<unsigned char>& data) const override {}
    void Decrypt(std::vector<unsigned char>& data) const override {}
};

#endif