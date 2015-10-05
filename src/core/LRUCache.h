// Author: HONG Shibao <shibaohong@outlook.com>

#pragma once

#include "CacheInterface.h"
#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>
#include <boost/bimap/list_of.hpp>
#include <mutex>

namespace thinkpoet
{

template<typename KeyType, typename ValueType>
class LRUCache: public CacheInterface<KeyType, ValueType>
{
public:
    typedef typename boost::bimaps::bimap<
    boost::bimaps::unordered_set_of<KeyType>,
    boost::bimaps::list_of<ValueType>
    > BimapLRUCache;

    LRUCache():
        CacheInterface<KeyType, ValueType>(),
        _bimap_lru_cache(nullptr)
    {
    }

    ~LRUCache()
    {
        std::lock_guard<std::mutex> lock(_mutex_cache);
        _bimap_lru_cache->clear();
    }

    size_t Size()
    {
        std::lock_guard<std::mutex> lock(_mutex_cache);
        return _bimap_lru_cache->size();
    }

    bool Initialize(size_t capacity) override
    {
        this->_capacity = capacity;
        std::lock_guard<std::mutex> lock(_mutex_cache);
        _bimap_lru_cache.reset(new BimapLRUCache());
        return true;
    }

    void Clear() override
    {
        std::lock_guard<std::mutex> lock(_mutex_cache);
        _bimap_lru_cache->clear();
    }

    bool Get(const KeyType &key, ValueType *val) override
    {
        std::lock_guard<std::mutex> lock(_mutex_cache);
        auto it = _bimap_lru_cache->left.find(key);
        if(it == _bimap_lru_cache->left.end())
        {
            return false;
        }
        *val = it->second;
        _bimap_lru_cache->right.relocate(
                    _bimap_lru_cache->right.end(),
                    _bimap_lru_cache->project_right(it));
        return true;
    }

    void Put(const KeyType &key, const ValueType &val) override
    {
        std::lock_guard<std::mutex> lock(_mutex_cache);
        auto it = _bimap_lru_cache->left.find(key);
        if(it == _bimap_lru_cache->left.end())
        {
            if(_bimap_lru_cache->size() == this->_capacity)
            {
                _bimap_lru_cache->right.erase(_bimap_lru_cache->right.begin());
            }
            _bimap_lru_cache->insert(typename BimapLRUCache::value_type(key, val));
        }
        else
        {
            _bimap_lru_cache->left.replace_data(it, val);
            _bimap_lru_cache->right.relocate(
                        _bimap_lru_cache->right.end(),
                        _bimap_lru_cache->project_right(it));
        }
    }

private:
    std::unique_ptr<BimapLRUCache> _bimap_lru_cache;
    std::mutex _mutex_cache;
};

}  // namespace thinkpoet


