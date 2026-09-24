#ifndef FASTENGINE_XORCIPHER_H
#define FASTENGINE_XORCIPHER_H

#include "ICipher.h"

namespace toolkit {
    class XorCipher : public ICipher {
        STRING key;

        void Process(std::vector<unsigned char>& data) const {
            if (key.empty()) return;
            for (size_t i = 0; i < data.size(); i++) {
                data[i] ^= static_cast<unsigned char>(key[i % key.size()]);
            }
        }

    public:
        explicit XorCipher(MOVE_PLEASE STRING key) : key(MOVE(key)) {}

        void Encrypt(std::vector<unsigned char>& data) const override { Process(data); }
        void Decrypt(std::vector<unsigned char>& data) const override { Process(data); }
    };
}
#endif