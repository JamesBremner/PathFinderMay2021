#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "cPathFinderReader.h"

cPathFinderReader::eFormat
cPathFinderReader::open(const std::string &fname)
{
    eFormat ret = eFormat::none;
    myFile.open(fname);
    if (!myFile.is_open())
        return eFormat::not_open;
    std::string line;
    getline(myFile, line);
    if (line.find("format") != 0)
        return eFormat::none;
    if (line.find("costs") != -1)
    {
        costs();
        myFinder.path();
        std::cout << myFinder.pathText() << "\n";
        return eFormat::costs;
    }
    if (line.find("spans") != -1)
    {
        costs();
        myFinder.span();
        std::cout << myFinder.spanText();
        return eFormat::spans;
    }
    if (line.find("sales") != -1)
    {
        sales();
        myFinder.tsp();
        std::cout << myFinder.pathText() << "\n";
        return eFormat::sales;
    }
    if (line.find("hills") != -1)
    {
        myFinder.hills(orthogonalGrid());
        return eFormat::hills;
    }
    if (line.find("cams") != -1)
    {
        costs(false);
        myFinder.cams();
        return eFormat::cams;
    }
    if (line.find("gsingh") != -1)
    {
        multi();
        myFinder.gsingh();
        return eFormat::gsingh;
    }
    if (line.find("shaun") != -1)
    {
        shaun();
        myFinder.shaun();
        return eFormat::shaun;
    }
    if (line.find("islands") != -1)
    {
        islands();
        std::cout << "There are "
                  << myFinder.islandCount()
                  << " islands\n";
        return eFormat::islands;
    }
    return ret;
}

std::vector<std::string> cPathFinderReader::ParseSpaceDelimited(
    const std::string &l)
{
    std::vector<std::string> token;
    std::stringstream sst(l);
    std::string a;
    while (getline(sst, a, ' '))
        token.push_back(a);

    token.erase(
        remove_if(
            token.begin(),
            token.end(),
            [](std::string t)
            {
                return (t.empty());
            }),
        token.end());

    return token;
}

