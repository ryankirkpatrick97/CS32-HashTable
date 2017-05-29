//
//  tabledemo.cpp
//  Lab05

// tabledemo.cpp - demonstration program for Table
// cmc, 4/4/2016


#include <stdio.h>


#include <iostream>
#include <fstream>
#include "table.h"

using namespace std;

unsigned int user_get(Table &t);
unsigned int user_remove(Table &t);

int main() {

    cout << "Demonstrate very small table\n";
    Table t(5);
    t.put(7, "seven");
    t.put(9, "nine");
    t.put(17, "Seventeen");
    t.put(4, "four");
    t.put(36, "Thirty-six");
    cout << t;
    cout << "key accesses: " << Entry::access_count() << endl;

    cout << "\nNow demonstrate default size table\n";
    Table t2;
    Entry e;
    unsigned int count = 0;
    cout << "Enter up to 100 entries for a new table.\n"
    << "(enter 0 key and random data to quit)\n";
    do {
        cin >> e;
        if (e.get_key() == 0) break;
        t2.put(e);
        ++count;
    } while(count < 100);
    cout << t2;
    cout << "Try removing some entries (enter 0 to quit)\n";
    while (user_remove(t2) != 0)
        ;
    cout << t2;

    cout << "\nFinally demonstrate larger table\n";
    ifstream input;
    input.open("fips.txt");
    if (!input.good()) {
        cout << "No fips.txt in current directory. Quitting\n";
        return 1;
    }
    Table t3(3142, input);
    cout << "Try getting some entries by FIPS code keys\n"
    << "(enter 0 key to quit)\n";
    while (user_get(t3) != 0)
        ;
    cout << "Print large table to sortedfips.txt?\n";
    char ans;
    cin >> ans;
    if (ans == 'Y' || ans == 'y') {
        unsigned int start = Entry::access_count();
        ofstream out;
        out.open("sortedfips.txt");
        out << t3;
        out.close();
        cout << "done writing to sortedfips.txt ... required "
        << Entry::access_count() - start << " accesses\n";
    }
    return 0;
}

unsigned int user_get(Table &t) {
    unsigned int key;
    cout << "Enter key to get:\n";
    cin >> key;
    if (key != 0) {
        unsigned int start = Entry::access_count();
        cout << "data at key " << key << ": " << t.get(key) << endl;
        cout << "(accesses: " <<
        Entry::access_count() - start << ")\n";
    }
    return key;
}

unsigned int user_remove(Table &t) {
    unsigned int key;
    cout << "Enter key to remove:\n";
    cin >> key;
    if (key != 0) {
        unsigned int start = Entry::access_count();
        if (t.remove(key))
            cout << "removed key: " << key << endl;
        else
            cout << "did not find key: " << key << endl;
        cout << "(accesses: "
        << Entry::access_count() - start << ")\n";
    }
    return key;
}
