//
// Created by rita on 04-01-2023.
//

#ifndef AIRTRANSPORT_DATAREPOSITORY_H
#define AIRTRANSPORT_DATAREPOSITORY_H

#include <list>
#include <optional>
#include <algorithm>
#include "airport.h"
#include "airline.h"

struct pairCityHash {
    std::size_t operator()(std::pair<std::string, std::string> const &pair) const {
        std::size_t h1 = std::hash<std::string>{}(pair.first);
        std::size_t h2 = std::hash<std::string>{}(pair.second);
        return h1 ^ (h2 << 1);
    }
};

struct pairCityEquals {
    bool
    operator()(std::pair<std::string, std::string> const &pair1,
               std::pair<std::string, std::string> const &pair2) const {
        return pair1.first == pair2.first && pair1.second == pair2.second;
    }
};

typedef std::unordered_map<std::pair<std::string, std::string>, std::list<Airport>, pairCityHash, pairCityEquals> cityToAirportsMap;
typedef std::unordered_set<std::pair<std::string, std::string>, pairCityHash, pairCityEquals> cityTable;


class DataRepository {
private:
    airlineTable airlines;
    airportTable airports;
    cityToAirportsMap cityToAirports;
public:
    DataRepository();

    const airlineTable &getAirlines() const;

    void setAirlines(const airlineTable &airlines);

    const airportTable &getAirports() const;

    void setAirports(const airportTable &airports);

    const cityToAirportsMap &getCityToAirports() const;

    void setCityToAirports(const cityToAirportsMap &cityToAirports);

    std::optional<Airport> findAirport(const std::string &code);

    std::optional<Airline> findAirline(const std::string &code);

    std::list<Airport> findAirportsInCity(const std::string &city, const std::string &country);

    Airline addAirlineEntry(std::string code, std::string name, std::string callsign, std::string country);

    Airport addAirportEntry(std::string code, std::string name, std::string city, std::string country, float latitude,
                            float longitude);

    void addAirportToCityEntry(const std::string &city, const std::string &country, const Airport &airport);

    bool checkValidCityCountry(const std::string &city, const std::string &country);

    std::list<Airport> findAirportsInLocation(float latitude, float longitude, float maxDistance);

    unsigned int getTotalNumCountries();
};


#endif //AIRTRANSPORT_DATAREPOSITORY_H
