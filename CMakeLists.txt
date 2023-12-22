cmake_minimum_required(VERSION 3.24)
project(AirTransport)

set(CMAKE_CXX_STANDARD 17)

add_executable(AirTransport src/main.cpp src/airline.cpp src/airline.h src/airport.cpp src/airport.h src/graph.cpp src/graph.h src/menu.cpp src/menu.h src/position.cpp src/position.h src/dataRepository.h src/dataRepository.cpp src/dataRepository.cpp)
