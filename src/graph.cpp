#include "graph.h"
#include <algorithm>

using namespace std;

// Constructor: nr nodes
Graph::Graph(int num) : n(num), nodes(num + 1) {
}

/**
 * Adds an edge to the graph
 * Time Complexity: O(1)
 * @param src - Number of the source node
 * @param dest - Number of the destination node
 * @param connectingAirlines - List of Airlines whose flights connect the two nodes (Airports)
 */
void Graph::addEdge(int src, int dest, const airlineTable &connectingAirlines) {
    if (src < 1 || src > n || dest < 1 || dest > n) return;
    nodes[src].adj.push_back({dest, connectingAirlines});
}

/**
 * Adds an edge to the graph
 * Time Complexity: O(outdegree(src))
 * @param src - Number of the source node
 * @param dest - Number of the destination node
 * @param airline - Pointer to an Airlines whose flight connects the two nodes (Airports)
 */
void Graph::addEdge(int src, int dest, const Airline &airline) {
    if (src < 1 || src > n || dest < 1 || dest > n) return;

    auto existingEdgeIt = std::find_if(nodes[src].adj.begin(), nodes[src].adj.end(),
                                       [dest](Edge e) { return e.dest == dest; });
    if (existingEdgeIt != nodes[src].adj.end()) {
        existingEdgeIt->airlines.insert(airline);
    } else nodes[src].adj.push_back({dest, airlineTable({airline})});
}

/**
 * Adds a new node to the graph
 * Time Complexity: O(n) (worst case) | O(1) (average case)
 * @param airport - Airport the new node will represent
 */
void Graph::addNode(const Airport &airport) {
    nodes.push_back({airport});
    airportToNode[airport] = ++n;
}

int Graph::getN() const {
    return n;
}

void Graph::setN(int n) {
    Graph::n = n;
}

const vector<Graph::Node> &Graph::getNodes() const {
    return nodes;
}

void Graph::setNodes(const vector<Node> &nodes) {
    Graph::nodes = nodes;
}

/**
 * Returns the index of the node representing the airport with the given code
 * @param code - Code of the Airport whose node index should be found
 * @return Index of the node representing the given airport, or 0 if no node represents it
 */
int Graph::findAirportNode(const string &code) {
    auto it = airportToNode.find(Airport(code));
    return it != airportToNode.end() ? it->second : 0;
}

const airportMap<int> &Graph::getAirportToNode() const {
    return airportToNode;
}

void Graph::setAirportToNode(const airportMap<int> &airportToNode) {
    Graph::airportToNode = airportToNode;
}

const airlineTable &Graph::getAirlines() const {
    return airlines;
}

void Graph::setAirlines(const airlineTable &airlines) {
    Graph::airlines = airlines;
}

/**
 * Computes the number of flights that leave from a given airport
 * Time Complexity: O(outdegree(v)), where v is the node associated with the given Airport
 * @param airport - Airport whose number of flights should be calculated
 * @return Number of flights leaving from given Airport
 */
unsigned Graph::numFlights(const Airport &airport) const {
    unsigned total = 0;
    int v = airportToNode.at(airport);
    for (const Edge &e: nodes[v].adj) {
        total += e.airlines.size();
    }
    return total;
}


