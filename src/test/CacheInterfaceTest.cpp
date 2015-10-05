// Author: HONG Shibao <shibaohong@outlook.com>

#include "core/CacheInterface.h"
#include "core/LRUCache.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

namespace thinkpoet
{

template<typename T>
class CacheInterfaceTest: public ::testing::Test
{
public:
    CacheInterfaceTest(): _cache(nullptr)
    {
    }

protected:
    void SetUp() override
    {
        _cache.reset(new T);
    }

    void TearDown() override
    {
    }

    unique_ptr<CacheInterface<string, string>> _cache;
};

TYPED_TEST_CASE_P(CacheInterfaceTest);

TYPED_TEST_P(CacheInterfaceTest, PutGet)
{
    this->_cache->Initialize(3);
    this->_cache->Put("price:[0.0,10.0]", "1,2,3");
    this->_cache->Put("brand:test", "2,3,4");
    string val;
    EXPECT_TRUE(this->_cache->Get("brand:test", &val));
    EXPECT_EQ(val, "2,3,4");
}

TYPED_TEST_P(CacheInterfaceTest, Capacity)
{
    this->_cache->Initialize(3);
    EXPECT_EQ(this->_cache->Capacity(), 3);
    this->_cache->Put("price:[0.0,10.0]", "1,2,3");
    this->_cache->Put("brand:test", "2,3,4");
    EXPECT_EQ(this->_cache->Capacity(), 3);
}

TYPED_TEST_P(CacheInterfaceTest, Size)
{
    this->_cache->Initialize(3);
    EXPECT_EQ(this->_cache->Size(), 0);
    this->_cache->Put("price:[0.0,10.0]", "1,2,3");
    EXPECT_EQ(this->_cache->Size(), 1);
    this->_cache->Put("brand:test", "2,3,4");
    EXPECT_EQ(this->_cache->Size(), 2);
    this->_cache->Put("quantity:[5,10]", "5");
    EXPECT_EQ(this->_cache->Size(), 3);
    this->_cache->Put("color:blue", "1,2");
    EXPECT_EQ(this->_cache->Size(), 3);
}

TYPED_TEST_P(CacheInterfaceTest, LRU)
{
    this->_cache->Initialize(3);
    this->_cache->Put("1", "a");
    this->_cache->Put("2", "b");
    this->_cache->Put("3", "c");
    this->_cache->Put("4", "d");
    EXPECT_EQ(this->_cache->Size(), 3);
    string val;
    EXPECT_FALSE(this->_cache->Get("1", &val));
    EXPECT_TRUE(this->_cache->Get("2", &val));
    this->_cache->Put("5", "e");
    EXPECT_FALSE(this->_cache->Get("3", &val));
    this->_cache->Put("4", "f");
    this->_cache->Put("6", "g");
    EXPECT_FALSE(this->_cache->Get("2", &val));
    EXPECT_TRUE(this->_cache->Get("4", &val));
    EXPECT_EQ(val, "f");
}

REGISTER_TYPED_TEST_CASE_P(CacheInterfaceTest,
                           PutGet,
                           Capacity,
                           Size,
                           LRU);

typedef ::testing::Types<LRUCache<string, string>> CacheImplementions;

// Use --gtest_filter=TestCache/FQEngineTest/0.*
//            to filter and just run LRUCache<string, string>
INSTANTIATE_TYPED_TEST_CASE_P(TestCache, CacheInterfaceTest, CacheImplementions);

}
