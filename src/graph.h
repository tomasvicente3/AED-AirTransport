#ifndef GRAPH_H_
#define GRAPH_H_

#include <list>
#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>
#include <stack>
#include "airline.h"
#include "airport.h"
#include "dataRepository.h"

using namespace std;

class Graph {
    struct Edge {
        int dest;   // Destination node
        airlineTable airlines; // The airlines whose flights connect the two nodes
    };

    struct Node {
        Airport airport; //The Airport this node represents
        list<Edge> adj; // The list of outgoing edges (to adjacent nodes)
        bool visited;   // As the node been visited on a search?
        list<pair<airlineTable, string>> predecessing_trip; // The node that connected to this node
        int dist;
        int num;
        int low;
    };

    int n;              // Graph size (vertices are numbered from 1 to n)
    vector<Node> nodes; // The list of nodes being represented
    airportMap<int> airportToNode;
    airlineTable airlines;
    vector<int> stack; // stack as vector (so we can use the find() algorithm)
    int idx;

public:
    // Constructor: nr nodes and direction (default: undirected)
    explicit Graph(int nodes);

    // Add edge from source to destination with a certain weight
    void addEdge(int src, int dest, const airlineTable &connectingAirlines);

    void addEdge(int src, int dest, const Airline &airline);

    void addNode(const Airport &airport);

    int dfs_scc(int v);
    int bfsMaxDistance(int v);

    int getN() const;
    int getTotalFlightsAirlineless() const; //total voos ignorando companhias
    int getTotalFlights() const; // total de voos únicos

    void setN(int n);

    const vector<Node> &getNodes() const;

    void setNodes(const vector<Node> &nodes);

    const airportMap<int> &getAirportToNode() const;

    void setAirportToNode(const airportMap<int> &airportToNode);

    const airlineTable &getAirlines() const;

    void setAirlines(const airlineTable &airlines);

    unsigned numFlights(const Airport &airport) const;

    int countSCCs();
    
    int getDiameter();

    int findAirportNode(const string &code);

    unsigned int numAirlines(const Airport &airport) const;

    unsigned int numDestinations(const Airport &airport) const;

    unsigned int numCountries(const Airport &airport) const;

    airlineTable intersectTables(const airlineTable &table1, const airlineTable &table2);

    void bfsDistance(int v);

    unsigned int numAirportsInXFlights(const Airport &airport, unsigned int numFlights);

    unsigned int numCitiesInXFlights(const Airport &airport, unsigned numFlights);

    unsigned int numCountriesInXFlights(const Airport &airport, unsigned int numFlights);

    list<pair<airlineTable, string>> shortest_path_bfs(list<int> source, int destination, airlineTable validAirlines);

    list<list<pair<airlineTable, string>>>
    getShortestPath(const list<Airport> &source, const list<Airport> &target, airlineTable validAirlines);
};

#endif
