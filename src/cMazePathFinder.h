#include "cPathFinder.h"

class cMazePathFinder : public cPathFinder
{
public:

cMazePathFinder();

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
        enum class eformat
    {
        none,
        art,
        numeric,
        cellcost
    } myFormat;

    void readnumeric(
        const std::string &line);

    void readcellcost(
        const std::string &line);

    void grid2graph();
    void grid2graphcellcost();

    std::vector<int> NodesConnected(int v);
};