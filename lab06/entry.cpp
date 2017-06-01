// entry.cpp - implements class Entry,
// including instance counts for PA2

#include "entry.h"
#include <iostream>
using namespace std;

unsigned int Entry::accesses = 0;
unsigned int Entry::instances = 0;

Entry::Entry(unsigned int key, std::string data)
: key(key), data(data) { ++instances; }

Entry::Entry(const Entry &other)
: key(other.key), data(other.data){ ++instances; }

Entry::~Entry() { --instances; }

unsigned int Entry::get_key() const
{ ++accesses; return key; }

std::string Entry::get_data() const
{ return data; }

unsigned int Entry::access_count()
{ return accesses; }

unsigned int Entry::instance_count()
{ return instances; }

void Entry::set_key(unsigned int k)
{ key = k; }

void Entry::set_data(std::string d)
{ data = d; }
    
Entry::operator unsigned int () const
{ return get_key(); }

istream& operator>> (istream& inp, Entry &e) {
    inp >> e.key;
    // get data in two parts to handle white space
    string first_word, rest_of_line;
    inp >> first_word;
    getline(inp, rest_of_line);
    e.data = first_word + rest_of_line;
    return inp;
}

ostream& operator<< (ostream& out, Entry &e) {
    out << e.get_key() << ": " << e.get_data();
    return out;
}