void cPathFinderReader::costs(bool weights)
{
    myFinder.clear();
    int cost;
    int maxNegCost = 0;
    std::string line;
    while (std::getline(myFile, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {
        case 'g':
            if (myFinder.linkCount())
                throw std::runtime_error("cPathFinderReader:: g ( directed ) must occurr before any links");
            myFinder.directed();
            break;

        case 'l':
            if (weights && (token.size() != 4))
                throw std::runtime_error("cPathFinder::read bad link line");
            else if (3 > token.size() || token.size() > 4)
                throw std::runtime_error("cPathFinder::read bad link line");
            if (weights)
                cost = atof(token[3].c_str());
            else
                cost = 1;
            if (cost < maxNegCost)
                maxNegCost = cost;
            myFinder.addLink(
                myFinder.findoradd(token[1]),
                myFinder.findoradd(token[2]),
                cost);
            break;

        case 's':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad start line");
            myFinder.start(token[1]);
            break;

        case 'e':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad end line");
            myFinder.end(token[1]);
            break;
        }
    }
    if (maxNegCost < 0)
    {
        std::cout << "Negative link costs present\n"
                  << "Adding positive offset to all link costs\n";
        myFinder.makeCostsPositive(maxNegCost);
    }
}

std::vector<std::string> cPathFinderReader::singleParentTree()
{
    std::ifstream inf(myfname);
    if (!inf.is_open())
    {
        throw std::runtime_error("cannot open " + myfname);
    }
    std::string line;
    while (std::getline(inf, line))
    {
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {

        case 't':
        {
            token.erase(token.begin());
            int child = 0;
            for (auto &t : token)
            {
                if (!child)
                {
                    child++;
                    continue;
                }
                myFinder.addLink(
                    myFinder.findoradd(t),
                    myFinder.findoradd(std::to_string(child++)),
                    1);
            }
        }
        break;

        case 'a':
            token.erase(token.begin());
            return token;
        }
    }
    throw std::runtime_error("no A input");
}

void cPathFinderReader::sales()
{
    struct sCity
    {
        int x;
        int y;
        std::string name;
    };
    std::vector<sCity> vCity;
    sCity city;

    enum class eInput
    {
        unknown,
        city,
        link,
    } input = eInput::unknown;

    std::string line;
    while (std::getline(myFile, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        if (token.size() != 4)
            throw std::runtime_error("Input line must have 4 tokens");

        switch (token[0][0])
        {
        case 'c':
            if (input == eInput::unknown)
                input = eInput::city;
            else if (input != eInput::city)
                throw std::runtime_error("Mixed input formats");
            city.x = atoi(token[1].c_str());
            city.y = atoi(token[2].c_str());
            city.name = token[3];
            vCity.push_back(city);
            break;
        case 'l':
            if (input == eInput::unknown)
                input = eInput::link;
            else if (input != eInput::link)
                throw std::runtime_error("Mixed input formats");
            myFinder.addLink(
                myFinder.findoradd(token[1]),
                myFinder.findoradd(token[2]),
                atoi(token[3].c_str()));
            break;
        default:
            std::cout << "ignored\n";
            break;
        }
    }

    if (input == eInput::city)
    {
        // link all the cities by the pythagorian distance between them
        for (int c1 = 0; c1 < vCity.size(); c1++)
        {
            for (int c2 = c1 + 1; c2 < vCity.size(); c2++)
            {
                float dx = vCity[c1].x - vCity[c2].x;
                float dy = vCity[c1].y - vCity[c2].y;
                float d = sqrt(dx * dx + dy * dy);
                std::cout << vCity[c1].name << " " << vCity[c2].name << " "
                          << dx << " " << dy << " " << d << "\n";
                myFinder.addLink(
                    myFinder.findoradd(vCity[c1].name),
                    myFinder.findoradd(vCity[c2].name),
                    (int)d);
            }
        }
    }
    else
    {
        // add links with infinite cost bewteen unlinked cities
        myFinder.makeComplete();
    }
}

std::vector<std::vector<float>> cPathFinderReader::orthogonalGrid()
{
    if (!myFile.is_open())
        throw std::runtime_error(
            "cPathFinderReader::orthogonalGrid file not open");

    const bool fDirected = true;

    myFinder.clear();
    myFinder.directed(fDirected);

    std::vector<std::vector<float>> grid;
    int RowCount = 0;
    int ColCount = -1;
    std::string line;
    while (std::getline(myFile, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {
        case 'o':
        {
            if (ColCount == -1)
                ColCount = token.size() - 1;
            else if (token.size() - 1 != ColCount)
                throw std::runtime_error("Bad column count");
            std::vector<float> row;
            for (int k = 1; k < token.size(); k++)
                row.push_back(atof(token[k].c_str()));
            grid.push_back(row);
        }
        break;
        case 's':
            if (token.size() != 3)
                throw std::runtime_error("Bad start");
            if (ColCount == -1)
                throw std::runtime_error("Start node must be at end");
            myFinder.start(
                (atoi(token[2].c_str()) - 1) * ColCount + atoi(token[1].c_str()) - 1);
            break;
        case 'e':
            if (token.size() != 3)
                throw std::runtime_error("Bad end");
            if (ColCount == -1)
                throw std::runtime_error("End node must be at end");
            myFinder.end(
                (atoi(token[2].c_str()) - 1) * ColCount + atoi(token[1].c_str()) - 1);
            break;
        }
    }
    RowCount = grid.size();
    // std::cout << "<-cHillPathFinder::read " << myRowCount << "\n";

    // add nodes at each grid cell
    for (int row = 0; row < RowCount; row++)
    {
        for (int col = 0; col < ColCount; col++)
        {
            int n = myFinder.addNode(
                "c" + std::to_string(col + 1) + "r" + std::to_string(row + 1));
        }
    }

    // link cells orthogonally
    for (int row = 0; row < RowCount; row++)
        for (int col = 0; col < ColCount; col++)
        {
            int n = row * ColCount + col;

            if (fDirected)
            {
                if (col > 0)
                {
                    int left = row * ColCount + col - 1;
                    myFinder.addLink(n, left, 1);
                }
            }
            if (col < ColCount - 1)
            {
                int right = row * ColCount + col + 1;
                myFinder.addLink(n, right, 1);
            }
            if (fDirected)
            {
                if (row > 0)
                {
                    int up = (row - 1) * ColCount + col;
                    myFinder.addLink(n, up, 1);
                }
            }
            if (row < RowCount - 1)
            {
                int down = (row + 1) * ColCount + col;
                myFinder.addLink(n, down, 1);
            }
        }
    return grid;
}
std::string cPathFinderReader::orthogonalGridNodeName(
    int row, int col)
{
    return "c" + std::to_string(col + 1) + "r" + std::to_string(row + 1);
}
void cPathFinderReader::islands()
{
    if (!myFile.is_open())
        throw std::runtime_error(
            "cPathFinderReader::orthogonalGrid file not open");

    std::vector<std::vector<char>> grid;
    int RowCount = 0;
    int ColCount = -1;
    std::string line;
    while (std::getline(myFile, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {
        case 'o':
        {
            if (ColCount == -1)
                ColCount = token.size() - 1;
            else if (token.size() - 1 != ColCount)
                throw std::runtime_error("Bad column count");
            std::vector<char> row;
            for (int k = 1; k < token.size(); k++)
                row.push_back(token[k][0]);
            grid.push_back(row);
        }
        break;
        }
    }
    RowCount = grid.size();

    // add nodes at each land cell
    for (int row = 0; row < RowCount; row++)
    {
        for (int col = 0; col < ColCount; col++)
        {
            if (grid[row][col] == '1')
                myFinder.addNode(
                    orthogonalGridNodeName(row, col));
        }
    }

    // link cells orthogonally
    for (int row = 0; row < RowCount; row++)
    {
        for (int col = 0; col < ColCount; col++)
        {
            int n = row * ColCount + col;

            if (col < ColCount - 1)
            {
                if (grid[row][col] == '1' && grid[row][col + 1] == '1')
                {
                    myFinder.addLink(
                        orthogonalGridNodeName(row, col ),
                        orthogonalGridNodeName(row, col + 1 ),
                        1);
                }
            }
            if (row < RowCount - 1)
            {
                if (grid[row][col] == '1' && grid[row + 1][col] == '1')
                {
                    myFinder.addLink(
                        orthogonalGridNodeName(row, col ),
                        orthogonalGridNodeName(row+1, col ),
                        1);
                }
            }
        }
    }
}

void cPathFinderReader::multi()
{
    myFinder.clear();
    std::string line;
    while (std::getline(myFile, line))
    {
        auto token = ParseSpaceDelimited(line);
        if (token.size() < 2)
            throw std::runtime_error("Bad link");
        int src = myFinder.findoradd(token[0]);
        for (int kt = 1; kt < token.size(); kt++)
            myFinder.addLink(
                src,
                myFinder.findoradd(token[kt]),
                1);
    }
}

void cPathFinderReader::shaun()
{
    myFinder.clear();
    int cost;
    int maxNegCost = 0;
    std::string line;
    while (std::getline(myFile, line))
    {
        std::cout << line << "\n";
        auto token = ParseSpaceDelimited(line);
        if (!token.size())
            continue;
        switch (token[0][0])
        {
        case 'n':
            if (token.size() != 3)
                throw std::runtime_error("Bad node");
            myFinder.nodeColor(
                myFinder.findoradd(token[1]),
                token[2]);
            break;

        case 'l':
            if (token.size() != 3)
                throw std::runtime_error("Bad link");
            myFinder.addLink(
                myFinder.findoradd(token[1]),
                myFinder.findoradd(token[2]),
                0);
            break;
        }
    }
}