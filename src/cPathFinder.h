#include <iostream>
#include <fstream>
#include <boost/graph/adjacency_list.hpp>

/** general purpose path finder
 * 
 **** Usage: Node indices
 * 
 * Use addLink( int u, int v, float cost )
 * to add a costed link between nodes referred by their index
 * 
 * Use start() and end()
 * to specify starting and ending node indices
 * 
 * Use path() 
 * to calculate optimim path from start to end
 * 
 * Use pathText()
 * to get a string listing the node indices visited
 * 
 **** Usage: Node names
 *
 * To refer to nodes by name,
 * rather than keeping track of their indices,
 * use findoradd( std:;string& name )
 * PathText() will return a list of nodes visited by their name
 * 
 * <pre>
 
   cPathFinder f;
   f.addLink(
       f.findoradd("A"),
       f.findoradd("B"),
       cost );
    ...
    f.start(f.find("A"));
    f.end(f.find("B"));
    f.path();
    std::cout << f.pathText() << "\n";
  </pre>
 * 
 */
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
 * and can use the protected function ParseSpaceDelimited() to assist parsing.
 *
 */
    void read(
        const std::string &fname);

    /** Add costed undirected link between two nodes
 *
 * @param[in] u node index
 * @param[in] v node index
 * @param[in] cost link cost
 *
 * If the nodes do not exist, they will be added.
 *
 */
    void addLink(
        int u,
        int v,
        float cost);

    void start(int start)
    {
        myStart = start;
    }
    void end(int end)
    {
        myEnd = end;
    }

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

    /** Find or add node by name
 * 
 * @param[in] name
 * @return node index
 * 
 * If a node of specified nane does not exist, it is added.
 */
    int findoradd(const std::string &name);

    /** Find or add node by name
 * 
 * @param[in] name
 * @return node index, -1 if named node does not exist
 */
    int find(const std::string &name);

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
        boost::undirectedS,
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
};