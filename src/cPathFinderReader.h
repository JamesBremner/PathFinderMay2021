#include <string>
#include "cPathFinder.h"

/// Read input files into pathfinder
class cPathFinderReader
{
public:
    cPathFinderReader(cPathFinder &finder)
        : myFinder(finder)
    {
    }
    void set(const std::string &fname)
    {
        myfname = fname;
    }

    enum class eFormat
    {
        none,
        not_open,
        costs,
        spans,
        sales,
        hills,
        cams,
        gsingh,
        shaun,
        islands,
        maze_ascii_art,
    };

    /** Open file, determine format and read data into pathfinder
     * @param[in] fname to open
     * @return format
     */
    eFormat open(
        const std::string &fname);

    /** Read input file with costed links 
     * 
     *  @param[in] weights true if required, false if not, default true
     * 
     *  format: https://github.com/JamesBremner/PathFinder/wiki/Costs 
     * 
     * If weights is false, then input lines may have just three columns,
     * and cost of 1 will be assigned to link
     */
    void costs(bool weights = true);

    /// read tree
    std::vector<std::string> singleParentTree();

    /// read cities with locations or links
    void sales();

    /** read orthogonal grid of cell values
     * @return grid of cell values
     */
    std::vector<std::vector<float>>
    orthogonalGrid();

    /// read uncosted links, multiple links from one source on one line
    void multi();

    void shaun();

    void islands();

private:
    cPathFinder &myFinder;
    std::string myfname;
    std::ifstream myFile;

    /** Parse Space Delimited line
 * @param[in] l line from a space delimited text file
 * @return vector of strings containing the columns extracted from line
 */
    std::vector<std::string> ParseSpaceDelimited(
        const std::string &l);

        std::string orthogonalGridNodeName(
            int row, int col );
};