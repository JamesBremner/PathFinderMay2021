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
 * use findoradd( std::string& name )
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

    void clear();
    
    /// set staring node
    void start(int start)
    {
        myStart = start;
    }
    void start( const std::string& start )
    {
        myStart = find( start );
    }
    int start() const
    {
        return myStart;
    }

    /// set ending node
    void end(int end)
    {
        myEnd = end;
    }
    void end( const std::string& end )
    {
        myEnd = find( end );
    }
    /** Find optimum path from start to end node
 *
 * The path from attributes myStart to myEnd is
 * is saved into myPath
 * The path can be listed by call to pathText()
 */
    void path();

    /** Find paths from start to all nodes
 */
    void paths(int start);

    /** Find path to end node, after call to paths( int start )
     * 
     * @param[in] end index of end vertex
     * 
     * The path is stored in attribute myPath.
     * The path can be listed by call to pathText()
     * 
     * If no path exists, an exception is thrown.
 */
    void pathPick(int end);

    /** Find minimum edge set that connects all nodes together
 */
    void span();

    /// Human readable list of links
    std::string linksText();

    /// Human readable path list
    std::string pathText();

    /// Human readable list of edges that connects all nodes together
    std::string spanText();

    /** graphical display of graph with path in red
     * 
     * @return display in graphviz dot format
     * 
     * render at https://dreampuf.github.io/GraphvizOnline
     * 
     * or install graphviz and use command
     * 
     * dot -Kfdp -n -Tpng -o sample.png sample.dot
     * 
     */
    std::string pathViz();

    std::string pathViz(
        const std::vector<int> &vp,
        bool all = true);

    /** graphical display of graph with spanning tree
     * 
     * @param[in] all true to see all links, spanning tree in red
     * @param[in] all false to see spanning tree
     * @return display in graphviz dot format
     */
    std::string spanViz(bool all = true);

    /** Find or add node by name
 * 
 * @param[in] name
 * @return node index
 * 
 * If a node of specified nane does not exist, it is added.
 */
    int findoradd(const std::string &name);

    /** Find node by name
 * 
 * @param[in] name
 * @return node index, -1 if named node does not exist
 */
    int find(const std::string &name);

    /** Parse Space Delimited line
 * @param[in] l line from a space delimited text file
 * @return vector of strings containing the columns extracted from line
 */
    std::vector<std::string> ParseSpaceDelimited(
        const std::string &l);

    bool IsAdjacent(int u, int v);          /// true if link between nodes
    bool IsConnected();                     /// true if all nodes are connected together

    int nodeCount()
    {
        return num_vertices(myGraph);
    }
    int linkCount()
    {
        return num_edges( myGraph);
    }

    std::string nodeColor( int n );
    std::string nodeName( int n );

    /** set graph links type
     * 
     * @param[in] f true for directed, false for undirected, default directed
     * 
     * If not called, the graph will be undirected
     * 
     */
    void directed(bool f = true)
    {
        myfDirected = f;
    }

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
        std::string myColor;
    };

    typedef boost::adjacency_list<
        boost::listS,
        boost::vecS,
        boost::undirectedS,
        cNode,
        cEdge>
        graph_t;

    graph_t myGraph; // graph

    typedef boost::adjacency_list<
        boost::listS,
        boost::vecS,
        boost::directedS,
        cNode,
        cEdge>
        dir_graph_t;

    dir_graph_t myDirGraph; // graph

    int myStart;                          // starting node index
    int myEnd;                            // ending node index
    std::vector<int> myPath;              // vector of node indices visited
    std::vector<int> myDist;              // cost to reach each node from start
    std::vector<int> myPred;              // previous node to each node from start
    std::vector<std::vector<int>> mySpan; // edges in minimum spanning tree
    int mySpanCost;                       // total cost of links in spanning tree
    bool myfDirected;                     // true if links are directed



    template <typename T>
    std::string linksTextT(T &g);

    template <typename T>
    void pathsT(int start, T &g);
};