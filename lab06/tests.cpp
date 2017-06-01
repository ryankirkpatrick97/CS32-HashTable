// tabletests.cpp - tests for correctness and big-O
// requirements of table functions.
// cmc, 4/15/2016

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "table.h"
using namespace std;

const unsigned int SIZESMALL = 128, SIZELARGE = 2048,
    ITERS = 4;
const double TOO_SLOW_FOR_O1 = 1.5,
    TOO_SLOW_FOR_NLOGN = 1000.0, TOO_FAST = 60.0;

const unsigned int SMSAMP[] = {
    48099,46009,37027,1037};

const unsigned int LGSAMP[] = {
    45065,26121, 18183, 37105};

class NullBuffer : public std::streambuf {
public:
    int overflow(int c) { return c; }
};

bool basic_correct(ifstream &, ostream &);
bool remove_correct(ifstream &, ostream &);
bool output_correct(ifstream &, ostream &);
void rewind_input(ifstream &);
void time_tests(unsigned int, ifstream &);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "usage: " << argv[0] << " test#\n";
        return 1;
    }

    ifstream input;
    input.open("fips.txt");
    if (!input.good()) {
        cout << "No fips.txt in current directory. Quitting\n";
        return 2;
    }

    int t = atoi(argv[1]);
    switch (t) {
        case 1: basic_correct(input, cout); break;
        case 2: remove_correct(input, cout); break;
        case 3: output_correct(input, cout); break;
        case 4: case 5: case 6:
            time_tests(t-3, input); break;
        default:
            cout << "test# can be 1 to 6 only\n";
            return 3;
    }
    return 0;
}

