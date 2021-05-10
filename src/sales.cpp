#include <limits>
#include "cSalesPathFinder.h"

// construct path finder
cSalesPathFinder finder;


main(int argc, char *argv[])
{
    // intoduce ourself
    std::cout << "Sales\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: sales <inputfilename>\n";
        exit(0);
    }

    // read input
    finder.read(argv[1]);

    std::cout << finder.linksText();

    // find optimum path
    finder.tsp();
}