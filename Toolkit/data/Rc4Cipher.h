#ifndef FASTENGINE_RC4CIPHER_H
#define FASTENGINE_RC4CIPHER_H

#include "ICipher.h"
#include <algorithm>

class Rc4Cipher : public ICipher {
    STRING key;

    void Process(std::vector<unsigned char>& data) const {
        if (key.empty()) return;

        unsigned char S[256];
        for (int i = 0; i < 256; i++) {
            S[i] = static_cast<unsigned char>(i);
        }

        int j = 0;
        const size_t keyLen = key.size();
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + static_cast<unsigned char>(key[i % keyLen])) & 0xFF;
            std::swap(S[i], S[j]);
        }

        int i = 0;
        j = 0;
        for (auto& byte : data) {
            i = (i + 1) & 0xFF;
            j = (j + S[i]) & 0xFF;
            std::swap(S[i], S[j]);
            const unsigned char k = S[(S[i] + S[j]) & 0xFF];
            byte ^= k;
        }
    }

public:
    explicit Rc4Cipher(MOVE_PLEASE STRING key) : key(MOVE(key)) {}

    void Encrypt(std::vector<unsigned char>& data) const override { Process(data); }
    void Decrypt(std::vector<unsigned char>& data) const override { Process(data); }
};

#endif