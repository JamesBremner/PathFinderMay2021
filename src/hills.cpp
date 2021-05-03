#include "cHillPathFinder.h"

main(int argc, char *argv[])
{
    // introduce ourself
    std::cout << "Hills\n";

    // check command line
    if (argc != 2)
    {
        std::cout << "usage: hills <inputfilename>\n";
        exit(0);
    }

    // construct path finder
    cHillPathFinder finder;

    // read input
    finder.read( argv[1] );

    /* creat graph of orthogonal links between grid points
    * costed by 1 + elevation change squared
    */
    finder.ConstructBoostGraph();

    //std::cout << finder.linksText() << "\n";

    // find optimum path
    finder.path();
    
    std::cout << finder.pathText();
    
}