/**
 * Finds one of the routes connecting one of the source nodes to the destination node that has the minimum amount of flights, avoiding invalid Edges.
 * Time Complexity: O(|V|+|E| * n * m) (worst case) | O(|V|+|E| * n) (average case), where n is the size of validAirlines and m is the largest number of airlines on an Edge
 * 
 * @param source - Index of the source node
 * @param destination - Index of the destination node
 * @param validAirlines - unordered_set of Airlines that are valid
 * @return A list of pair<airlineTable, string>, each representing the airlines that connected the previous pair to this one, and the code of the connected Airport
*/
list<pair<airlineTable, string>>
Graph::shortest_path_bfs(list<int> source, int destination, airlineTable validAirlines) {
    if (std::find(source.begin(), source.end(), destination) != source.end()) return {};

    for (int i = 1; i <= n; i++) {
        nodes[i].visited = false;
        nodes[i].dist = -1;
        nodes[i].predecessing_trip = {};
    }
    queue<int> q; // queue of unvisited nodes
    for (int i: source) {
        q.push(i);
        nodes[i].visited = true;
        nodes[i].dist = 0;
        nodes[i].predecessing_trip = {{{}, nodes[i].airport.getCode()}};
    }

    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front();
        q.pop();
        // show node order
        //cout << u << " ";
        for (auto e: nodes[u].adj) {
            int w = e.dest;
            airlineTable available_airlines = intersectTables(validAirlines, e.airlines);
            if (!nodes[w].visited && !available_airlines.empty()) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + 1;
                nodes[w].predecessing_trip = nodes[u].predecessing_trip;
                nodes[w].predecessing_trip.push_back({available_airlines, nodes[w].airport.getCode()});
            }
            if (w == destination) break;
        }
    }
    return nodes[destination].predecessing_trip;
}

/**
 * Computes the number of Airlines that carry flights leaving from a given Airport
 * Time Complexity: O(outdegree(v) * N² + N) (worst case) | O(outdegree(v) * N) (average case), where N is the number of different airlines carrying flights from the given Airport and v is the node associated with the given Airport
 * @param airport - Airport whose number of Airlines should be calculated
 * @return Number of Airlines carrying flights leaving from given Airport
 */
unsigned Graph::numAirlines(const Airport &airport) const {
    airlineTable currentAirlines;
    int v = airportToNode.at(airport);
    for (Edge e: nodes[v].adj) {
        currentAirlines.merge(e.airlines);
    }
    return currentAirlines.size();
}

/**
 * Computes the number of cities reachable in a flight from a given Airport
 * Time Complexity: O(outdegree(v) * N) (worst case) | O(outdegree(v)) (average case), where N is the number of different cities directly reachable from the given Airport and v is the node associated with the given Airport
 * @param airport - Airport whose number of destinations should be calculated
 * @return Number of different cities reachable in direct flights from the given Airport
 */
unsigned Graph::numDestinations(const Airport &airport) const {
    cityTable currentCities;
    int v = airportToNode.at(airport);
    for (const Edge &e: nodes[v].adj) {
        int w = e.dest;
        currentCities.insert({nodes[w].airport.getCity(), nodes[w].airport.getCountry()});
    }
    return currentCities.size();
}

/**
 * Computes the number of countries reachable in a flight from a given Airport
 * Time Complexity: O(outdegree(v) * N) (worst case) | O(outdegree(v)) (average case), where N is the number of different countries directly reachable from the given Airport and v is the node associated to the given Airport
 * @param airport - Airport whose number of destination countries should be calculated
 * @return Number of different countries reachable in direct flights from the given Airport
 */
unsigned Graph::numCountries(const Airport &airport) const {
    unordered_set<string> currentCountries;
    int v = airportToNode.at(airport);
    for (const Edge &e: nodes[v].adj) {
        int w = e.dest;
        currentCountries.insert(nodes[w].airport.getCountry());
    }
    return currentCountries.size();
}

/**
 * Intersects two unordered_set<Airline>
 * Time Complexity: O(n * m) (worst case) | O(n) (average case), where n is the size of the first table and m the size of the second table
 * @param table1 - First table
 * @param table2 - Second table
 * @return Intersection between the two given airline tables
 */
airlineTable Graph::intersectTables(const airlineTable &table1, const airlineTable &table2) {
    airlineTable intersection;
    for (auto i = begin(table1); i != end(table1); i++) {
        auto table2It = table2.find(*i);
        if (table2It != end(table2)) {
            intersection.insert(*table2It);
        }
    }
    return intersection;
}

