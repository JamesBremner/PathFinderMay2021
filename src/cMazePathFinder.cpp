#include "cMazePathFinder.h"
using namespace boost;
void cMazePathFinder::read(
    const std::string &fname)
{
    std::ifstream inf(fname);
    if (!inf.is_open())
    {
        std::cout << "cannot open " << fname << "\n";
        exit(1);
    }

    bool fascii = false;
    bool even = true;
    myColCount = 0;
    myRowCount = 0;
    myGrid.clear();
    std::string line;
    while (std::getline(inf, line))
    {
        std::cout << line << "\n";

        if (!myColCount)
        {
            if (line[0] == '+')
                fascii = true;
        }
        if (!fascii)
        {
            readnumeric(line);
            continue;
        }
        std::cout << "ascii\n";

        if (!even)
        {
            if (!myColCount)
                myColCount = line.length() / 4;
            else if (line.length() / 4 != myColCount)
                throw std::runtime_error("Bad column count");
            for (int col = 0; col < myColCount; col++)
            {
                if (line[col * 4] == ' ')
                {
                    int u = myRowCount * myColCount + col - 1;
                    int v = u + 1;
                    //std::cout << u << " " << v << ", ";
                    addLink(u, v, 1);
                }
                if (line[col * 4] == 's')
                {
                    myStart = myRowCount * myColCount;
                    //std::cout << "start " << myStart << "\n";
                }
                if (line[(col + 1) * 4] == 'e')
                {
                    myEnd = (myRowCount + 1) * myColCount - 1;
                    //std::cout << "end " << myEnd << "\n";
                }
            }
            //std::cout << "\n";
            myRowCount++;
        }
        else
        {
            if (myRowCount)
            {
                if (!myColCount)
                    myColCount = line.length() / 4;
                else if (line.length() / 4 != myColCount)
                    std::runtime_error("Bad column count");
                for (int col = 0; col < myColCount; col++)
                {
                    if (line[col * 4 + 1] == ' ')
                    {
                        int u = (myRowCount - 1) * myColCount + col;
                        int v = u + myColCount;
                        //std::cout << u << " " << v << ", ";
                        addLink(u, v, 1);
                    }
                }
                //std::cout << "\n";
            }
        }

        even = !even;
    }
    std::cout << myColCount << " by " << myRowCount << "\n";

    if (!fascii)
        grid2graph();
}

void cMazePathFinder::readnumeric(
    const std::string &line)
{
    //std::cout << "readnumeric\n";
    if (!myColCount)
        myColCount = 1 + line.length() / 2;
    else if (1 + line.length() / 2 != myColCount)
        throw std::runtime_error("Bad column count");

    std::vector<int> row;

    for (int col = 0; col < myColCount; col++)
    {
        switch (line[2 * col])
        {
        case '0':
            row.push_back(0);
            break;
        case '1':
            row.push_back(1);
            break;
        case 's':
            row.push_back(0);
            myStart = myRowCount * myColCount + col;
            break;
        case 'e':
            row.push_back(0);
            myEnd = myRowCount * myColCount + col;
            break;
        }
    }
    myGrid.push_back(row);
    myRowCount++;
}

void cMazePathFinder::grid2graph()
{
    for (int row = 0; row < myRowCount; row++)
    {
        for (int col = 0; col < myColCount; col++)
        {
            if (myGrid[row][col])
                continue;
            int n = row * myColCount + col;
            if (row > 0)
            {
                if (!myGrid[row - 1][col])
                    addLink(
                        n,
                        (row - 1) * myColCount + col,
                        1);
            }
            if (row < myRowCount - 1)
            {
                if (!myGrid[row + 1][col])
                    addLink(
                        n,
                        (row + 1) * myColCount + col,
                        1);
            }
            if (col > 0)
            {
                if (!myGrid[row][col - 1])
                    addLink(
                        n,
                        row * myColCount + col - 1,
                        1);
            }
            if (col < myColCount - 1)
            {
                if (!myGrid[row][col + 1])
                    addLink(
                        n,
                        row * myColCount + col + 1,
                        1);
            }
        }
    }
}
