//
//  table.h
//  Lab05
//
#ifndef table_h
#define table_h

#include<iostream>
#include<vector>
#include"entry.h"

class Table{
public:
    //CONSTRUCTORS
    Table(unsigned int max_entries = 100);
    Table(unsigned int entries, std::istream& input);

    //MANIPULATING MEMBER FUNCTIONS

    //puts an entry into the hashtable
    void put(unsigned int key, std::string data);
    void put(Entry e);

    //true if removes an entry, false otherwise
    bool remove(unsigned int key);


    //CONSTANT MEMBER FUNCTIONS

    std::string get(unsigned int key) const;

    //check to see if the key at tableKey matches passed key
    bool keyMatch(unsigned int key, unsigned int tableKey, unsigned int index) const;

    //FRIEND OPERATOR
    friend std::ostream& operator<< (std::ostream& out, const Table& t);

    //helper functions
    unsigned int hash(unsigned int key) const;
private:

    //data
    std::vector<std::vector<Entry> > hashtable;
    unsigned int CAPACITY;

};

#endif /* table_h */
