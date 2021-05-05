#include "cPathFinder.h"

main(int argc, char *argv[])
{
    // intoduce ourself
    std::cout << "Spans\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: spans <inputfilename>\n";
        exit(0);
    }

    // construct path finder
    cPathFinder finder;

    // read input
    finder.read( argv[1] );

    std::cout << finder.linksText();
    
    // find optimum path
    finder.span();
    
    std::cout << finder.spanText();
     
}