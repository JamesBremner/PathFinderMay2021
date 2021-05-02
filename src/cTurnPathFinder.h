#include "cPathFinder.h"

/// declaration of structure holding link input data
struct sDirLink
{
    int src;
    int dst;
    int dir; // allowed orientations of robot on link
    float cost;
};

class cTurnPathFinder : public cPathFinder
{
public:

/// read input file
    void read(
        const std::string &fname);

/** Split links according to allowed direction of travel
 * @param[in] vBiLink the input links
 * @param[out] vForward links allowing forward travel
 * @param[out] vBack links allowing backward travel
 * @param[out] vTurn modes where robot can turn around
 */
    void split();

/// Create boost graph from combination of forward, backward and turning links
    void ConstructBoostGraph();

    void tpath();

private:
    std::vector< sDirLink > myInputLink;
    std::vector<sDirLink> myForward;
    std::vector<sDirLink> myBack;
    std::vector<sDirLink> myTurn;
    int myStartDirection;
};