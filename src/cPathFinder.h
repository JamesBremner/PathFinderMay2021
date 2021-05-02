#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

/// edge properties
class cEdge
{
public:
    cEdge()
        : myCost(1)
    {
    }
    int myCost;
};

/// node properties
class cNode
{
public:
    cNode(const std::string &name)
        : myName(name)
    {
    }
    cNode()
        : myName("???")
    {
    }
    std::string myName;
};

/// declaration of boost graph configuration
using namespace boost;
typedef boost::adjacency_list<
    boost::listS,
    boost::vecS,
    boost::directedS,
    cNode,
    cEdge>
    graph_t;

class cPathFinder
{
public:
    // read input file
    void read(
        const std::string &fname);

    void path();

/** Add costed link between two nodes
 * 
 * @param[in] u node index
 * @param[in] v node index
 * @param[in] cost link cost
 * 
 *  if the node do not exist, they will be added
 * Two directed links are added u > v and V > u
 */
    void addLink(
        int u,
        int v,
        float cost);

    // Human readable list of links
    std::string linksText();

    // Human readable path list
    std::string pathText();

private:
    graph_t             myGraph;        // graph
    int                 myStart;        // starting node index
    int                 myEnd;          // ending node index
    std::vector<int>    myPath;         // vector of node indices visited

    /** Parse Space Delimited line
 * @param[in] l line from a space delimited text file
 * @return vector of strings containing the columns extracted from line
 */
    std::vector<std::string> ParseSpaceDelimited(
        const std::string &l);
};