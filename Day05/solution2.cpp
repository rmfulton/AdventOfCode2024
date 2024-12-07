#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <vector>
#include <regex>

using namespace std;

vector<int> parseConstraint(string line)
{
    for (int i = 0; i < line.size(); ++i)
    {
        if (line[i] == '|')
        {
            line[i] = ' ';
        }
    }
    istringstream ss(line);
    vector<int> constraint;
    int x;
    for (int i = 0; i < 2; ++i)
    {
        ss >> x;
        constraint.push_back(x);
    }
    return constraint;
}

vector<int> parsePage(string line)
{
    for (int i = 0; i < line.size(); ++i)
    {
        if (line[i] == ',')
        {
            line[i] = ' ';
        }
    }
    istringstream ss(line);
    vector<int> page;
    for (int x; ss >> x;)
        page.push_back(x);
    return page;
}

vector<vector<vector<int> > > parseInput(ifstream *infile)
{
    string line = "|";
    vector<vector<int> > constraints;
    vector<vector<int> > pages;
    while (*infile >> line)
    {
        if (line.find("|") != string::npos)
        {
            vector<int> constraint = parseConstraint(line);
            constraints.push_back(constraint);
        }
        else
        {
            vector<int> page = parsePage(line);
            pages.push_back(page);
        }
    }
    vector<vector<vector<int> > > constraintsAndPages;
    constraintsAndPages.push_back(constraints);
    constraintsAndPages.push_back(pages);
    return constraintsAndPages;
}

bool isGoodPage(vector<int> candidate, vector<vector<int> > constraints)
{
    map<int, int> indexOf;
    for (int i = 0; i < candidate.size(); ++i)
    {
        indexOf[candidate[i]] = i;
    }
    for (vector<int> c : constraints)
    {
        int left = c[0];
        int right = c[1];
        if (indexOf.find(left) != indexOf.end())
        {
            if (indexOf.find(right) != indexOf.end())
            {
                if (indexOf[left] > indexOf[right])
                {
                    return false;
                }
            }
        }
    }
    return true;
}
vector<int> fixedPage(vector<int> candidate, vector<vector<int> > constraints)
{
    map<int, int> indexOf;
    for (int i = 0; i < candidate.size(); ++i)
    {
        indexOf[candidate[i]] = i;
    }
    bool isBadPage = true;
    while (isBadPage)
    {
        //innocent until proven
        isBadPage = false;
        for (vector<int> c : constraints)
        {
            int left = c[0];
            int right = c[1];
            if (indexOf.find(left) != indexOf.end())
            {
                if (indexOf.find(right) != indexOf.end())
                {
                    if (indexOf[left] > indexOf[right])
                    {
                        //swap
                        candidate[indexOf[left]] = right;
                        candidate[indexOf[right]] = left;
                        //update indices
                        int tmp = indexOf[left];
                        indexOf[left] = indexOf[right];
                        indexOf[right] = tmp;
                        //this might invalidate previous checks
                        isBadPage = true;
                    }
                }
            }
        }
    }
    return candidate;
}
vector<vector<int> > getFixedBadPages()
{
    string filename = "input.txt";
    ifstream infile(filename);
    vector<vector<vector<int> > > candP = parseInput(&infile);
    vector<vector<int> > constraints = candP[0];
    vector<vector<int> > pages = candP[1];
    vector<vector<int> > badPages;
    for (vector<int> p : pages)
    {
        if (!isGoodPage(p, constraints))
        {
            badPages.push_back(p);
        }
    }
    vector<vector<int> > fixedPages;
    for (vector<int> p : badPages)
    {
        fixedPages.push_back(fixedPage(p, constraints));
    }
    return fixedPages;
}

int main()
{
    vector<vector<int> > goodPages = getFixedBadPages();
    int s = 0;
    for (vector<int> p : goodPages)
    {
        s += p[p.size() / 2];
    }
    std::cout << s << std::endl;
    return 0;
}