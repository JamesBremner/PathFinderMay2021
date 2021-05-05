#include "cPathFinder.h"

class cMazePathFinder : public cPathFinder
{
public:

/** read input file
 * 
 * @param[in] fname
 * 
 * Accepts maze in ascii art or numeric form
 * 
 */
    void read(
        const std::string &fname);

///  Maze with path in ascii art format
    std::string
    asciiArtText();

private:
    int myColCount;
    int myRowCount;
    std::vector<std::vector<int>> myGrid;

    void readnumeric(
        const std::string &line);

    void grid2graph();

    std::vector<int> NodesConnected(int v);
};