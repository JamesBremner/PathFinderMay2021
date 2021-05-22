#include <string>
#include "cPathFinder.h"

/// Read input files into pathfinder
class cPathFinderReader
{
    public:
    cPathFinderReader( cPathFinder& finder )
    : myFinder( finder )
    {
    }
    void set( const std::string& fname )
    {
        myfname = fname;
    }

/// Read input file with costed links ( format: https://github.com/JamesBremner/PathFinder/wiki/Costs )
    void costs();

/** Parse Space Delimited line
 * @param[in] l line from a space delimited text file
 * @return vector of strings containing the columns extracted from line
 */
    std::vector<std::string> ParseSpaceDelimited(
        const std::string &l);

    private:
    
    cPathFinder& myFinder;
    std::string myfname;
};