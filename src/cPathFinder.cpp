#include <boost/graph/dijkstra_shortest_paths.hpp>
#include "cPathFinder.h"

void cPathFinder::read(
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
            if (token.size() != 4)
                throw std::runtime_error("cPathFinder::read bad link line");
            addLink(
                atoi(token[1].c_str()),
                atoi(token[2].c_str()),
                atof(token[3].c_str()));
            break;

        case 's':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad start line");
            myStart = atoi(token[1].c_str());
            break;

        case 'e':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad end line");
            myEnd = atoi(token[1].c_str());
            break;
        }
    }
}

void cPathFinder::path()
{
    // run dijkstra algorithm
    std::vector<int> p(num_vertices(myGraph));
    std::vector<int> vDist(num_vertices(myGraph));
    boost::dijkstra_shortest_paths(
        myGraph,
        myStart,
        weight_map(get(&cEdge::myCost, myGraph))
            .predecessor_map(boost::make_iterator_property_map(
                p.begin(), get(boost::vertex_index, myGraph)))
            .distance_map(boost::make_iterator_property_map(
                vDist.begin(), get(boost::vertex_index, myGraph))));

    // pick out path, starting at goal and finishing at start
    myPath.push_back(myEnd);
    int prev = myEnd;
    while (1)
    {
        //std::cout << prev << " " << p[prev] << ", ";
        int next = p[prev];
        myPath.push_back(next);
        if (next == myStart)
            break;
        prev = next;
    }

    // reverse so path goes from start to goal
    std::reverse(myPath.begin(), myPath.end());
}

void cPathFinder::addLink(
    int u,
    int v,
    float cost)
{
    myGraph[add_edge(u, v, myGraph).first].myCost = cost;
    myGraph[add_edge(v, u, myGraph).first].myCost = cost;
}

std::vector<std::string> cPathFinder::ParseSpaceDelimited(
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

std::string cPathFinder::linksText()
{
    std::stringstream ss;
    graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(myGraph); ei != ei_end; ++ei)
    {
        std::string un = myGraph[source(*ei, myGraph)].myName;
        if (un == "???")
            un = std::to_string(source(*ei, myGraph));
        std::string vn = myGraph[target(*ei, myGraph)].myName;
        if (vn == "???")
            vn = std::to_string(target(*ei, myGraph));
        ss << "("
           << un << ","
           << vn << ","
           << myGraph[*ei].myCost
           << ") ";
    }
    ss << "\n";
    return ss.str();
}

std::string cPathFinder::pathText()
{
    std::stringstream ss;
    for (auto n : myPath) {
        std::string sn = myGraph[n].myName;
        if( sn == "???")
            sn = std::to_string( n );
        ss << sn << " -> ";
    }
    ss << "\n";
    return ss.str();
}