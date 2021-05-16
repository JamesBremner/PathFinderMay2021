#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include "cPathFinder.h"
using namespace boost;

void cPathFinder::clear()
{
    myGraph.clear();
    myDirGraph.clear();
}

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
        case 'g':
            directed();
            break;

        case 'l':
            if (token.size() != 4)
                throw std::runtime_error("cPathFinder::read bad link line");
            addLink(
                findoradd(token[1]),
                findoradd(token[2]),
                atof(token[3].c_str()));
            break;

        case 's':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad start line");
            myStart = find(token[1]);
            break;

        case 'e':
            if (token.size() != 2)
                throw std::runtime_error("cPathFinder::read bad end line");
            myEnd = find(token[1]);
            break;
        }
    }
}

void cPathFinder::paths(int start)
{
    if (!myfDirected)
        pathsT(start, myGraph);
    else
        pathsT(start, myDirGraph);
}
template <typename T>
void cPathFinder::pathsT(int start, T &g)
{
    std::cout << "->cPathFinder::path " << num_vertices(myGraph)
              << " " << myStart << "\n";
    // run dijkstra algorithm
    myPred.resize(num_vertices(g));
    myDist.resize(num_vertices(g));
    dijkstra_shortest_paths(
        g,
        start,
        weight_map(get(&cEdge::myCost, g))
            .predecessor_map(boost::make_iterator_property_map(
                myPred.begin(), get(boost::vertex_index, g)))
            .distance_map(boost::make_iterator_property_map(
                myDist.begin(), get(boost::vertex_index, g))));
    // std::cout << "<-cPathFinder::path ";
}

void cPathFinder::path()
{
    // run the Dijsktra algorithm
    paths(myStart);

    if (myEnd >= 0)

        // pick out the path from source to destination
        pathPick(myEnd);

    else
    {
        std::cout << "Hop count from root to every node:\n";
        for (int kv = 0; kv < num_vertices(myGraph); kv++)
        {
            if (kv == myStart)
                continue;
            pathPick(kv);
            //std::cout << pathText() << "\n";
            std::cout << namestring( myStart ) << " to " 
                << namestring( kv ) << " " 
                << myPath.size() - 1 << "\n";
        }
    }
}

void cPathFinder::span()
{
    typedef graph_traits<graph_t>::edge_descriptor edge_t;
    std::vector<edge_t> spanning_tree;
    kruskal_minimum_spanning_tree(
        myGraph,
        std::back_inserter(spanning_tree),
        weight_map(get(&cEdge::myCost, myGraph)));

    std::cout << "spanning_tree " << spanning_tree.size() << "\n";
    mySpanCost = 0;
    mySpan.clear();
    for (auto e : spanning_tree)
    {
        mySpanCost += myGraph[e].myCost;
        std::vector<int> ve{
            (int)source(e, myGraph),
            (int)target(e, myGraph)};
        mySpan.push_back(ve);
    }
}

void cPathFinder::pathPick(int end)
{
    myPath.clear();
    // std::cout << "->cPathFinder::pathPick "
    //     << myStart <<" " << end << "\n";

    if (myPred[end] == end)
        throw std::runtime_error("There is no path from " + std::to_string(myStart) + " to " + std::to_string(end));

    // pick out path, starting at goal and finishing at start
    myPath.push_back(end);
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
    if (!myfDirected)
        myGraph[add_edge(u, v, myGraph).first].myCost = cost;
    else
        myDirGraph[add_edge(u, v, myDirGraph).first].myCost = cost;
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
    if (!myfDirected)
        return linksTextT(myGraph);
    else
        return linksTextT(myDirGraph);
}
template <typename T>
std::string cPathFinder::linksTextT(T &g)
{
    std::stringstream ss;
    typename graph_traits<T>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(g); ei != ei_end; ++ei)
    {
        std::string un = g[source(*ei, g)].myName;
        if (un == "???")
            un = std::to_string(source(*ei, g));
        std::string vn = g[target(*ei, g)].myName;
        if (vn == "???")
            vn = std::to_string(target(*ei, g));
        ss << "("
           << un << ","
           << vn << ","
           << g[*ei].myCost
           << ") ";
    }
    ss << "\n";
    return ss.str();
}

