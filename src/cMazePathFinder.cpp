#include <algorithm>
#include "cMazePathFinder.h"
using namespace boost;

cMazePathFinder::cMazePathFinder()
    : myFormat(eformat::none)
{
}
void cMazePathFinder::read(
    const std::string &fname)
{
    std::ifstream inf(fname);
    if (!inf.is_open())
    {
        std::cout << "cannot open " << fname << "\n";
        exit(1);
    }

    bool even = true;
    myColCount = 0;
    myRowCount = 0;
    myGrid.clear();
    std::string line;
    while (std::getline(inf, line))
    {
        std::cout << line << "\n";

        if (myFormat == eformat::none)
        {
            if (line[0] == '+')
                myFormat = eformat::art;
            else if (line[0] == 'c')
            {
                myFormat = eformat::cellcost;
                directed();
                continue; // jump past first line
            }
            else
                myFormat = eformat::numeric;
        }

        if (myFormat == eformat::numeric)
        {
            readnumeric(line);
            continue;
        }

        if (myFormat == eformat::cellcost)
        {
            readcellcost(line);
            continue;
        }

        if (!even)
        {
            if (!myColCount)
                myColCount = line.length() / 4;
            else if (line.length() / 4 != myColCount)
            {
                std::cout << line.length() << "\n";
                throw std::runtime_error("Bad column count");
            }
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
                {
                    std::cout << line.length() << "\n";
                    throw std::runtime_error("Bad column count");
                }
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

    if (myFormat == eformat::numeric)
        grid2graph();
    else if (myFormat == eformat::cellcost)
        grid2graphcellcost();
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

void cMazePathFinder::readcellcost(
    const std::string &line)
{
    auto token = ParseSpaceDelimited(line);
    if (!myColCount)
        myColCount = token.size();
    else if (token.size() != myColCount)
        throw std::runtime_error("Bad column count");
    std::vector<int> row;
    for (auto &sc : token)
    {
        switch (sc[0])
        {
        case 's':
            myStart = myColCount * myGrid.size() + row.size();
            sc = sc.substr(1);
            break;
        case 'e':
            myEnd = myColCount * myGrid.size() + row.size();
            sc = sc.substr(1);
            break;
        default:
            break;
        }
        row.push_back(atoi(sc.c_str()));
    }
    myGrid.push_back(row);
    myRowCount = myGrid.size();

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

void cMazePathFinder::grid2graphcellcost()
{
    for (int row = 0; row < myRowCount; row++)
    {
        for (int col = 0; col < myColCount; col++)
        {
            int n = row * myColCount + col;
            if (row > 0)
                addLink(
                    n,
                    (row - 1) * myColCount + col,
                    myGrid[row - 1][col]);
            if (col > 0)
                addLink(
                    n,
                    (row)*myColCount + col - 1,
                    myGrid[row][col - 1]);
            if( row < myRowCount - 1 )
                addLink(
                    n,
                    (row+1) * myColCount + col,
                    myGrid[row+1][col] );
            if( col < myColCount - 1 )
                addLink(
                    n,
                    row * myColCount + col + 1,
                    myGrid[row][col+1] );
        }
    }
}

std::vector<int> cMazePathFinder::NodesConnected(int v)
{
    std::vector<int> vc;
    if( myFormat == eformat::cellcost ) {
    } else {
    typedef std::pair<graph_traits<graph_t>::out_edge_iterator, graph_traits<graph_t>::out_edge_iterator> out_edge_iter_pair_t;
    for (out_edge_iter_pair_t ep = out_edges(v, myGraph);
         ep.first != ep.second; ++(ep.first))
    {
        vc.push_back(target(*ep.first, myGraph));
    }
    }
    return vc;
}

std::string cMazePathFinder::asciiArtText()
{
    //std::cout << "asciiArtText " << myRowCount << " rows " << myColCount << " cols\n";

    // construct blank charcter grid
    std::vector<char> r(myColCount * 4 + 1, ' ');
    std::vector<std::vector<char>> grid(2 * myRowCount + 1, r);

    int row, col;

    // wall along on bottom
    for (col = 0; col < 4 * myColCount + 1; col += 4)
    {
        grid[2 * myRowCount][col] = '+';
        grid[2 * myRowCount][col + 1] = '-';
        grid[2 * myRowCount][col + 2] = '-';
        grid[2 * myRowCount][col + 3] = '-';
    }
    for (row = 0; row < 2 * myRowCount; row += 2)
    {
        // wall on right
        grid[row + 1][4 * myColCount] = '|';

        // loop over cells
        for (col = 0; col < 4 * myColCount; col += 4)
        {
            // top left cell corner
            grid[row][col] = '+';

            // cell index in graph
            int n = (row / 2) * myColCount + (col / 4);

            // connected cells
            auto vc = NodesConnected(n);

            // cell in path
            if (std::find(
                    myPath.begin(),
                    myPath.end(),
                    n) != myPath.end())
                grid[row + 1][col + 2] = '*';

            if( myFormat != eformat::cellcost) {
            // connected to previous cell in row
            bool open = false;
            for (int c : vc)
            {
                if (c == n - 1)
                    open = true;
            }
            if (!open)
                grid[row + 1][col] = '|';

            // connected to cell above
            open = false;
            for (int c : vc)
            {
                if (c == n - myColCount)
                    open = true;
            }
            if (!open)
            {
                grid[row][col + 1] = '-';
                grid[row][col + 2] = '-';
                grid[row][col + 3] = '-';
            }
            }

            // start cell
            if (n == myStart)
                grid[row + 1][col + 2] = 's';

            // end cell
            if (n == myEnd)
                grid[row + 1][col + 2] = 'e';
        }

        // top left corner of last cell in row
        grid[row][col] = '+';
    }

    std::stringstream ss;
    for (int row = 0; row < 2 * myRowCount + 1; row++)
    {
        for (int col = 0; col < 4 * myColCount + 1; col++)
        {
            ss << grid[row][col];
        }
        ss << "\n";
    }

    return ss.str();
}
