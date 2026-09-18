//
// Created by dlllibstdntc on 17.09.2026.
//

#ifndef FASTENGINE_ICIPHER_H
#define FASTENGINE_ICIPHER_H

#include <Platform.h>

class ICipher {
public:
    virtual ~ICipher() = default;
    virtual void Encrypt(VECTOR<BYTE>& data) const = 0;
    virtual void Decrypt(VECTOR<BYTE>& data) const = 0;
};

#endif //FASTENGINE_ICIPHER_H
