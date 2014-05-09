// THIS IS THE PROVIDED CODE FOR PROGRAM #2, DSA 1, SPRING 2014

#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <cctype>
#include <cstdlib>

using namespace std;

// A simple class; each object holds one string
class Data {
public:
  string data;
  Data(const string &s) { data = s; }
};

// Load the data from a specified input file
void loadDataList(list<Data *> &l) {
  string filename;
  cout << "Enter name of input file: ";
  cin >> filename;

  ifstream input(filename.c_str());
  if (!input) {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  // The first line indicates the size
  string line;
  getline(input, line);
  stringstream ss(line);
  int size;
  ss >> size;

  // Load the data
  for (int i = 0; i < size; i++) {
    string line;
    getline(input, line);
    l.push_back(new Data(line));
  }

  input.close();
}

// Output the data to a specified input file
void writeDataList(const list<Data *> &l) {
  string filename;
  cout << "Enter name of output file: ";
  cin >> filename;

  ofstream output(filename.c_str());
  if (!output) {
    cerr << "Error: could not open " << filename << endl;
    exit(1);
  }

  // Write the size first
  int size = l.size();
  output << size << endl;

  // Write the data
  for (list<Data *>::const_iterator ipD = l.begin(); ipD != l.end(); ipD++) {
    output << (*ipD)->data << endl;
  }

  output.close();
}

void sortDataList(list<Data *> &);

// The main function calls routines to get the data, sort the data,
// and output the data. The sort is timed according to CPU time.
int main() {
  list<Data *> theList;
  loadDataList(theList);

  cout << "Data loaded.  Executing sort..." << endl;

  clock_t t1 = clock();
  sortDataList(theList);
  clock_t t2 = clock();
  double timeDiff = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

  cout << "Sort finished. CPU time was " << timeDiff << " seconds." << endl;

  writeDataList(theList);
}

// Professor Sable is cool.

// -------------------------------------------------
// YOU MAY NOT CHANGE OR ADD ANY CODE ABOVE HERE !!!
// -------------------------------------------------

#include <queue>

bool greaterEqual(Data * d2, Data * d1) {
    // Return (d1 >= d2). This is necessary because Data is not comparable as we require.
    // Use this comparator as necessary, in particular useful as an override for 
    // std::list.sort() which allows us to implement built in MergeSort.

    int length1 = d1->data.length();
    int length2 = d2->data.length();

    int leftOfDecimal1 = d1->data.find(".");
    int leftOfDecimal2 = d2->data.find(".");

    // If the number of digits to the left of the decimal is greater in one case than
    // the other, that value is larger.
    if (leftOfDecimal1 > leftOfDecimal2) {
        return true;
    } else if (leftOfDecimal1 < leftOfDecimal2) {
        return false;
    }

    // Otherwise, compare each digit from left to right until one digit in a Data object
    // is greater than the same digit in the other Data object.
    int compareAt = 0;
    while (compareAt < length1 && compareAt < length2) {
        char point1 = d1->data.at(compareAt);
        char point2 = d2->data.at(compareAt);
        if (point1 > point2) {
            return true;
        } else if (point1 < point2) {
            return false;
        }
        compareAt++;
    }

    // If all digits are equal up to a certain point, but one string has more digits following
    // that point, that is the larger value.
    if (length1 > length2) {
        return true;
    } else if (length1 < length2) {
        return false;
    }

    // If none of these conditions are met, the values are equal.
    return true;
}

void insertionSort(list<Data *> &l) {
    list<Data *>::iterator compareTo;
    list<Data *>::iterator placeHolder;
    list<Data *>::iterator it = l.begin();
    it++;

    for(; it != l.end(); it++) {
        placeHolder = it;
        compareTo = it;
        compareTo--;
        while (greaterEqual(*placeHolder, *compareTo)) {
            swap(*compareTo, *placeHolder);
            placeHolder--;
            compareTo--;
            if (placeHolder == l.begin()) {
                break;
            }
        }
    }
}

void radixSort(list<Data *> &l) {
    queue<Data *> bins[10];
    int leftOfDecimal;

    for (int i = 0; i < 7; i++) {
        if (i == 3) {
            continue;
        }

        list<Data *>::iterator it = l.begin();
        for (; it != l.end(); it++) {
            if ((*it)->data.size() != 7) {
                leftOfDecimal = (*it)->data.find(".");
                int rightOfDecimal = (*it)->data.length() - (leftOfDecimal + 1);
                int binValue = 0;

                if (3 - leftOfDecimal > i || i > 3 + rightOfDecimal) {
                    binValue = 0;
                } else {
                    char valueAt = (*it)->data.at(i - (3 - leftOfDecimal));
                    binValue = valueAt -'0';
                }

                bins[binValue].push(*it);

            } else {
                char valueAt = (*it)->data.at(i);
                int binValue = valueAt - '0';
                bins[binValue].push(*it);
            }   
        }

        it = l.begin();
        int currentBin = 0;
        for (; it != l.end(); it++) {
            while (bins[currentBin].empty()) {
                currentBin++;
                if (currentBin == 10) {
                    break;
                }
            }

            if (currentBin == 10) {
                break;
            }

            Data * newValue = bins[currentBin].front();
            swap(*it, newValue);
            bins[currentBin].pop();
        }
    }
}

void sortDataList(list<Data *> &l) {
    list<Data *>::iterator it = l.begin();

    if ((*it)->data.length() <= 7) {
        // Check if the length of the first Data element is short; if so, Case 3.
        // Case 3 will use Radix Sort, as defined in radixSort().
        radixSort(l);
    } else {
        // Scan through just the first 100 entries, if more than 80 of them are sorted,
        // assume that we are in Case 4. 
        int correctlySorted = 0;
        for (int i = 0; i < 100; i++) {
            Data * d1 = *(it);
            it++;
            Data * d2 = *(it);
            if (greaterEqual(d1, d2)) {
                correctlySorted++;
            }
        }

        if (correctlySorted >= 80) {
            // 80% of the first 100 are correctly sorted; assume Case 4. It is bad when Case 2
            // gets classified as Case 4, slightly less bad if Case 4 misclassified as Case 2.
            // Case 4 will use Insertion Sort, as defined in insertionSort().
            insertionSort(l);
        } else {
            // Case 1 or 2. Implement C++ built in MergeSort function, with overloaded Compare method,
            // as defined above in greaterEqual().
            l.sort(greaterEqual); 
        }
    }
}
