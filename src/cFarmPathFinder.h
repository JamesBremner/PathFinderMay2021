#include "cPathFinder.h"
#include "geo.h"

class cFarmPathFinder : public cPathFinder
{
public:
    /// read input file
    void read(
        const std::string &fname);

private:
    geo::cPolygon   myField;
    float           myRowAngle;
    float           myRowSep;
    geo::cxy        myRef;

    void ConstructRows();
};