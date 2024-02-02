#include <iostream>
#include <fstream>
#include "trie.h"
#include "PageRank.h"

using namespace std;

//Using insertion sort will be very effective, as lots of pages have the same rank, the vector is partially sorted
void insertionSort(std::vector<double> &ranks, std::vector<string> &sources)
{
    int n = ranks.size();

    for (int i = 1; i < n; ++i)
    {
        double key = ranks[i];
        string key2 = sources[i];
        int j;

        // Find the correct position for the current element
        for (j = i - 1; j >= 0 && ranks[j] < key; --j)
        {
            ranks[j + 1] = ranks[j];
            sources[j + 1] = sources[j];
        }

        // Insert the current element into the correct position
        ranks[j + 1] = key;
        sources[j + 1] = key2;
    }
}

int main()
{

    std::string filename = "matrix.txt";

    Eigen::MatrixXd resul = page_ranking(filename);

    Trie<string> info;

    std::string file_name = "output.txt";

    // Creating an input file stream object
    std::ifstream infile(file_name);

    // Checking if the file is open
    if (!infile.is_open())
    {
        std::cerr << "Error opening file: " << file_name << std::endl;
        return 1;
    }

    // Reading data from the file
    std::string line;
    string source;

    vector<string> sources;
    vector<double> ranks;

    while (std::getline(infile, line))
    {
        if (line.find("http:/") != std::string::npos || line.find("https:/") != std::string::npos)
        {
            source = line;
            sources.push_back(line);
        }
        else
        {
            info.insert((string)line, (string)source);
        }
    }

    for (int i = 0; i < resul.rows(); i++)
    {
        ranks.push_back(resul(i));
    }

    infile.close();


    insertionSort(ranks, sources);

    string request;
    cin>>request;

    std::vector<string> a=info.search(request);

    int size=sources.size();

    bool found=false;

    for(int i=0; i<size; i++)
    {
        for(int k=0; k<a.size(); k++)
        {
            if(sources[i]==a[k])
            {
                cout<<sources[i]<<endl;
                found=true;
            }
        }
    }
    if(!found)
        cout << "Your search \""<< request <<"\" did not match any documents."<<endl;

            return 0;
}
