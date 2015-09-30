// Author: HONG Shibao <shibaohong@outlook.com>

#include "LRUCache.h"
#include "LRUCacheShield.h"
#include <iostream>
#include <string>
#include <thread>
#include <memory>

using namespace std;

class Test
{
public:
    bool query(int k, string *v)
    {
        for(int i=0;i<500000000;++i)
            ;
        *v=to_string(k);
        return true;
    }
};

void thread_run(thinkpoet::LRUCacheShield<int, string>& c, int k)
{
    string val;
    Test test;
    c.ShieldGet(k, &val, test, &Test::query, k);
    cout << this_thread::get_id() << " " << k << " " << val << endl;
}

int main()
{
    cout << "SB" << endl;
    thinkpoet::LRUCacheShield<int, string> cache(100);
    cache.Initialize();
    vector<unique_ptr<thread>> vec;
    int num_thread=8;
    chrono::time_point<chrono::high_resolution_clock> st, ed;
    st=chrono::high_resolution_clock::now();
    for(int i=0;i<num_thread;++i)
    {
        vec.emplace_back(new thread(thread_run, ref(cache), 0));
    }
    for(int i=0;i<num_thread;++i)
    {
        vec[i]->join();
    }
    ed=chrono::high_resolution_clock::now();
    cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(ed-st).count() << endl;
    return 0;
}

