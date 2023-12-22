#include "menu.h"

using namespace std;

unsigned const Menu::COLUMN_WIDTH = 45;
unsigned const Menu::COLUMNS_PER_LINE = 3;
string const Menu::airlinesFilePath = "../dataset/airlines.csv";
string const Menu::airportsFilePath = "../dataset/airports.csv";
string const Menu::flightsFilePath = "../dataset/flights.csv";

Menu::Menu() = default;

/**
 * Delegates extracting file info, calling the appropriate functions for each file
 */
void
Menu::extractFileInfo() {
    extractAirlinesFile();
    extractAirportsFile();
    extractFlightsFile();
}

/**
 * Extracts and stores the information of airlines.csv
 * Time Complexity: O(n²) (worst case) | 0(n) (average case), where n is the number of lines of airlines.csv
 */
void Menu::extractAirlinesFile() {

    ifstream airlines(airlinesFilePath);

    string currentParam, currentLine;
    string code, name, callsign, country;

    int counter = 0;

    getline(airlines, currentParam); //Ignore first line with just descriptors

    while (getline(airlines, currentLine)) {
        istringstream iss(currentLine);
        while (getline(iss, currentParam, ',')) {
            switch (counter++) {
                case 0: {
                    code = currentParam;
                    break;
                }
                case 1: {
                    name = currentParam;
                    break;
                }
                case 2: {
                    callsign = currentParam;
                    break;
                }
                case 3: {
                    country = currentParam;
                    counter = 0;
                    break;
                }
            }
            if (counter == 0) {
                dataRepository.addAirlineEntry(code, name, callsign, country);
            }
        }
    }
}

/**
 * Extracts and stores the information of airports.csv
 * Time Complexity: O(n²) (worst case) | 0(n) (average case), where n is the number of lines of airports.csv
 */
void Menu::extractAirportsFile() {
    {
        ifstream airports(airportsFilePath);

        string currentParam, currentLine;
        string code, name, city, country;
        float latitude, longitude;

        int counter = 0;

        getline(airports, currentParam); //Ignore first line with just descriptors

        while (getline(airports, currentLine)) {
            istringstream iss(currentLine);
            while (getline(iss, currentParam, ',')) {
                switch (counter++) {
                    case 0: {
                        code = currentParam;
                        break;
                    }
                    case 1: {
                        name = currentParam;
                        break;
                    }
                    case 2: {
                        city = currentParam;
                        break;
                    }
                    case 3: {
                        country = currentParam;
                        break;
                    }
                    case 4: {
                        latitude = stof(currentParam);
                        break;
                    }
                    case 5: {
                        longitude = stof(currentParam);
                        counter = 0;
                        break;
                    }
                }
                if (counter == 0) {
                    Airport newAirport = dataRepository.addAirportEntry(code, name, city, country, latitude, longitude);
                    graph.addNode(newAirport);
                    dataRepository.addAirportToCityEntry(city, country, newAirport);
                }
            }
        }
    }
}

/**
 * Extracts and stores the information of flights.csv
 * Time Complexity: O(n), where n is the number of lines of flights.csv
 */
void Menu::extractFlightsFile() {

    ifstream flights(flightsFilePath);

    string currentParam, currentLine;
    string sourceCode, targetCode, airlineCode;

    int counter = 0;

    getline(flights, currentParam); //Ignore first line with just descriptors

    while (getline(flights, currentLine)) {
        istringstream iss(currentLine);
        while (getline(iss, currentParam, ',')) {
            switch (counter++) {
                case 0: {
                    sourceCode = currentParam;
                    break;
                }
                case 1: {
                    targetCode = currentParam;
                    break;
                }
                case 2: {
                    airlineCode = currentParam;
                    counter = 0;
                    break;
                }
            }
            if (counter == 0) {
                int sourceNode = graph.findAirportNode(sourceCode);
                int targetNode = graph.findAirportNode(targetCode);
                optional<Airline> airline = dataRepository.findAirline(airlineCode);
                graph.addEdge(sourceNode, targetNode, airline.value());
            }
        }
    }
}