bool basic_correct(ifstream &input, ostream &out) {
    out << "\nTesting constructors, put and get.\n\n";

    out << "Construct an empty table.\n";
    Table t1;

    out << "  Put some key+data pairs in the table.\n";
    const unsigned int keys[] = {67453294, 13413, 12351235, 3463246, 457, 234,141, 64631 };
    const string data[] = {"apple", "as", "dfs", "fshfd", "aser", "ffsdfh", "asdf", "hsd" };
    unsigned int i;
    for (i=0; i < 7; i++)
        t1.put(keys[i], data[i]);

    out << "  Verify get(key) returns correct data.\n";
    for (i=0; i < 7; i++)
        if (t1.get(keys[i]) != data[i]) {
            out << "ERROR: mismatched data for key " << keys[i]
                << "\nTest failed.\n";
            return false;
        }
    string s = t1.get(7);
    if (s != string()) {
        out << "ERROR: get returned \"" << s
        << "\" for missing key.\n";
    }
    out << "    Okay.\n";

    out << "  Now put some Entry objects in the table.\n";
    t1.put(Entry(134513,"hairy ape"));
    t1.put(Entry(3213, "iguana"));
    t1.put(Entry(1658142, "jack rabbit"));
    out << "  Verify get(key) returns correct data again.\n";
    if (t1.get(1658142) != string("jack rabbit")) {
        out << "ERROR: mismatched data for key " << 1658142
        << "\nTest failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "  Now update data for existing entries with put.\n";
    t1.put(1245, "shaved ape");
    if (t1.get(1245) != string("shaved ape")) {
        out << "ERROR: put(key, data) did not update entry.\n";
        out << "Test failed.\n";
        return false;
    }
    t1.put(Entry(666, "mamba"));
    if (t1.get(666) != string("mamba")) {
        out << "ERROR: put(Entry) did not update entry.\n";
        out << "Test failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "Construct large table from input stream.\n";
    Table t2(3144, input);
    out << "  Verify successful get for selected entries.\n";
    if (t2.get(6083) != string("Santa Barbara, California")
        || t2.get(100001) != string("Abbott")
        || t2.get(100002) != string("Avocado")) {
        out << "ERROR: mismatched data for one or more keys.\n";
        out << "Test failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "All tests passed.\n";
    return true;
}

bool remove_correct(ifstream &input, ostream &out) {
    out << "\nTesting remove.\n\n";

    out << "Construct table from input stream.\n";
    Table t(100, input);

    out << "  Verify returns false for non-existing key.\n";
    if (t.remove(7)) {
        out << "ERROR: returned true for missing key.\n";
        out << "Test failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "  Verify true for selected entries.\n";
    if (!t.remove(48099) || !t.remove(17139)
        || !t.remove(31179) || !t.remove(19177)) {
        out << "ERROR: returned false for existing key.\n";
        out << "Test failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "  Now verify false for removed entries.\n";
    if (t.remove(48099) || t.remove(48099)
        || t.remove(31179) || t.remove(31179)) {
        out << "ERROR: true for remove same key twice.\n";
        out << "Test failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "  Use get to verify removed entries gone.\n";
    string blank;
    if (t.get(48099) != blank || t.get(17139) != blank) {
        out << "ERROR: get finds removed entries.\n";
        out << "Test failed.\n";
        return false;
    }
    out << "    Okay.\n";

    out << "All tests passed.\n";
    return true;
}

bool output_correct(ifstream &input, ostream &out) {
    out << "\nTesting sorted output.\n\n";

    const unsigned int SIZE = 1000;
    out << "Construct table from input stream.\n";
    Table t(SIZE, input);

    // rewind and read entries into vector, and sort the vector
    rewind_input(input);
    vector<Entry> v;
    Entry e;
    for (unsigned int i = 0; i < SIZE; i++) {
        input >> e;
        v.push_back(e);
    }
    sort(v.begin(), v.end());

    out << "   Output table to stream.\n";
    stringstream results;
    results << t;

    out << "   Compare output to correct results.\n";
    string correct, outline;
    for (Entry e : v) {
        correct = to_string(e.get_key()) + ": " + e.get_data();
        getline(results, outline);
        if (outline != correct) {
            out << "ERROR: mismatch at \"" << outline << "\"\n";
            return false;
        }
    }
    out << "Test passed.\n";
    return true;
}

void time_tests(unsigned int test_num, ifstream &input) {

    void get_time(Table &, Table &);
    void remove_time(Table &, Table &);
    void output_time(Table &, Table &);

    NullBuffer null_buffer;
    std::ostream null_stream(&null_buffer);

    switch (test_num) {
    case 1:
        if (basic_correct(input, null_stream)) {
            rewind_input(input);
            Table small(SIZESMALL, input), large(SIZELARGE, input);
            get_time(small, large);
        }
        else
            cout << "no get time test: incorrect basic tests.\n";
        break;

    case 2:
        if (remove_correct(input, null_stream)) {
            rewind_input(input);
            Table small(SIZESMALL, input), large(SIZELARGE, input);
            remove_time(small, large);
        }
        else
            cout << "no remove time test: incorrect remove.\n";
        break;

    case 3:
        if (output_correct(input, null_stream)) {
            rewind_input(input);
            Table small(SIZESMALL, input), large(SIZELARGE, input);
            output_time(small, large);
        }
        else
            cout << "no output time test: incorrect output.\n";
        break;
    }
}

void rewind_input(ifstream &input) {
    if (!input.good())
        input.clear();
    input.seekg(0);
}

void get_time(Table &small, Table &large) {
    cout << "Testing speed of function get.\n";

    unsigned int start = Entry::access_count();
    for (unsigned int i=0; i < ITERS; i++)
        small.get(SMSAMP[i]);
    unsigned int finish_small = Entry::access_count();
    unsigned int accesses_small = finish_small - start;

    for (unsigned int i=0; i < ITERS; i++)
        large.get(LGSAMP[i]);
    unsigned int accesses_large =
        Entry::access_count() - finish_small;

    double ratio = 0.0;
    if (accesses_small > 0)
        ratio = static_cast<double>(accesses_large)
            / accesses_small;

    if (accesses_small < ITERS || accesses_large < ITERS)
        cout << "Did not get Entry objects.\n"
             << "Test failed.\n";
    else if (ratio > TOO_SLOW_FOR_O1)
        cout << "Too many Entry accesses for O(1).\n"
             << "Test failed.\n";
    else
        cout << "Test passed.\n";
}

void remove_time(Table &small, Table &large) {
    cout << "Test speed of function remove.\n";

    unsigned int start = Entry::access_count();
    for (unsigned int i=0; i < ITERS; i++){
        if (!small.remove(SMSAMP[i])) {
            cout << "Returned false for item in table.\n";
            cout << "Test failed.\n";
            exit(4);
        }
    }

    unsigned int finish_small = Entry::access_count();
    unsigned int accesses_small = finish_small - start;

    for (unsigned int i=0; i < ITERS; i++)
        if (!large.remove(LGSAMP[i])) {
            cout << "Returned false for item in table.\n";
            cout << "Test failed.\n";
            exit(4);
        }
    unsigned int accesses_large =
    Entry::access_count() - finish_small;

    double ratio = 0.0;
    if (accesses_small > 0)
        ratio = static_cast<double>(accesses_large)
        / accesses_small;

    if (accesses_small < ITERS || accesses_large < ITERS)
        cout << "Did not remove Entry objects.\n"
        << "Test failed.\n";
    else if (ratio > TOO_SLOW_FOR_O1)
        cout << "Too many Entry accesses for O(1).\n"
        << "Test failed.\n";
    else
        cout << "Test passed.\n";
}

void output_time(Table &small, Table &large) {
    cout << "Test speed of sorted output function.\n";

    NullBuffer null_buffer;
    std::ostream null_stream(&null_buffer);

    unsigned int start = Entry::access_count();
    null_stream << small;
    unsigned int finish_small = Entry::access_count();
    unsigned int accesses_small = finish_small - start;

    null_stream << large;
    unsigned int accesses_large =
    Entry::access_count() - finish_small;

    double ratio = 0.0;
    if (accesses_small > 0)
        ratio = static_cast<double>(accesses_large)
        / accesses_small;

    if (ratio < TOO_FAST)
        cout << "Did not sort Entry objects.\n"
        << "Test failed.\n";
    else if (ratio > TOO_SLOW_FOR_NLOGN)
        cout << "Too many Entry accesses for n log n.\n"
        << "Test failed.\n";
    else
        cout << "Test passed.\n";
}
