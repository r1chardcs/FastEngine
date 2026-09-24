#ifndef FASTENGINE_LOCALSTORAGE_H
#define FASTENGINE_LOCALSTORAGE_H

#include <Platform.h>
#include <vector>

#include "ICipher.h"
#include "NoCipher.h"
#include "StorageSerializer.h"

namespace toolkit {
    class LocalStorage {
        STRING dir;
        STRING name;

        HASH_MAP<STRING, std::vector<unsigned char>> data;
        GLOBAL_PTR<ICipher> cipher;

        mutable MUTEX mutex_data;

    public:
        LocalStorage(MOVE_PLEASE STRING dir,
                     MOVE_PLEASE STRING name)
            : dir(MOVE(dir)),
              name(MOVE(name)),
              cipher(MakeGlobalPtr<NoCipher>()) {}

        void SetCipher(GLOBAL_PTR<ICipher> newCipher);

        template <typename T>
        void Set(const STRING& key, const T& value) {
            const auto bytes = StorageSerializer<T>::Serialize(value);
            MUTEX_LOCK lock(mutex_data);
            data[key] = bytes;
        }

        template <typename T>
        T Get(const STRING& key, const T& defaultValue = T()) const {
            MUTEX_LOCK lock(mutex_data);
            const auto it = data.find(key);
            if (it == data.end()) return defaultValue;
            return StorageSerializer<T>::Deserialize(it->second);
        }

        BOOL Has(const STRING& key) const;
        void Remove(const STRING& key);
        void Clear();

        BOOL Save();
        BOOL Load();

        STRING GetFullPath() const;
    };
}
#endif