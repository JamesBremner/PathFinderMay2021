#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "cPathFinderReader.h"

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
            [](std::string t) {
                return (t.empty());
            }),
        token.end());

    return token;
}

void cPathFinderReader::costs()
{
    myFinder.clear();
    std::ifstream inf(myfname);
    if (!inf.is_open())
    {
        throw std::runtime_error("cannot open " + myfname);
    }
    int cost;
    int maxNegCost = 0;
    std::string line;
    while (std::getline(inf, line))
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
            if (token.size() != 4)
                throw std::runtime_error("cPathFinder::read bad link line");
            cost = atof(token[3].c_str());
            if( cost < maxNegCost )
                maxNegCost = cost;
            myFinder.addLink(
                myFinder.findoradd(token[1]),
                myFinder.findoradd(token[2]),
                cost );
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
    if( maxNegCost < 0 )
    {
        std::cout << "Negative link costs present\n"
            << "Adding positive offset to all link costs\n";
        myFinder.negCost( maxNegCost );
    }
}

std::vector< std::string > cPathFinderReader::singleParentTree()
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
            token.erase( token.begin() );
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
            token.erase( token.begin() );
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

    std::ifstream f(myfname);
    if (!f.is_open())
    {
        throw std::runtime_error("cannot open " + myfname);
    }

    std::string line;
    while (std::getline(f, line))
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
        // link all the cities by the pythagporian distance between them
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
        // add links with infinite coset bewteen unlinked cities
        for (int c1 = 0; c1 < myFinder.linkCount(); c1++)
        {
            for (int c2 = c1 + 1; c2 < myFinder.linkCount(); c2++)
            {
                if (!myFinder.IsAdjacent(c1, c2))
                    myFinder.addLink(c1, c2,
                                   2000000 );
            }
        }
    }
}

