// entry.h - defines class Entry for CS 32 PA2

#ifndef ENTRY_H
#define ENTRY_H

#include <string>
#include <iosfwd>

class Entry {

public:
    // constructors and destructor
    Entry(unsigned int key = 0, std::string data = "");
    Entry(const Entry &other);
    ~Entry();
    
    // access and mutator functions
    unsigned int get_key() const;
    std::string get_data() const;
    static unsigned int access_count();
    static unsigned int instance_count();
    void set_key(unsigned int k);
    void set_data(std::string d);
    
    // operator conversion function simplifies comparisons
    operator unsigned int () const;
    
    // input and output friends
    friend std::istream& operator>>
        (std::istream& inp, Entry &e);
    friend std::ostream& operator<<
        (std::ostream& out, Entry &e);
    
private:
    unsigned int key;
    std::string data;
    static unsigned int accesses;
    static unsigned int instances;
};

#endif
