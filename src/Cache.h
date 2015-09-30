// Author: HONG Shibao <shibaohong@outlook.com>

#pragma once

#include <cstddef>

namespace thinkpoet {

template<typename KeyType, typename ValueType>
class CacheInterface
{
public:
    CacheInterface(size_t capacity): _capacity(capacity)
    {
    }

    virtual ~CacheInterface() {}

    virtual size_t GetCapacity()
    {
        return _capacity;
    }

    virtual bool Initialize() = 0;
    virtual void Clear() = 0;

    virtual bool Get(const KeyType &key, ValueType *val) = 0;
    virtual void Put(const KeyType &key, const ValueType &val) = 0;

protected:
    size_t _capacity;
};

}  // namespace thinkpoet

