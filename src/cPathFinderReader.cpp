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
    std::ifstream inf(myfname);
    if (!inf.is_open())
    {
        throw std::runtime_error("cannot open " + myfname);
    }
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
            myFinder.addLink(
                myFinder.findoradd(token[1]),
                myFinder.findoradd(token[2]),
                atof(token[3].c_str()));
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