#include <iostream>
#include <fstream>
#include "ssTable.h"
#include <vector>

using namespace std;

int main() {
    vector<PAIR> vec;

    for(uint64_t i = 0;i < 20; ++i){
        vec.emplace_back(i,to_string(i));
    }

    SSTable s("../level/level0/1-0.sst",vec);

    cout << s.isContain(101) << endl;

    cout << s.get(99) << endl;
    cout << s.get(5) << endl;

}