# PathFinder Engine
Discover optimum path through a graph of costed links.  The engine can be used directly to find minimum cost path through the graph, or specialized to model particular problems that need to find paths.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/blob/47984f841d164f6b9cb081440c2eddf63e819075/src/cPathFinder.h#L5-L122)

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

## Costs
An application that inputs links, link costs, starting and ending nodes, then uses the PathFinder engine to find optimal path. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Costs)

## GUI
A graphical user interface version of costs.

## Turns
An application that models a robot that can travel either "forwards" or "backwards". The robot moves through a graph whose links may be traversed only forwards, only backwards or in either direction. The robot can turn around only at certain nodes.  Uses the PathFinder engine find the optimal path.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Turns)

## Hills
An application that finds a path through a grid of points with different elevations that minimises the changes in elevation. Uses the PathFinder engine find the optimal path. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Hills)

## Mazes
An application that finds a path through a maze.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Mazes)

## Spans
An application that finds minimum cost links that connect all nodes.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Spans)

## Sales
An application that finds minimum cost path that visits all nodes. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Sales)
