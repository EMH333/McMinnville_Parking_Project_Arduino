#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

vector<vector<int>> readData(string file)
{
    vector<vector<int>> table;
    table.clear();

    // File pointer
    ifstream fin;

    // Open an existing file
    fin.open(file, ios::in);

    // Read the Data from the file
    // as String Vector
    string line, word, temp;

    while (fin >> temp)
    {
        vector<int> row;
        row.clear();

        // read an entire row and
        // store it in a string variable 'line'
        getline(fin, line);

        // used for breaking words
        stringstream s(line);

        // read every column data of a row and
        // store it in a string variable, 'word'
        while (getline(s, word, ','))
        {
            // add all the column data
            // of a row to a vector
            row.push_back(stoi(word));
        }
        table.push_back(row);
    }

    std::cout << "Done loading values" << endl;
    return table;
}
