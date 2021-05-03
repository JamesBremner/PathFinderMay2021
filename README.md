# PathFinder Engine
Discover optimum path through a graph of costed links.  The engine can be used directly to find minimum cost path through the graph, or specialized to model particular problems that need to find paths.  The following lists available applications that model certain problems.

# Costs
An application that inputs links, link costs, starting and ending nodes, then uses the PathFinder engine to find optimal path. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Costs)

# Turns
An application that models a robot that can travel either "forwards" or "backwards". The robot moves through a graph whose links may be traversed only forwards, only backwards or in either direction. The robot can turn around only at certain nodes.  Uses the PathFinder engine find the optimal path.  [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Turns)

# Hills
An application that finds a path through a grid of points with different elevations that minimises the changes in elevation. Uses the PathFinder engine find the optimal path. [Detailed Documentation](https://github.com/JamesBremner/PathFinder/wiki/Hills)

# Mazes
An application that finds a path through a maze.
