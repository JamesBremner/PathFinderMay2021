#include "cHillPathFinder.h"

void cHillPathFinder::read(
    const std::string &fname)
{
    std::ifstream inf(fname);
    if (!inf.is_open())
    {
        std::cout << "cannot open " << fname << "\n";
        exit(1);
    }
    myGrid.clear();
    myRowCount = 0;
    myColCount = -1;
    std::string line;
    while (std::getline(inf, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {
        case 'h':
        {
            if (myColCount == -1)
                myColCount = token.size() - 1;
            else if (token.size() - 1 != myColCount)
                throw std::runtime_error("Bad column count");
            std::vector<float> row;
            for (int k = 1; k < token.size(); k++)
                row.push_back(atof(token[k].c_str()));
            myGrid.push_back(row);
        }
        break;
        case 's':
            if (token.size() != 3)
                throw std::runtime_error("Bad start");
            myStartCol = atoi(token[1].c_str())-1;
            myStartRow = atoi(token[2].c_str())-1;
            break;
        case 'e':
            if (token.size() != 3)
                throw std::runtime_error("Bad end");
            myEndCol = atoi(token[1].c_str())-1;
            myEndRow = atoi(token[2].c_str())-1;
            break;
        }
    }
    myRowCount = myGrid.size();
   // std::cout << "<-cHillPathFinder::read " << myRowCount << "\n"; 
}
void cHillPathFinder::ConstructBoostGraph()
{
    //std::cout << "->ConstructBoostGraph " << myColCount << " " << myRowCount << "\n";
    for (int row = 0; row < myRowCount; row++)
    {
        for (int col = 0; col < myColCount; col++)
        {
            int n = findoradd(name(col, row));
        }
    }

    for (int row = 0; row < myRowCount; row++)
        for (int col = 0; col < myColCount; col++)
        {
            int n = find(name(col, row));
            if (col > 0)
            {
                int left = find(name(col - 1, row));
                float delta = myGrid[row][col] - myGrid[col - 1][row];
                addLink(n, left, 1 + delta * delta);
            }
            if (col < myColCount - 1)
            {
                int right = find(name(col + 1, row));
                float delta = myGrid[row][col] - myGrid[col + 1][row];
                addLink(n, right, 1 + delta * delta);
            }
            if (row > 0)
            {
                int up = find(name(col, row - 1));
                float delta = myGrid[row][col] - myGrid[col][row - 1];
                addLink(n, up, 1 + delta * delta);
            }
            if (row < myRowCount - 1)
            {
                int up = find(name(col, row + 1));
                float delta = myGrid[row][col] - myGrid[col][row + 1];
                addLink(n, up, 1 + delta * delta);
            }
        }
    myStart = find(name(myStartCol, myStartRow));
    myEnd = find(name(myEndCol, myEndRow));

    // std::cout << "<-ConstructBoostGraph " 
    //     << myEndCol <<" "<< myEndRow <<" "
    //     << myStart <<" "<< myEnd << "\n";
}

std::string cHillPathFinder::name(int col, int row)
{
    return std::to_string(col + 1) + "_" + std::to_string(row + 1);
}

