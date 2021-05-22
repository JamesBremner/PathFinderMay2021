#include "cPathFinderReader.h"

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
    cPathFinderReader reader( finder );
    reader.set( argv[1] );
    reader.costs();

    std::cout << finder.linksText();
    
    // find optimum path
    finder.path();
    
    std::cout << finder.pathText();

    std::ofstream f("g.dot");
    f << finder.pathViz();
    
}