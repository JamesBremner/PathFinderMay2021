#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

/// general purpose path finder
class cPathFinder
{
public:

    /** read input file
 *
 * @param[in] fname path to input file.
 *
 * The input file is a space delimited text files
 * specifying the indices of linked nodes, the cost of the links
 * and indices of the starting and ending nodes.
 *
 * File format is specified at https://github.com/JamesBremner/PathFinder/wiki/Costs
 *
 * Specializations with their own file formats should override this method
 *
 */
    void read(
        const std::string &fname);

    /** Add costed link between two nodes
 *
 * @param[in] u node index
 * @param[in] v node index
 * @param[in] cost link cost
 *
 * If the nodes do not exist, they will be added.
 *
 * Two directed links are added u > v and V > u
 */
    void addLink(
        int u,
        int v,
        float cost);

    /** Find optimum path from start to end node
 *
 * The path from attributes myStart to myEnd is
 * is saved into myPath
 * The path can be listed by call to pathText();
 */
    void path();

    /** Find paths from start to all nodes
 */
    void paths(int start);

    /** Find path to end node, after call to paths( int start )
 */
    void pathPick(int end);

    /// Human readable list of links
    std::string linksText();

    /// Human readable path list
    std::string pathText();

protected:
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

    typedef boost::adjacency_list<
        boost::listS,
        boost::vecS,
        boost::directedS,
        cNode,
        cEdge>
        graph_t;

    graph_t myGraph;         // graph
    int myStart;             // starting node index
    int myEnd;               // ending node index
    std::vector<int> myPath; // vector of node indices visited
    std::vector<int> myDist; // cost to reach each node from start
    std::vector<int> myPred; // previous node to each node from start

    /** Parse Space Delimited line
 * @param[in] l line from a space delimited text file
 * @return vector of strings containing the columns extracted from line
 */
    std::vector<std::string> ParseSpaceDelimited(
        const std::string &l);

    int findoradd(const std::string &name);
    int find(const std::string &name);
};