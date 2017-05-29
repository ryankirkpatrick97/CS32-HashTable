

#ifndef entry_h
#define entry_h

// entry.h - defines class Entry for CS 32 PA1


#include <string>
#include <iosfwd>

class Entry {
    
public:
    // constructor
    Entry(unsigned int key = 0, std::string data = "");
    
    // access and mutator functions
    unsigned int get_key() const;
    std::string get_data() const;
    static unsigned int access_count();
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
};

#endif /* entry_h */
