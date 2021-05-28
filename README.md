# PathFinder Engine
Discover optimum path through a graph of costed links.  The engine can be used directly to find minimum cost path through the graph, or specialized to model particular problems that need to find paths.  [Detailed Documentation](https://jamesbremner.github.io/PathFinder/classc_path_finder.html)

# Example code

```C++
   cPathFinder f;
   
   // add links
   f.addLink(
       f.findoradd("A"),
       f.findoradd("B"),
       costAB );
    ... add rest of links
    
    // specify path start and end
    f.start(f.find("A"));
    f.end(f.find("B"));
    
    // find the path
    f.path();
    
    // display the path
    std::cout << f.pathText() << "\n";
```
# Applications

Applications that model certain problems.

## GUI
A graphical user interface to PathFinder.  The following options are available:

- __Costs.__
An option that inputs links, link costs, starting and ending nodes, then uses the PathFinder engine to find optimal path. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Costs)

- __Sales.__
An option that finds minimum cost path that visits all nodes. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Sales)

- __Prerequisites__
An option that finds paths through ordered requirements. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Prerequisites)

- __Spans__
An option that finds minimum cost links that connect all nodes.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Spans)

- __Cams__
An option that finds a set of nodes that cover every link. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Cams)

- __Hills__
An option that finds a path through a grid of points with different elevations that minimises the changes in elevation. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Hills)

## Turns
An application that models a robot that can travel either "forwards" or "backwards". The robot moves through a graph whose links may be traversed only forwards, only backwards or in either direction. The robot can turn around only at certain nodes.  Uses the PathFinder engine find the optimal path.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Turns)

## Mazes
An application that finds a path through a maze.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Mazes)