/**
 * BFS function that visits the graph and sets the distance variable for all nodes
 * Time Complexity: O(|V| +|E|)
 * @param v - Root node of the BFS
 */
void Graph::bfsDistance(int v) {
    for (int i = 1; i <= n; i++) {
        nodes[i].visited = false;
        nodes[i].dist = -1;
    }
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    nodes[v].dist = 0;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front();
        q.pop();
        // show node order
        //cout << u << " ";
        for (auto e: nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + 1;
            }
        }
    }
}

/**
 * Computes the number of airports reachable from the given Airport in less than x flights
 * Time Complexity: O(|V| + |E|)
 * @param airport - Source Airport
 * @param numFlights - Max number of flights
 * @return Number of airports reachable from the given Airport in less or numFlights flights
 */
unsigned Graph::numAirportsInXFlights(const Airport &airport, unsigned numFlights) {
    unsigned total = 0;
    int v = airportToNode.at(airport);
    bfsDistance(v);
    for (int i = 1; i <= n; i++) {
        if (nodes[i].dist <= numFlights) total++;
    }
    return total - 1; //Excluding the airport itself
}

/**
 * Computes the number of cities reachable from the given Airport in less than x flights
 * Time Complexity: O(|V|²) (worst case) | O(|V| + |E|) (average case)
 * @param airport - Source Airport
 * @param numFlights - Max number of flights
 * @return Number of cities reachable from the given Airport in less or numFlights flights
 */
unsigned Graph::numCitiesInXFlights(const Airport &airport, unsigned numFlights) {
    cityTable currentCities;
    int v = airportToNode.at(airport);
    bfsDistance(v);
    for (int i = 1; i <= n; i++) {
        if (nodes[i].dist <= numFlights)
            currentCities.insert({nodes[i].airport.getCity(), nodes[i].airport.getCountry()});
    }
    return currentCities.size() - 1; //Excluding the airport itself
}

/**
 * Computes the number of countries reachable from the given Airport in less than x flights
 * Time Complexity: O(|V|²) (worst case) | O(|V| + |E|) (average case)
 * @param airport - Source Airport
 * @param numFlights - Max number of flights
 * @return Number of countries reachable from the given Airport in less or numFlights flights
 */
unsigned Graph::numCountriesInXFlights(const Airport &airport, unsigned numFlights) {
    unordered_set<string> currentCountries;
    int v = airportToNode.at(airport);
    bfsDistance(v);
    for (int i = 1; i <= n; i++) {
        if (nodes[i].dist <= numFlights) currentCountries.insert(nodes[i].airport.getCountry());
    }
    return currentCountries.size() - 1; //Excluding the airport itself
}

/**
 * Basic BFS algorithm adapted to register the distance from the starting node in its atributes
 * Time Complexity: O(|V+E|)
 * @param v - Index of the node node from where the search begins
 */
int Graph::bfsMaxDistance(int v) {
    for (int i = 1; i <= n; i++) {
        nodes[i].visited = false;
        nodes[i].dist = -1;
    }
    int maxDistance = 0;
    queue<int> q; // queue of unvisited nodes
    q.push(v);
    nodes[v].visited = true;
    nodes[v].dist = 0;
    while (!q.empty()) { // while there are still unvisited nodes
        int u = q.front();
        q.pop();
        // show node order
        //cout << u << " ";
        for (auto e: nodes[u].adj) {
            int w = e.dest;
            if (!nodes[w].visited) {
                q.push(w);
                nodes[w].visited = true;
                nodes[w].dist = nodes[u].dist + 1;
                if (nodes[w].dist > maxDistance) maxDistance = nodes[w].dist;
            }
        }
    }
    return maxDistance;
}

/**
 * Calculates the diameter of the graph composed by the airports
 * Time Complexity: O(|V|(|V+E|))
 */