/**
 * Outputs to the screen a message indicating that the given Airport doesn't exist
 */
void Menu::airportDoesntExist() {
    cout << "An airport with this code doesn't exist!" << endl;
}

/**
 * Outputs to the screen a message indicating that the given Airline doesn't exist
 */
void Menu::airlineDoesntExist() {
    cout << "An airline with this code doesn't exist!" << endl;
}

/**
 * Outputs main menu screen and calls other menu screens according to user input
 */
void Menu::mainMenu() {

    unsigned char commandIn = '\0';
    string line;

    while (commandIn != 'q') {
        if (commandIn == '\0') { //If program just started or returned from a different menu, print header

            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "AIR TRANSPORT";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << " LOOKUP SYSTEM" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Flights: [1]" << setw(COLUMN_WIDTH)
                 << "Information: [2]" << endl;
            cout << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }
        cout << endl << "Press the appropriate key to the function you'd like to access: ";
        cin >> commandIn;
        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                commandIn = flightsMenu();
                break;
            }
            case '2': {
                commandIn = infoMenu();
                break;
            }
            case 'q': {
                cout << "Thank you for using our Air Transport Lookup System!";
                break;
            }
            default: {
                cout << "Please press one of listed keys." << endl;
                break;
            }
        }
    }
}

/**
 * Outputs airline restrictions menu screen and returns a set containing all the valid airlines for the given inputs
 * @return - unordered_set<Airline> containing all the valid airlines for the flight
 *
 */
airlineTable Menu::airlineRestrictionsMenu() {
    unsigned char commandIn;
    airlineTable validAirlines;

    cout << setw(COLUMN_WIDTH) << setfill(' ') << "Any airline: [1]" << setw(COLUMN_WIDTH)
         << "One airline: [2]" << setw(COLUMN_WIDTH) << "Several airlines: [3]" << endl;

    while (true) {
        cout << "Please select the airlines you'd like to fly with this flight: ";
        cin >> commandIn;

        if (!checkInput(1)) {
            continue;
        }
        switch (commandIn) {
            case '1': {
                return dataRepository.getAirlines();
            }
            case '2': {
                string code;
                cout << "Please enter the code of your preferred airline: ";
                cin >> code;
                if (!checkInput(3)) break;
                optional<Airline> airline = dataRepository.findAirline(code);
                if (!airline.has_value()) {
                    airlineDoesntExist();
                    break;
                }
                validAirlines.insert(airline.value());
                return validAirlines;
            }
            case '3': {
                string code;

                cout << "Please enter the code of your preferred airline, or q to finish: ";
                cin >> code;

                while (code != "q") {
                    if (!checkInput(3)) break;
                    optional<Airline> airline = dataRepository.findAirline(code);
                    if (!airline.has_value()) {
                        airlineDoesntExist();
                        break;
                    }
                    validAirlines.insert(airline.value());
                    cout << "Please enter the code of your preferred airline, or q to finish: ";
                    cin >> code;
                }
                return validAirlines;
            }
            default:
                cout << "Please press one of listed keys." << endl;
                break;
        }
    }
}

