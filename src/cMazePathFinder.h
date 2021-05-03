#include "cPathFinder.h"

class cMazePathFinder : public cPathFinder
{
public:
    void read(
        const std::string &fname);
private:
    int myColCount;
    int myRowCount;
    std::vector< std::vector< int > > myGrid;

    void readnumeric(
        const std::string &line);

    void grid2graph();
};