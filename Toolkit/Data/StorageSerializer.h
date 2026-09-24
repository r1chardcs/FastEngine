#ifndef FASTENGINE_STORAGESERIALIZER_H
#define FASTENGINE_STORAGESERIALIZER_H

#include <Platform.h>
#include <vector>
#include <cstring>
#include <type_traits>

namespace toolkit {
    template <typename T>
    struct StorageSerializer {
        static std::vector<unsigned char> Serialize(const T& value) {
            static_assert(std::is_trivially_copyable_v<T>,
                          "T must be trivially copyable or have a StorageSerializer specialization");
            std::vector<unsigned char> bytes(sizeof(T));
            std::memcpy(bytes.data(), &value, sizeof(T));
            return bytes;
        }

        static T Deserialize(const std::vector<unsigned char>& bytes) {
            static_assert(std::is_trivially_copyable_v<T>,
                          "T must be trivially copyable or have a StorageSerializer specialization");
            T value{};
            std::memcpy(&value, bytes.data(), std::min(sizeof(T), bytes.size()));
            return value;
        }
    };

    template <>
    struct StorageSerializer<STRING> {
        static std::vector<unsigned char> Serialize(const STRING& value) {
            return std::vector<unsigned char>(value.begin(), value.end());
        }

        static STRING Deserialize(const std::vector<unsigned char>& bytes) {
            return STRING(bytes.begin(), bytes.end());
        }
    };
}
#endif