int Graph::getDiameter() {
    int maxDistance = -1;
    for (int i = 1; i <= n; i++) {
        int currentDistance = bfsMaxDistance(i);
        if(currentDistance > maxDistance) maxDistance = currentDistance;
    }
    return maxDistance;
}

/**
 * Computes the amount of flights that exist, ignoring their airlines.
 * Time Complexity: O(|V|)
 * @return Number of total flights
 */
int Graph::getTotalFlightsAirlineless() const{
    int nFlights = 0;
    for (int i = 1; i <= n; i++){
        nFlights += nodes[i].adj.size();
    }
    return nFlights;
}

/**
 * Computes the amount of unique flights that exist, considering the airlines.
 * Time Complexity: O(|V*E|)
 * @return Number of total flights
 */

int Graph::getTotalFlights() const{
    int nFlights = 0;
    for (int i = 1; i<= n; i++){
        for (Edge e : nodes[i].adj) nFlights += e.airlines.size();
    }
    return nFlights;
}


/**
 * Computes a list of the shortest paths (not exhaustive) connecting the source airports to the target airports, using only airlines in validAirlines
 * Time Complexity: O(|V|+|E| * n * m * k) (worst case) | O(|V|+|E| * n * k) (average case), where n is the size of validAirlines, m is the largest number of airlines on an Edge, and k the size of target
 * @param source - List of source Airports
 * @param target - List of target Airports
 * @param validAirlines - unordered_set of Airlines that are valid
 * @return A list of the shortest paths, where paths are a list of pair<airlineTable, string>, each representing the airlines that connected the previous pair to this one, and the code of the connected Airport
 */
list<list<pair<airlineTable, string>>>
Graph::getShortestPath(const list<Airport> &source, const list<Airport> &target, airlineTable validAirlines) {
    list<int> listSource, listDest;
    list<list<pair<airlineTable, string>>> shortestPaths;

    for (const Airport &airport: source) { listSource.push_back(airportToNode[airport]); }

    for (const Airport &airport: target) {
        auto currentPath = shortest_path_bfs(listSource, airportToNode[airport], validAirlines);
        if (shortestPaths.size() == 0 || currentPath.size() == shortestPaths.front().size())
            shortestPaths.push_back(currentPath);
        else if (currentPath.size() < shortestPaths.front().size()) shortestPaths = {currentPath};
    }
    return shortestPaths;
}


/**
 * Depth-First Search Algorithm variation that returns the ammounts of strongly connected components starting on a certain airport
 * Time Complexity: O(|V+E|)
 * @param v - Index of the airport where the search beggings
 */
int Graph::dfs_scc(int v){
    nodes[v].num = this -> idx;
    nodes[v].low = this -> idx;
    this->idx++;
    stack.push_back(v);
    int acc = 0;

    for (Edge e : nodes[v].adj){
        int w = e.dest;
        if (nodes[w].num == -1){
            dfs_scc(w);
            if (nodes[w].low < nodes[v].low) nodes[v].low = nodes[w].low;
        }
        else if (find(stack.begin(), stack.end(),w)!=stack.end()){
            if (nodes[w].num < nodes[v].low) nodes[v].low = nodes[w].num;
        }
    }

    if (nodes[v].num == nodes[v].low){
        acc++;
        int w;
        do{
            w = stack.back();
            stack.pop_back();
        } while (w != v);
    }
    return acc;
}

/**
 * Computes how many Strongly Connected Components are in the graph composed by the airports
 * Time Complexity: O(|V|*|V+E|)
 */
int Graph::countSCCs(){
    this-> idx = 1;
    vector<int> temp;
    this->stack = temp;
    int acc = 0;

    for (int i = 1; i <= n; i++){
        nodes[i].num = -1;
        nodes[i].low = -1;
        nodes[i].visited = false;
    }

    for (int i = 1; i <= n; i++){
        if (nodes[i].num == -1){
            acc += dfs_scc(i);
        }
    }
    return acc;
}
