#include<iostream>
#include<algorithm>
#include"table.h"
using namespace std;

//CONSTRUCTORS
Table::Table(unsigned int max_entries){
    std::vector<Entry> basicEntryVector;
    this->hashtable.resize(max_entries, basicEntryVector);
    this->CAPACITY = max_entries;
}

Table::Table(unsigned int entries, std::istream& input){
    std::vector<Entry> basicEntryVector;
    this->hashtable.resize(entries, basicEntryVector);
    this->CAPACITY = entries;
    unsigned int key;
    std::string data, first_word, rest_of_line;
    for(int i = 0; i < entries; i++){
        input >> key;
        input >> first_word;
        getline(input, rest_of_line);
        data = first_word + rest_of_line;
        this->put(key, data);
    }
}

Table::Table(const Table& t){
    this->hashtable = t.hashtable;
    this->CAPACITY = t.CAPACITY;
}

Table::~Table(){
    unsigned int tableKey = 0;

    while(tableKey < this->CAPACITY){
        this->hashtable[tableKey].clear();
        tableKey++;
    }
    this->hashtable.clear();
}

//MANIPULATING MEMBER FUNCTIONS
void Table::put(unsigned int key, std::string data){
    unsigned int tableKey;
    tableKey = hash(key);
    unsigned int index = 0;
    unsigned int size;
    size = this->hashtable[tableKey].size();
    bool addedEntry = false;

    while (index < size){
        if(keyMatch(key, tableKey, index)){
            this->hashtable[tableKey][index].set_data(data);
            this->hashtable[tableKey][index].set_key(key);
            addedEntry = true;
            break;
        }
        index++;
    }
    if(!addedEntry){
        this->hashtable[tableKey].resize(size+1, Entry());
        this->hashtable[tableKey][index].set_data(data);
        this->hashtable[tableKey][index].set_key(key);
    }

}

void Table::put(Entry e){
    unsigned int key = e.get_key();
    std::string data = e.get_data();
    put(key, data);
}

bool Table::remove(unsigned int key){
    unsigned int tableKey = hash(key);
    unsigned int index = 0;
    unsigned int size = this->hashtable[tableKey].size();
    while(index < size){
        if(keyMatch(key, tableKey, index)){
            this->hashtable[tableKey][index].set_data(this->hashtable[tableKey][size-1].get_data());
            this->hashtable[tableKey][index].set_key(this->hashtable[tableKey][size-1].get_key());
            this->hashtable[tableKey].pop_back();
            return true;
            break;
        }
        index++;
    }
    return false;
}

//CONSTANT MEMBER FUNCTIONS
std::string Table::get(unsigned int key) const{
    std::string data = "";
    unsigned int tableKey;
    tableKey = hash(key);
    unsigned int index = 0;
    unsigned int size = this->hashtable[tableKey].size();

    while(index < size){
        if(keyMatch(key, tableKey, index)){
            data = this->hashtable[tableKey][index].get_data();
            break;
        }
        index++;
    }

return data;
}

bool Table::keyMatch(unsigned int key, unsigned int tableKey, unsigned int index) const{
    if(index >= this->hashtable[tableKey].size()){
        return false;
    }
    else{
        return(key == this->hashtable[tableKey][index].get_key());
    }
}

ostream& operator<< (std::ostream& out, const Table& t){
    std::vector<Entry> sortedTable;
    unsigned int tableKey = 0;

    while(tableKey < t.CAPACITY){
        unsigned int innerSize = t.hashtable[tableKey].size();
        unsigned int index = 0;
        while(index < innerSize){
            sortedTable.push_back(t.hashtable[tableKey][index]);
            index++;
        }
        tableKey++;
    }

    std::sort(sortedTable.begin(), sortedTable.end());
    unsigned int totalSize = sortedTable.size();
    for(int i = 0; i < totalSize; i++){
        out << sortedTable[i] << endl;
    }
    return out;
}

Table& Table::operator=(const Table &t){
    if (&t == this){
        return (*this);
    }
    //Set back to base table if data already in table
    if(!this->hashtable.empty()){
        unsigned int outerSize = this->CAPACITY;
        unsigned int tableKey = 0;
        while(tableKey < outerSize){
            this->hashtable[tableKey].clear();
            tableKey++;
        }
        this->hashtable.clear();
    }

    //Set this to t
    this->CAPACITY = t.CAPACITY;
    std::vector<Entry> basicEntryVector;
    this->hashtable.resize(this->CAPACITY,basicEntryVector);
    unsigned int outerSize = t.CAPACITY;
    unsigned int tableKey = 0;
    while(tableKey < outerSize){
        unsigned int innerSize = t.hashtable[tableKey].size();
        unsigned int index = 0;
        while(index < innerSize){
            this->put(t.hashtable[tableKey][index]);
            index++;
        }
        tableKey++;
    }
    
    return (*this);
}

unsigned int Table::hash(unsigned int key) const{
    return (key % CAPACITY);
}
