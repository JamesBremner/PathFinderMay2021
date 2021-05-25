#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/metric_tsp_approx.hpp>
#include "cPathFinder.h"
using namespace boost;

void cPathFinder::clear()
{
    myGraph.clear();
    myDirGraph.clear();
    myMaxNegCost = 0;
}

void cPathFinder::start(int start)
{
    myStart = start;
}
void cPathFinder::start(const std::string &start)
{
    myStart = find(start);
    if (myStart < 0)
        throw std::runtime_error("cPathFinder::bad start node");
}
int cPathFinder::start() const
{
    return myStart;
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
    if (myStart < 0)
        throw std::runtime_error("cPathFinder::path start node undefined");

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
            std::cout << nodeName(myStart) << " to "
                      << nodeName(kv) << " "
                      << myPath.size() - 1 << "\n";
        }
    }
}

int cPathFinder::distance(int end)
{
    if (0 > end || end >> (int)myDist.size())
        return -1;
    return myDist[end];
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

std::vector<int> cPathFinder::pathPick(int end)
{
    myPath.clear();
    // std::cout << "->cPathFinder::pathPick "
    //     << myStart <<" " << end << "\n";

    if (end < 0)
        throw std::runtime_error("cPathFinder::pathPick bad end node");
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

    return myPath;
}

void cPathFinder::tsp()
{
    double len = 0; //length of the tour
    auto tour_visitor = make_tsp_tour_len_visitor(
        myGraph,
        std::back_inserter(myPath),
        len,
        get(&cEdge::myCost, myGraph));
    metric_tsp_approx(
        myGraph,
        get(&cEdge::myCost, myGraph),
        get(vertex_index, myGraph),
        tour_visitor);
    // metric_tsp_approx_from_vertex(
    //     myGraph,
    //     myStart,
    //     get(&cEdge::myCost, myGraph),
    //     get(vertex_index, myGraph),
    //     tour_visitor);
}

void cPathFinder::addLink(
    int u,
    int v,
    float cost)
{
    if (u < 0 || v < 0)
        throw std::runtime_error("cPathFinder::addLink bad node");
    if (!myfDirected)
        myGraph[add_edge(u, v, myGraph).first].myCost = cost;
    else
        myDirGraph[add_edge(u, v, myDirGraph).first].myCost = cost;
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

int cPathFinder::nodeCount()
{
    if (!myfDirected)
        return num_vertices(myGraph);
    else
        return num_vertices(myDirGraph);
}
int cPathFinder::linkCount()
{
    if (!myfDirected)
        return num_edges(myGraph);
    else
        return num_edges(myDirGraph);
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

bool cPathFinder::IsAdjacent(int u, int v)
{
    if (u < 0 || v < 0)
        return false;
    return edge(u, v, myGraph).second;
}

bool cPathFinder::IsConnected()
{
    std::vector<int> component(boost::num_vertices(myGraph));
    return (1 == boost::connected_components(myGraph, &component[0]));
}

std::string cPathFinder::nodeName(int n)
{
    std::string sn = myGraph[n].myName;
    if (sn == "???")
        sn = std::to_string(n);
    return sn;
}

std::string cPathFinder::nodeColor(int n)
{
    return myGraph[n].myColor;
}
std::string cPathFinder::pathText()
{
    std::stringstream ss;
    for (auto n : myPath)
    {
        std::string sn = myGraph[n].myName;
        if (sn == "???")
            sn = std::to_string(n);
        ss << sn << " -> ";
        //ss << std::to_string(n) << " -> ";
    }

    if (myPath.size())
    {
        std::cout << "dbg " << myDist[myPath.back()] << " " << myMaxNegCost << " " << myPath.size() << "\n";
        ss << " Cost is "
           << myDist[myPath.back()] + myMaxNegCost * (myPath.size() - 1)
           << "\n";
    }

    return ss.str();
}

std::string cPathFinder::spanText()
{
    // std::cout << "spanText " << mySpan.size()
    //           << " cost " << mySpanCost << "\n";

    std::stringstream ss;
    for (auto e : mySpan)
    {
        ss << nodeName(e[0])
           << " - "
           << nodeName(e[1])
           << ", ";
    }
    ss << " cost " << mySpanCost << "\n";
    return ss.str();
}

void cPathFinder::negCost(int cost)
{
    graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(myGraph); ei != ei_end; ++ei)
    {
        myGraph[*ei].myCost -= cost;
    }
    myMaxNegCost = cost;
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
    {
        f << myGraph[v].myName
          << " [color=\"" << myGraph[v].myColor << "\"  penwidth = 3.0 ];\n";
    }

    // loop over links
    graph_traits<graph_t>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(myGraph); ei != ei_end; ++ei)
    {
        // check if link between two nodes on path
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

void cPathFinder::cams()
{
    //     APPROXIMATION-VERTEX-COVER(G)=
    // C = ∅
    // E'= G.E

    // while E' ≠ ∅:
    //     let (u, v) be an arbitrary edge of E'
    //     C = C ∪ {u, v}
    //     remove from E' every edge incident on either u or v

    // remove all leaf nodes from C
    // return C

    // store indices of nodes that cover links
    std::set< int > setCover;

    // graph of links between covering nodes
    graph_t cover;

    // working copy on input graph
    auto work = myGraph;

    myPath.clear();

    graph_traits<graph_t>::out_edge_iterator ei, ei_end;
    
    // loop until all links are covered
    while (num_edges(work))
    {
        // select first link in working graph
        auto it = edges(work).first;
        int u = source(*it, work);
        int v = target(*it, work);
        //std::cout << u << " - " << v << "\n"; 

        // add nodes on selected link to cover
        add_edge(u, v, cover);
        setCover.insert( u );
        setCover.insert( v );

        // remove all links that can be seen from new cover nodes
        for (boost::tie(ei, ei_end) = out_edges(u, work); ei != ei_end; ++ei)
        {
            remove_edge(*ei, work);
        }
        for (boost::tie(ei, ei_end) = out_edges(v, work); ei != ei_end; ++ei)
        {
            remove_edge(*ei, work);
        }
    }

    std::cout << "nodes that cover all vertices:\n";
    for( int u : setCover ) {
        // exclude edge nodes, their link will be covered from other end
        if( out_degree( u, myGraph ) > 1 ) {
            std::cout << u << " ";
            myGraph[ u ].myColor = "red";
        }
    }
    std::cout << "\n";
}
