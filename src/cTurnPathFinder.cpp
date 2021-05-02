#include "cTurnPathFinder.h"

void cTurnPathFinder::read(
    const std::string &fname)
{
    std::ifstream inf(fname);
    if (!inf.is_open())
    {
        std::cout << "cannot open " << fname << "\n";
        exit(1);
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
        case 'l':
            if (token.size() != 5)
                throw std::runtime_error("cTurnPathFinder::read bad link line");
            {
                sDirLink L;
                L.src = atoi(token[1].c_str());
                L.dst = atoi(token[2].c_str());
                L.dir = atoi(token[3].c_str());
                L.cost = atof(token[4].c_str());
                myInputLink.push_back(L);
            }
            break;

        case 's':
            if (token.size() != 3)
                throw std::runtime_error("cPathFinder::read bad start line");
            myStart = atoi(token[1].c_str());
            myStartDirection = 1;
            if (token[2][0] == 'b')
                myStartDirection = 2;
            break;

        case 'e':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad end line");
            myEnd = atoi(token[1].c_str());
            break;

        case 't':
            if (token.size() != 5)
                throw std::runtime_error("cTurnPathFinder::read bad turning line");
            {
                sDirLink L;
                L.src = atoi(token[1].c_str());
                L.dir = -99;
                L.cost = atof(token[4].c_str());
                myInputLink.push_back(L);
            }
            break;
        }
    }
}

void cTurnPathFinder::split()
{
    std::cout << "split\n";
    for (auto &l : myInputLink)
    {
        switch (l.dir)
        {
        case 0:
            myForward.push_back(l);
            myBack.push_back(l);
            break;

        case 1:
            myForward.push_back(l);
            break;

        case 2:
            myBack.push_back(l);
            break;

        case -99:
            myTurn.push_back(l);
            break;
        }
    }

    std::cout << "\nForward links\n";
    for (auto &l : myForward)
    {
        std::cout
            << std::to_string(l.src)
            << "f - "
            << std::to_string(l.dst)
            << "f\n";
    }
    std::cout << "\nBack links\n";
    for (auto &l : myBack)
    {
        std::cout
            << std::to_string(l.src)
            << "b - "
            << std::to_string(l.dst)
            << "b\n";
    }
    std::cout << "\nTurning Links\n";
    for (auto &l : myTurn)
    {
        std::cout
            << std::to_string(l.src)
            << "f - "
            << std::to_string(l.src)
            << "b\n";
    }
}

void cTurnPathFinder::ConstructBoostGraph()
{
    for (auto &l : myForward)
    {
        auto s = findoradd(std::to_string(l.src) + "f");
        auto d = findoradd(std::to_string(l.dst) + "f");
        addLink(s, d, l.cost);
    }
    for (auto &l : myBack)
    {
        auto s = findoradd(std::to_string(l.src) + "b");
        auto d = findoradd(std::to_string(l.dst) + "b");
        addLink(s, d, l.cost);
    }
    for (auto &l : myTurn)
    {
        // add edge begining and ending on node
        // which allows the robot to turn around
        auto s = findoradd(std::to_string(l.src) + "f");
        auto d = findoradd(std::to_string(l.src) + "b");
        addLink(s, d, l.cost);
    }
}

void cTurnPathFinder::tpath()
{
    // starting node in boost graph
    std::string startNodeName = std::to_string( myStart );
    if (myStartDirection == 1 )
        startNodeName += "f";
    else
        startNodeName += "b";
    myStart = find(startNodeName);
    // std::cout << "start " << startNodeName 
    //     << " " << myStart
    //     << " " << myStartDirection << "\n";       
    if (myStart < 0)
        throw std::runtime_error("Bad path start");

    cPathFinder::path();

    std::string sgf = std::to_string(myEnd) + "f";
    std::string sgb = std::to_string(myEnd) + "b";
    int goalf = find(sgf);
    int goalb = find(sgb);
    if (goalf < 0 && goalb < 0)
        throw std::runtime_error("Bad path goal");
            // choose closest possible goal
    myEnd = goalf;
    if (goalf < 0 || goalb < 0)
    {
        if (goalf < 0)
            myEnd = goalb;
    }
    else
    {
        if (myDist[goalb] < myDist[goalf])
            myEnd = goalb;
    }

    pathPick();


}
