// Author: HONG Shibao <shibaohong@outlook.com>

#include "core/CacheInterface.h"
#include "core/LRUCache.h"
#include <gtest/gtest.h>
#include <memory>

using namespace std;

namespace thinkpoet {

template<typename T>
class CacheInterfaceTest: public ::testing::Test {
public:
    CacheInterfaceTest(): _cache(nullptr) {
    }

protected:
    void SetUp() override {
        _cache.reset(new T<string, string>);
    }

    void TearDown() override {
    }

    unique_ptr<CacheInterface<string, string>> _cache;
};

TYPED_TEST_CASE_P(CacheInterfaceTest);

TYPED_TEST_P(CacheInterfaceTest, PutGet) {
    _cache->Initialize(3);
    _cache->Put("price:[0.0,10.0]", "1,2,3");
    _cache->Put("brand:test", "2,3,4");
    string val;
    EXPECT_TRUE(_cache->Get("brand:test", &val));
    EXPECT_EQ(val, "2,3,4");
}

TYPED_TEST_P(CacheInterfaceTest, Capacity) {
    _cache->Initialize(3);
    EXPECT_EQ(_cache->Capacity(), 3);
    _cache->Put("price:[0.0,10.0]", "1,2,3");
    _cache->Put("brand:test", "2,3,4");
    EXPECT_EQ(_cache->Capacity(), 3);
}

TYPED_TEST_P(CacheInterfaceTest, Size) {
    _cache->Initialize(3);
    EXPECT_EQ(_cache->Size(), 0);
    _cache->Put("price:[0.0,10.0]", "1,2,3");
    EXPECT_EQ(_cache->Size(), 1);
    _cache->Put("brand:test", "2,3,4");
    EXPECT_EQ(_cache->Size(), 2);
    _cache->Put("quantity:[5,10]", "5");
    EXPECT_EQ(_cache->Size(), 3);
    _cache->Put("color:blue", "1,2");
    EXPECT_EQ(_cache->Size(), 3);
}

REGISTER_TYPED_TEST_CASE_P(CacheInterfaceTest,
                           PutGet,
                           Capacity,
                           Size);
typedef ::testing::Types<LRUCache> CacheImplementions;
// Use --gtest_filter=TestCache/FQEngineTest/0.* to filter and just run LRUCache
INSTANTIATE_TYPED_TEST_CASE_P(TestCache, CacheInterfaceTest, CacheImplementions);

}
