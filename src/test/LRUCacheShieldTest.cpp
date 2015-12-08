// Author: HONG Shibao <shibaohong@outlook.com>

#include <memory>
#include <thread>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "core/LRUCacheShield.h"

using namespace std;

namespace thinkpoet
{

class LRUCacheShieldTest:  public ::testing::Test
{
public:
    LRUCacheShieldTest()
    {
    }

protected:
    void SetUp() override;
    void TearDown() override;
};

void LRUCacheShieldTest::SetUp()
{
}

void LRUCacheShieldTest::TearDown()
{
}

bool get_val(const string prefix, int key, string* val)
{
    this_thread::sleep_for(chrono::milliseconds(100));
    *val = prefix + to_string(key);
    return true;
}

void ThreadFunc1(LRUCacheShield<int, string> &cache, int idx, string* ret)
{
    cache.ShieldGet(idx / 2, ret, &get_val, "prefix_");
}

TEST_F(LRUCacheShieldTest, FuncPtrShieldGet)
{
    LRUCacheShield<int, string> cache;
    cache.Initialize(2);
    chrono::time_point<chrono::high_resolution_clock> st, ed;
    st=chrono::high_resolution_clock::now();
    const int num_threads = 6;
    vector<unique_ptr<thread>> vec;
    vector<string> ret_vals(num_threads);
    for(int i = 0; i < num_threads; ++i)
    {
        vec.emplace_back(new thread(ThreadFunc1, ref(cache), i, &ret_vals[i]));
    }
    for(auto &t: vec)
    {
        t->join();
    }
    ed=chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(ed-st).count() << endl;
    EXPECT_EQ(cache.Capacity(), 2);
    EXPECT_EQ(cache.Size(), 2);
    for(int i = 0; i < num_threads; ++i)
    {
        string val;
        get_val("prefix_", i / 2, &val);
        EXPECT_EQ(ret_vals[i], val);
    }
}

class GetValClass
{
public:
    GetValClass(const string& prefix): _prefix(prefix)
    {
    }

    bool GetVal(int delta, int key, string* val)
    {
        this_thread::sleep_for(chrono::milliseconds(100));
        *val = _prefix + to_string(key + delta);
        return true;
    }

private:
    string _prefix;
};

void ThreadFunc2(LRUCacheShield<int, string> &cache, GetValClass& get_val_obj,
                 int idx, string* ret)
{
    cache.ShieldGet(idx / 2, ret, get_val_obj, &GetValClass::GetVal, 100);
}

TEST_F(LRUCacheShieldTest, MemberFuncPtrShieldGet)
{
    LRUCacheShield<int, string> cache;
    cache.Initialize(2);
    chrono::time_point<chrono::high_resolution_clock> st, ed;
    st=chrono::high_resolution_clock::now();
    const int num_threads = 6;
    vector<unique_ptr<thread>> vec;
    vector<string> ret_vals(num_threads);
    GetValClass get_val_obj("prefix_");
    for(int i = 0; i < num_threads; ++i)
    {
        vec.emplace_back(new thread(ThreadFunc2, ref(cache), ref(get_val_obj),
                                    i, &ret_vals[i]));
    }
    for(auto &t: vec)
    {
        t->join();
    }
    ed=chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(ed-st).count() << endl;
    EXPECT_EQ(cache.Capacity(), 2);
    EXPECT_EQ(cache.Size(), 2);
    for(int i = 0; i < num_threads; ++i)
    {
        string val;
        get_val_obj.GetVal(100, i / 2, &val);
        EXPECT_EQ(ret_vals[i], val);
    }
}

}  // namespace thinkpoet
