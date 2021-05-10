#include "cPathFinder.h"

class cSalesPathFinder : public cPathFinder
{
public:
    void read(
        const std::string &fname);
    void tsp();
};