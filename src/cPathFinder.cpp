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

void cPathFinder::paths( int start )
{
    // std::cout << "->cPathFinder::path " << num_vertices(myGraph) 
    //     <<" " << myStart << "\n";
    // run dijkstra algorithm
    myPred.resize( num_vertices(myGraph) );
    myDist.resize( num_vertices(myGraph) );
    boost::dijkstra_shortest_paths(
        myGraph,
        start,
        weight_map(get(&cEdge::myCost, myGraph))
            .predecessor_map(boost::make_iterator_property_map(
                myPred.begin(), get(boost::vertex_index, myGraph)))
            .distance_map(boost::make_iterator_property_map(
                myDist.begin(), get(boost::vertex_index, myGraph))));
    // std::cout << "<-cPathFinder::path ";
}

void cPathFinder::path()
{
    paths( myStart );
    pathPick( myEnd );
}

void cPathFinder::pathPick( int end ) {
    // std::cout << "->cPathFinder::pathPick "
    //     << myStart <<" " << myEnd << "\n";
    // pick out path, starting at goal and finishing at start
    myPath.push_back( end );
    int prev = end;
    while (1)
    {
        //std::cout << prev << " " << myPred[prev] << ", ";
        int next = myPred[prev];
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

int cPathFinder::findoradd(const std::string &name)
{
    int n = find(name);
    if (n < 0)
        n = add_vertex(name, myGraph);
    return n;
}

int cPathFinder::find(const std::string &name)
{
    for (int n = 0; n < num_vertices(myGraph); n++)
    {
        //std::cout << myGraph[n].myName << " ";
        if (myGraph[n].myName == name)
        {
            return n;
        }
    }
    //std::cout << name << " not found\n";
    return -1;
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
    std::cout << "\n";
    ss << "\n";
    return ss.str();
}