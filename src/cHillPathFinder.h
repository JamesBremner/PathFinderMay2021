#include "cPathFinder.h"

class cHillPathFinder : public cPathFinder
{
public:
    void read(
        const std::string &fname);

    void ConstructBoostGraph();

private:
    int myColCount;
    int myRowCount;
    std::vector<std::vector<float>> myGrid;
    int myStartCol;
    int myStartRow;
    int myEndCol;
    int myEndRow;

    std::string name(int col, int row);
};