/**
 * Outputs flights menu screen and decides graph function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned Menu::flightsMenu() {
    unsigned char commandIn = '\0';
    list<Airport> departure, arrival;

    while (commandIn != 'q') {

        bool validFirstInput = false, validFullInput = false;

        if (commandIn == '\0') {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "FLIG";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "HTS" << endl;
            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Airport: [1]" << setw(COLUMN_WIDTH)
                 << "City: [2]" << setw(COLUMN_WIDTH) << "Location: [3]" << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }
        for (string currentSelection: {"departure", "arrival"}) {

            if (currentSelection == "departure" || validFirstInput) {
                cout << endl << "Please select how to input your " << currentSelection << " location: ";
                cin >> commandIn;


                if (!checkInput(1)) {
                    commandIn = '\0';
                    break;
                }
                switch (commandIn) {
                    case '1': {
                        string airportCode;
                        cout << "Please enter the code of your preferred " << currentSelection << " airport: ";
                        cin >> airportCode;
                        if (!checkInput(3)) break;

                        optional<Airport> airport = dataRepository.findAirport(airportCode);
                        if (!airport.has_value()) {
                            airportDoesntExist();
                            break;
                        }
                        if (currentSelection == "departure") {
                            departure = {airport.value()};
                            validFirstInput = true;
                        } else {
                            arrival = {airport.value()};
                            validFullInput = true;
                        }

                        break;
                    }
                    case '2': {
                        string city;
                        cout << "Please enter your preferred city of " << currentSelection << ": ";
                        getline(cin, city);
                        if (!checkInput()) break;

                        string country;
                        cout << "Please enter the country this city is located in: ";
                        getline(cin, country);
                        if (!checkInput()) break;
                        if (!dataRepository.checkValidCityCountry(city, country)) {
                            cout << "This city and country combination is not valid!" << endl;
                            break;
                        }

                        if (currentSelection == "departure") {
                            departure = dataRepository.findAirportsInCity(city, country);
                            validFirstInput = true;
                        } else {
                            arrival = dataRepository.findAirportsInCity(city, country);
                            validFullInput = true;
                        }

                        break;
                    }
                    case '3': {
                        float latitude, longitude, maxDistance;
                        cout << "Please enter the latitude of your preferred " << currentSelection << " location: ";
                        cin >> latitude;
                        if (!checkInput()) break;
                        if (latitude < -90 || latitude > 90) {
                            cout << "Please enter an appropriate latitude." << endl;
                            break;
                        }

                        cout << "Please enter the longitude of your preferred " << currentSelection
                             << " location: ";
                        cin >> longitude;
                        if (!checkInput()) break;
                        if (longitude < -180 || longitude > 180) {
                            cout << "Please enter an appropriate latitude." << endl;
                            break;
                        }

                        cout << "Please enter the max distance of the airport to your preferred "
                             << currentSelection
                             << " location: ";
                        cin >> maxDistance;
                        if (!checkInput()) break;

                        if (currentSelection == "departure") {
                            departure = dataRepository.findAirportsInLocation(latitude, longitude, maxDistance);
                            validFirstInput = true;
                        } else {
                            arrival = dataRepository.findAirportsInLocation(latitude, longitude, maxDistance);
                            validFullInput = true;
                        }
                        break;
                    }
                    case 'b': {
                        return '\0';
                    }
                    case 'q': {
                        cout << "Thank you for using our Air Transport Lookup System!" << endl;
                        return 'q';
                    }
                    default:
                        cout << "Please press one of listed keys." << endl;
                        break;
                }
            }
        }


        if (validFullInput) {
            airlineTable validAirlines = airlineRestrictionsMenu();
            auto result = graph.getShortestPath(departure, arrival, validAirlines);
            if (result.size() == 0 || result.front().size() == 0) {
                cout << endl << "We couldn't find any valid flights for your preferences." << endl;
                continue;
            }
            cout << endl << "We suggest you take one of the following paths: " << endl;
            for (auto path: result) {
                for (const pair<airlineTable, string> &flights: path) {
                    cout << flights.second;
                    if (!flights.first.empty()) cout << " (flights by:";
                    for (const Airline &airline: flights.first) {
                        cout << " " << airline.getCode();
                    }
                    if (!flights.first.empty()) cout << ")";
                    if (flights.second != path.back().second) cout << " -> ";
                }
                cout << endl;
            }
        }
    }
    return commandIn;
}

/**
 * Outputs airport information menu screen and decides graph function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned Menu::airportInfoMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        if (commandIn == '\0') {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "AIRPORT IN";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "FOMARTION" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Number of flights: [1]" << setw(COLUMN_WIDTH)
                 << "Number of different airlines: [2]" << setw(COLUMN_WIDTH)
                 << "Number of different destinations: [3]"
                 << endl;
            cout << setw(COLUMN_WIDTH) << "Number of reachable countries: [4]" << setw(COLUMN_WIDTH)
                 << "Number of airports within x flights: [5]" << setw(COLUMN_WIDTH)
                 << "Number of cities within x flights: [6]" << endl;
            cout << setw(COLUMN_WIDTH) << "Number of countries within x flights: [7]"
                 << setw(COLUMN_WIDTH)
                 << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        while (commandIn != 'q') {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;

            if (!checkInput(1)) {
                commandIn = '\0';
                continue;
            }
            switch (commandIn) {
                case '1': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }
                    cout << graph.numFlights(airport.value()) << " flights leave from " << airport->getName()
                         << " airport." << endl;
                    break;
                }
                case '2': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }
                    cout << graph.numAirlines(airport.value()) << " airlines carry flights that leave from "
                         << airport->getName() << " airport." << endl;
                    break;
                }
                case '3': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }
                    cout << graph.numDestinations(airport.value()) << " cities are directly reachable from "
                         << airport->getName() << " airport." << endl;
                    break;
                }
                case '4': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }
                    cout << graph.numCountries(airport.value()) << " countries are directly reachable from "
                         << airport->getName() << " airport." << endl;
                    break;
                }
                case '5': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }

                    unsigned numFlights;
                    cout << "Please enter the max number of flights you'd like to check for: ";
                    cin >> numFlights;
                    if (!checkInput(5)) break;

                    cout << graph.numAirportsInXFlights(airport.value(), numFlights)
                         << " other airports are reachable in "
                         << numFlights << " or less flights from "
                         << airport->getName() << " airport." << endl;
                    break;
                }
                case '6': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }

                    unsigned numFlights;
                    cout << "Please enter the max number of flights you'd like to check for: ";
                    cin >> numFlights;
                    if (!checkInput(5)) break;

                    cout << graph.numCitiesInXFlights(airport.value(), numFlights) << " other cities are reachable in "
                         << numFlights << " or less flights from "
                         << airport->getName() << " airport." << endl;
                    break;
                }
                case '7': {
                    string airportCode;
                    cout << "Please enter the code of the airport you'd like to obtain information about: ";
                    cin >> airportCode;
                    if (!checkInput(3)) break;
                    optional<Airport> airport = dataRepository.findAirport(airportCode);
                    if (!airport.has_value()) {
                        airportDoesntExist();
                        break;
                    }

                    unsigned numFlights;
                    cout << "Please enter the max number of flights you'd like to check for: ";
                    cin >> numFlights;
                    if (!checkInput(5)) break;

                    cout << graph.numCountriesInXFlights(airport.value(), numFlights)
                         << " other countries are reachable in "
                         << numFlights << " or less flights from "
                         << airport->getName() << " airport." << endl;
                    break;
                }
                case 'b': {
                    return '\0';
                }
                case 'q': {
                    cout << "Thank you for using our Air Transport Lookup System!" << endl;
                    break;
                }
                default:
                    cout << "Please press one of listed keys." << endl;
                    break;
            }
        }
    }
    return commandIn;
}

/**
 * Outputs general information menu screen and decides function calls according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned Menu::generalInfoMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        if (commandIn == '\0') {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "GENERAL IN";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "FOMARTION" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Number of flights: [1]" << setw(COLUMN_WIDTH)
                 << "Number of airports: [2]" << setw(COLUMN_WIDTH)
                 << "Number of airlines: [3]"
                 << endl;
            cout << setw(COLUMN_WIDTH) << "Number of cities: [4]" << setw(COLUMN_WIDTH)
                 << "Number of countries: [5]" << setw(COLUMN_WIDTH)
                 << "Number of strongly connected components: [6]" << endl;
            cout << setw(COLUMN_WIDTH) << "Diameter: [7]" << setw(COLUMN_WIDTH)
                 << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        while (commandIn != 'q') {
            cout << endl << "Press the appropriate key to the function you'd like to access: ";
            cin >> commandIn;

            if (!checkInput(1)) {
                commandIn = '\0';
                continue;
            }
            switch (commandIn) {
                case '1': {
                    cout << "Our system includes " << graph.getTotalFlights() << " flights that connect "
                         << graph.getTotalFlightsAirlineless() << " different pairs of airports!" << endl;
                    break;
                }
                case '2': {
                    cout << "Our system includes " << dataRepository.getAirports().size() << " different airports!"
                         << endl;
                    break;
                }
                case '3': {
                    cout << "Our system includes " << dataRepository.getAirlines().size() << " different airlines!"
                         << endl;
                    break;
                }
                case '4': {
                    cout << "Our system includes " << dataRepository.getCityToAirports().size() << " different cities!"
                         << endl;
                    break;
                }
                case '5': {
                    cout << "Our system includes " << dataRepository.getTotalNumCountries() << " different countries!"
                         << endl;
                    break;
                }
                case '6': {
                    cout << "Our flights graph has " << graph.countSCCs() << " strongly connected components!"
                         << endl;
                    break;
                }
                case '7': {
                    cout << "Our flights graph has a diameter of " << graph.getDiameter() << "!" << endl;
                    break;
                }
                case 'b': {
                    return '\0';
                }
                case 'q': {
                    cout << "Thank you for using our Air Transport Lookup System!" << endl;
                    break;
                }
                default:
                    cout << "Please press one of listed keys." << endl;
                    break;
            }
        }
    }
    return commandIn;
}

/**
 * Outputs information menu screen and calls other menu screens according to user input
 * @return - Last inputted command, or '\0' for previous menu command
 */