std::string cPathFinder::namestring(int n)
{
    std::string sn = myGraph[n].myName;
    if (sn == "???")
        sn = std::to_string(n);
    return sn;
}

std::string cPathFinder::pathText()
{
    std::stringstream ss;
    for (auto n : myPath)
    {
        // std::string sn = myGraph[n].myName;
        // if (sn == "???")
        //     sn = std::to_string(n);
        // ss << sn << " -> ";
        ss << std::to_string(n) << " -> ";
    }
    std::cout << "\n";
    ss << "\n";
    return ss.str();
}

std::string cPathFinder::spanText()
{
    std::cout << "spanText " << mySpan.size()
              << " cost " << mySpanCost << "\n";

    std::stringstream ss;
    for (auto e : mySpan)
    {
        ss << namestring(e[0])
           << " - "
           << namestring(e[1])
           << ", ";
    }
    ss << "\n";
    return ss.str();
}

std::string cPathFinder::pathViz()
{
    return pathViz(myPath);
}

std::string cPathFinder::pathViz(
    const std::vector<int> &vp,
    bool all)
{
    std::stringstream f;
    f << "graph G {\n";
    for (int v = *vertices(myGraph).first; v != *vertices(myGraph).second; ++v)
        f << myGraph[v].myName << ";\n";

    graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(myGraph); ei != ei_end; ++ei)
    {
        bool onpath = false;
        int src = source(*ei, myGraph);
        int dst = target(*ei, myGraph);
        auto pathItsrc = std::find(vp.begin(), vp.end(), src);
        auto pathItdst = std::find(vp.begin(), vp.end(), dst);
        if (pathItsrc != vp.end() && pathItdst != vp.end())
        {
            if (pathItsrc == pathItdst + 1 || pathItsrc == pathItdst - 1)
                onpath = true;
        }

        if (all)
        {
            f << myGraph[src].myName << "--"
              << myGraph[dst].myName << " ";
            if (onpath)
                f << "[color=\"red\"] ";
            f << ";\n";
        }
        else
        {
            if (onpath)
                f << myGraph[src].myName << "--"
                  << myGraph[dst].myName << " ;\n";
        }
    }
    f << "}\n";
    return f.str();
}

std::string cPathFinder::spanViz(bool all)
{
    std::stringstream f;
    f << "graph G {\n";
    for (int v = *vertices(myGraph).first; v != *vertices(myGraph).second; ++v)
    {
        auto sn = myGraph[v].myName;
        int x = atoi(sn.substr(1).c_str());
        int y = atoi(sn.substr(sn.find("x") + 1).c_str());
        f << sn
          << " [pos=\""
          << std::to_string(x)
          << ","
          << std::to_string(y)
          << "\"];\n";
    }

    graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(myGraph); ei != ei_end; ++ei)
    {
        auto src = myGraph[source(*ei, myGraph)].myName;
        auto dst = myGraph[target(*ei, myGraph)].myName;
        bool span = false;
        for (auto &se : mySpan)
        {
            if (src == myGraph[se[0]].myName && dst == myGraph[se[1]].myName ||
                src == myGraph[se[1]].myName && dst == myGraph[se[0]].myName)
            {
                span = true;
                break;
            }
        }
        if (all)
        {
            f << src << "--"
              << dst << " ";
            if (span)
                f << "[color=\"red\"]";
            f << ";\n";
        }
        else if (span)
        {
            f << src << "--"
              << dst
              << " [color=\"red\"];\n";
        }
    }
    f << "}\n";
    return f.str();
}
