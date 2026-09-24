#include "LocalStorage.h"

#include <fstream>
#include <cstdint>

namespace {
    void WriteUInt32(std::vector<unsigned char>& buffer, std::uint32_t value) {
        buffer.push_back(static_cast<unsigned char>(value & 0xFF));
        buffer.push_back(static_cast<unsigned char>((value >> 8) & 0xFF));
        buffer.push_back(static_cast<unsigned char>((value >> 16) & 0xFF));
        buffer.push_back(static_cast<unsigned char>((value >> 24) & 0xFF));
    }

    std::uint32_t ReadUInt32(const std::vector<unsigned char>& buffer, size_t offset) {
        return static_cast<std::uint32_t>(buffer[offset])
             | (static_cast<std::uint32_t>(buffer[offset + 1]) << 8)
             | (static_cast<std::uint32_t>(buffer[offset + 2]) << 16)
             | (static_cast<std::uint32_t>(buffer[offset + 3]) << 24);
    }
}

void toolkit::LocalStorage::SetCipher(GLOBAL_PTR<ICipher> newCipher) {
    MUTEX_LOCK lock(mutex_data);
    cipher = newCipher ? MOVE(newCipher) : MakeGlobalPtr<NoCipher>();
}

BOOL toolkit::LocalStorage::Has(const STRING& key) const {
    MUTEX_LOCK lock(mutex_data);
    return data.find(key) != data.end();
}

void toolkit::LocalStorage::Remove(const STRING& key) {
    MUTEX_LOCK lock(mutex_data);
    data.erase(key);
}

void toolkit::LocalStorage::Clear() {
    MUTEX_LOCK lock(mutex_data);
    data.clear();
}

STRING toolkit::LocalStorage::GetFullPath() const {
    return dir + "/" + name;
}

BOOL toolkit:: LocalStorage::Save() {
    HASH_MAP<STRING, std::vector<unsigned char>> snapshot;
    GLOBAL_PTR<ICipher> currentCipher;
    {
        MUTEX_LOCK lock(mutex_data);
        snapshot = data;
        currentCipher = cipher;
    }

    std::vector<unsigned char> payload;
    WriteUInt32(payload, static_cast<std::uint32_t>(snapshot.size()));

    for (const auto& [key, value] : snapshot) {
        WriteUInt32(payload, static_cast<std::uint32_t>(key.size()));
        payload.insert(payload.end(), key.begin(), key.end());
        WriteUInt32(payload, static_cast<std::uint32_t>(value.size()));
        payload.insert(payload.end(), value.begin(), value.end());
    }

    if (currentCipher) {
        currentCipher->Encrypt(payload);
    }

    std::ofstream file(GetFullPath(), std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    file.write(reinterpret_cast<const char*>(payload.data()),
               static_cast<std::streamsize>(payload.size()));
    return file.good();
}

BOOL toolkit::LocalStorage::Load() {
    std::ifstream file(GetFullPath(), std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    const std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> payload(static_cast<size_t>(size));
    if (!file.read(reinterpret_cast<char*>(payload.data()), size)) {
        return false;
    }

    GLOBAL_PTR<ICipher> currentCipher;
    {
        MUTEX_LOCK lock(mutex_data);
        currentCipher = cipher;
    }

    if (currentCipher) {
        currentCipher->Decrypt(payload);
    }

    if (payload.size() < 4) {
        return false;
    }

    HASH_MAP<STRING, std::vector<unsigned char>> loaded;
    size_t offset = 0;

    const std::uint32_t count = ReadUInt32(payload, offset);
    offset += 4;

    for (std::uint32_t i = 0; i < count; i++) {
        if (offset + 4 > payload.size()) return false;
        const std::uint32_t keyLen = ReadUInt32(payload, offset);
        offset += 4;

        if (offset + keyLen > payload.size()) return false;
        STRING key(payload.begin() + offset, payload.begin() + offset + keyLen);
        offset += keyLen;

        if (offset + 4 > payload.size()) return false;
        const std::uint32_t valLen = ReadUInt32(payload, offset);
        offset += 4;

        if (offset + valLen > payload.size()) return false;
        std::vector<unsigned char> value(payload.begin() + offset, payload.begin() + offset + valLen);
        offset += valLen;

        loaded[key] = MOVE(value);
    }

    MUTEX_LOCK lock(mutex_data);
    data = MOVE(loaded);
    return true;
}