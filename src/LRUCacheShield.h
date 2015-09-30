// Author: HONG Shibao <shibaohong@outlook.com>

#pragma once

#include "LRUCache.h"
#include <mutex>
#include <memory>
#include <unordered_map>

namespace thinkpoet {

template<typename KeyType, typename ValueType>
class LRUCacheShield
{
public:
    LRUCacheShield(size_t capacity):
        _capacity(capacity),
        _lru_cache(nullptr)
    {
    }

    ~LRUCacheShield()
    {
        std::lock_guard<std::mutex> lock(_mutex_map);
        _key_to_mutex_ptr.clear();
    }

    int GetCapacity()
    {
        return _capacity;
    }

    int Size()
    {
        return _lru_cache->Size();
    }

    bool Initialize()
    {
        {
            std::lock_guard<std::mutex> lock(_mutex_map);
            _key_to_mutex_ptr.clear();
        }
        _lru_cache.reset(new LRUCache<KeyType, ValueType>(_capacity));
        return _lru_cache->Initialize();
    }

    void Clear()
    {
        {
            std::lock_guard<std::mutex> lock(_mutex_map);
            _key_to_mutex_ptr.clear();
        }
        _lru_cache->Clear();
    }

    template<typename Obj, typename MemberFuncPtr, typename... Args>
    bool ShieldGet(const KeyType &key, ValueType *val,
                   Obj &obj, MemberFuncPtr get_val, Args&&... args)
    {
        bool is_cached = _lru_cache->Get(key, val);
        if(is_cached)
        {
            return true;
        }
        std::shared_ptr<std::mutex> key_mutex(nullptr);
        {
            std::lock_guard<std::mutex> lock(_mutex_map);
            auto it = _key_to_mutex_ptr.find(key);
            if(it == _key_to_mutex_ptr.end())
            {
                auto emp_pair = _key_to_mutex_ptr.emplace(key, std::make_shared<std::mutex>());
                key_mutex = (emp_pair.first)->second;
            }
            else
            {
                key_mutex = it->second;
            }
        }
        bool success = false;
        {
            std::lock_guard<std::mutex> locker_qry(*key_mutex);
            if(_lru_cache->Get(key, val))
            {
                return true;
            }
            // The last parameter of this member function must be the value result to be returned
            // The return value must be bool type
            success = (obj.*get_val)(std::forward<Args>(args)..., val);
            if(success)
            {
                _lru_cache->Put(key, *val);
            }
        }
        {
            std::lock_guard<std::mutex> lock(_mutex_map);
            _key_to_mutex_ptr.erase(key);
        }
        return success;
    }

private:
    std::unique_ptr<LRUCache<KeyType, ValueType>> _lru_cache;
    std::unordered_map<KeyType, std::shared_ptr<std::mutex>> _key_to_mutex_ptr;
    std::mutex _mutex_map;
    size_t _capacity;
};

}  // namespace thinkpoet


