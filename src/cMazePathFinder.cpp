#include "cMazePathFinder.h"

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
    int colCount = 0;
    int rowCount = 0;
    std::string line;
    while (std::getline(inf, line))
    {
        std::cout << line << "\n";

        if (!even)
        {
            if (!colCount)
                colCount = line.length() / 4;
            else if (line.length() / 4 != colCount)
                std::runtime_error("Bad column count");
            for (int col = 0; col < colCount; col++)
            {
                if (line[col * 4] == ' ')
                {
                    int u = rowCount * colCount + col - 1;
                    int v = u + 1;
                    //std::cout << u << " " << v << ", ";
                    addLink(u, v, 1);
                }
                if (line[col * 4] == 's')
                {
                    myStart = rowCount * colCount;
                    //std::cout << "start " << myStart << "\n";
                }
                if (line[(col+1) * 4] == 'e')
                {
                    myEnd = (rowCount+1) * colCount - 1;
                    //std::cout << "end " << myEnd << "\n";
                }
            }
            //std::cout << "\n";
            rowCount++;
        }
        else
        {
            if (rowCount)
            {
                if (!colCount)
                    colCount = line.length() / 4;
                else if (line.length() / 4 != colCount)
                    std::runtime_error("Bad column count");
                for (int col = 0; col < colCount; col++)
                {
                    if (line[col * 4 + 1] == ' ')
                    {
                        int u = (rowCount-1) * colCount + col;
                        int v = u + colCount;
                        //std::cout << u << " " << v << ", ";
                        addLink(u, v, 1);
                    }
                }
                //std::cout << "\n";
            }
        }

        even = !even;
    }
    std::cout << colCount << " by " << rowCount << "\n";
}
