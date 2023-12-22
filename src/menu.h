#ifndef MENU_H
#define MENU_H


#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <string>
#include <fstream>
#include <sstream>
#include <unordered_set>
#include "graph.h"
#include "dataRepository.h"

class Menu {
private:
    Graph graph = Graph(0);
    DataRepository dataRepository;
    string static const airlinesFilePath;
    string static const airportsFilePath;
    string static const flightsFilePath;
    unsigned static const COLUMN_WIDTH;
    unsigned static const COLUMNS_PER_LINE;

public:
    Menu();

    void extractAirlinesFile();

    void extractAirportsFile();

    void extractFlightsFile();

    void extractFileInfo();

    void initializeMenu();

    unsigned int flightsMenu();

    unsigned int infoMenu();

    void mainMenu();

    static bool checkInput(unsigned int checkLength = 0);

    airlineTable airlineRestrictionsMenu();

    unsigned int airportInfoMenu();

    static void airportDoesntExist();

    static void airlineDoesntExist();

    unsigned int generalInfoMenu();
};

#endif

