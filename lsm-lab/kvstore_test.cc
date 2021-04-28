#include <iostream>
#include <string>
#include "kvstore.h"

using namespace std;

int main(){
    KVStore kv("./data");
    string s = "s";

    for(int i = 0;i < 1024*4;++i){
        kv.put(i,string(i+1,'s'));
    }

    string res = kv.get(2030);
    cout << res << '\n';

    for(int i = 1024; i < 2048; ++i){
        kv.del(i);
    }

    kv.reset();

};