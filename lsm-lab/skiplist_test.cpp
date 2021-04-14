#include "skiplist.h"
#include <iostream>

using namespace std;

int main(){
    SkipList *list = new SkipList;

    cout << "Test Output:1" << endl;
    list->put(1,"first");
    list->put(2,"second");
    list->put(3,"third");
    list->put(4,"fourth");
    list->put(5,"fifth");
    list->put(6,"sixth");
    list->put(7,"seventh");
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:2" << endl;
    cout << list->get(4) << '\n';
    cout << "-----------" << endl;

    cout << "Test Output:3" << endl;
    list->del(4);
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:4" << endl;
    list->put(3,"third2");
    list->put(2,"second2");
    list->put(1,"first1");
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:5" << endl;
    cout << list->get(3) << endl;
    cout << list->get(1) << endl;
    cout << "-----------" << endl;

    cout << "Test Output:6" << endl;
    list->del(1);
    list->del(3);
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:7" << endl;
    list->del(2);
    list->del(2);
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:8" << endl;
    list->del(2);
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:9" << endl;
    list->del(1);
    list->del(3);
    list->del(5);
    list->del(6);
    list->del(7);
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:10" << endl;
    list->put(1,"first");
    list->put(2,"second");
    list->put(3,"third");
    list->put(4,"fourth");
    list->show();
    cout << "-----------" << endl;

    cout << "Test Output:11" << endl;
    list->reset();
    list->show();
}