unsigned Menu::infoMenu() {
    unsigned char commandIn = '\0';

    while (commandIn != 'q') {
        if (commandIn == '\0') {
            //Header
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << setfill('-') << right << "INFOR";
            cout << setw(COLUMN_WIDTH * COLUMNS_PER_LINE / 2) << left << "MATION" << endl;

            cout << setw(COLUMN_WIDTH) << setfill(' ') << "Information on a given airport: [1]"
                 << setw(COLUMN_WIDTH)
                 << "Information on the global network: [2]" << endl;
            cout << setw(COLUMN_WIDTH) << "Back: [b]" << setw(COLUMN_WIDTH) << "Quit: [q]" << endl;
        }

        cout << endl << "Press the appropriate key to the function you'd like to access: ";
        cin >> commandIn;

        if (!checkInput(1)) {
            commandIn = '\0';
            continue;
        }
        switch (commandIn) {
            case '1': {
                commandIn = airportInfoMenu();
                break;
            }
            case '2': {
                commandIn = generalInfoMenu();
                break;
            }
            case 'b': {
                return '\0';
            }
            case 'q': {
                cout << "Thank you for using our Air Transport Lookup System!" << endl;
                break;
            }
            default:
                cout << "Please press one of listed keys." << endl;
                break;
        }
    }
    return commandIn;
}


/**
 * Checks if the input given by the user is appropriate or not
 * Time Complexity: O(1)
 * @param checkLength - Integer indicating if the length of the input should be checked or not, and, if so, its valid max length
 * @return Returns true if the input is appropriate and false if it isn't
 */
bool Menu::checkInput(unsigned int checkLength) {

    //checkLength = 0 Don't check length
    //checkLength = 1 Check for length 1
    //checkLength = 2 Check for max length 2
    //...

    if (!cin) // User didn't input what expected
    {
        cin.clear(); // Reset failbit
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //Skip bad input
        cout << "Please enter an appropriate input." << endl;
        return false;
    }

    if (checkLength > 0) {
        string line;
        getline(cin, line);
        if (line.length() >= checkLength) {
            cout << "Please enter an appropriate input." << endl;
            return false;
        }
    }
    return true;
}

/**
 * Delegates initialization of the menu, calling the appropriate functions for information extraction and output
 */
void Menu::initializeMenu() {
    extractFileInfo();
    mainMenu();
}

