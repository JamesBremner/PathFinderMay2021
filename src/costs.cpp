#include "cPathFinder.h"

main(int argc, char *argv[])
{
    // intoduce ourself
    std::cout << "Link Costs, a generic path finder\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: costs <inputfilename>\n";
        exit(0);
    }

    // construct path finder
    cPathFinder finder;

    // read input
    finder.read( argv[1] );

    std::cout << finder.linksText();
    
    // find optimum path
    finder.path();
    
    std::cout << finder.pathText();
    
}