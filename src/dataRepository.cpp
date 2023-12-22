//
// Created by rita on 04-01-2023.
//

#include <iostream>
#include "dataRepository.h"

using namespace std;

DataRepository::DataRepository() = default;

const airlineTable &DataRepository::getAirlines() const {
    return airlines;
}

void DataRepository::setAirlines(const airlineTable &airlines) {
    DataRepository::airlines = airlines;
}

/**
 * Adds a new entry to the unordered_set of Airlines, creating the corresponding Airline object
 * Time Complexity: O(n) (worst case) | O(1) (average case)
 *
 * @param code - Code of the new Airline
 * @param name - Name of the new Airline
 * @param callsign - Callsign of the new Airline
 * @param country - Country of the new Airline
 * @return Created Airline object
 */
Airline DataRepository::addAirlineEntry(string code, string name, string callsign, string country) {
    Airline newAirline = Airline(code, name, callsign, country);
    airlines.insert(newAirline);
    return newAirline;
}

const airportTable &DataRepository::getAirports() const {
    return airports;
}

void DataRepository::setAirports(const airportTable &airports) {
    DataRepository::airports = airports;
}

/**
 * Adds a new entry to the unordered_set of Airports, creating the corresponding Airport object
 * Time Complexity: O(n) (worst case) | O(1) (average case)
 *
 * @param code - Code of the new Airport
 * @param name - Name of the new Airport
 * @param city - City of the new Airport
 * @param country - Country of the new Airport
 * @param latitude - Latitude of the new Airport
 * @param longitude - Longitude of the new Airport
 * @return Created Airport object
 */
Airport DataRepository::addAirportEntry(string code, string name, string city, string country, float latitude,
                                        float longitude) {
    Airport newAirport = Airport(code, name, city, country, latitude, longitude);
    airports.insert(newAirport);
    return newAirport;
}

/**
 * Adds a new Airport value to an entry in the unordered_map of (city, country) pairs
 * Time Complexity: O(n) (worst case) | O(1) (average case)
 *
 * @param city - City of the new Airport
 * @param country - Country of the new Airport
 * @param airport - Airport to add
 */
void DataRepository::addAirportToCityEntry(const string &city, const string &country, const Airport &airport) {
    list<Airport> currList = cityToAirports[{city, country}];
    currList.push_back(airport);
    cityToAirports.insert_or_assign({city, country}, currList);
}

const cityToAirportsMap &DataRepository::getCityToAirports() const {
    return cityToAirports;
}

void DataRepository::setCityToAirports(const cityToAirportsMap &cityToAirports) {
    DataRepository::cityToAirports = cityToAirports;
}

/**
 * Finds the Airport object with the given code
 * @param code - Code of the Airport to be returned
 * @return optional<Airport> value which will contain the Airport object, or be empty if no such Airport was found
 */
std::optional<Airport> DataRepository::findAirport(const string &code) {
    std::optional<Airport> result;
    auto it = airports.find(Airport(code));
    if (it != airports.end()) result = *it;
    return result;
}

/**
 * Finds the Airline object with the given code
 * @param code - Code of the Airline to be returned
 * @return optional<Airline> value which will contain the Airline object, or be empty if no such Airline was found
 */
std::optional<Airline> DataRepository::findAirline(const string &code) {
    std::optional<Airline> result;
    auto it = airlines.find(Airline(code));
    if (it != airlines.end()) result = *it;
    return result;
}

/**
 * Finds the Airport objects with the given city
 * @param city - City whose airports should be found
 * @param country - Country the city belongs to (used to differentiate same name cities)
 * @return list<Airport> containing the Airports in the given city
 */
list<Airport> DataRepository::findAirportsInCity(const std::string &city, const std::string &country) {
    return cityToAirports.at({city, country});
}

/**
 * Checks if the given city and country combination is valid, that is, if there is stored data referencing it
 * @param city - City to be validated
 * @param country - Country to be validated
 * @return true if the combination is valid, false if it is not
 */
bool DataRepository::checkValidCityCountry(const std::string &city, const std::string &country) {
    return cityToAirports.find({city, country}) != cityToAirports.end();
}

/**
 * Finds all the airports in a certain distance to the given location
 * @param latitude - Latitude of the location
 * @param longitude - Longitude of the location
 * @param maxDistance - Max valid distance of the airport to the location
 * @return List of Airports with all the airports within a distance of maxDistance
 */
list<Airport> DataRepository::findAirportsInLocation(float latitude, float longitude, float maxDistance) {
    Position startPos = Position(latitude, longitude);
    list<Airport> valid;
    for (auto aport: airports) {
        if (aport.getLocation().getDistance(startPos) <= maxDistance) {
            valid.push_back(aport);
        }
    }
    return valid;
}

/**
 * Computes total number of different countries
 * Time Complexity: O(n²) (worst case) | 0(n) (average case), where n is the size of cityToAirport
 * @return Number of different countries
 */
unsigned DataRepository::getTotalNumCountries(){
    unordered_set<string> countries;
    for(auto p: cityToAirports) countries.insert(p.first.second);
    return countries.size();
}


