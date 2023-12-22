#include "position.h"

using namespace std;

Position::Position(const float &latitude, const float &longitude) {
    this->latitude = latitude;
    this->longitude = longitude;
}

float Position::getLatitude() const {
    return latitude;
}

void Position::setLatitude(float latitude) {
    Position::latitude = latitude;
}

float Position::getLongitude() const {
    return longitude;
}

void Position::setLongitude(float longitude) {
    Position::longitude = longitude;
}

double Position::getDistance(Position position) const {
    double radianLatDist = (this->latitude - position.getLatitude()) * M_PI / 180.0;
    double radianLonDist = (this->longitude - position.getLongitude()) * M_PI / 180.0;
    double radianLat1 = this->latitude * M_PI / 180.0;
    double radianLat2 = position.getLatitude() * M_PI / 180.0;

    double tempCalc =
            pow(sin(radianLatDist / 2), 2) + pow(sin(radianLonDist / 2), 2) * cos(radianLat1) * cos(radianLat2);
    double rad = 6371;
    double distanceValue = rad * 2 * asin(sqrt(tempCalc));

    return distanceValue;

}
