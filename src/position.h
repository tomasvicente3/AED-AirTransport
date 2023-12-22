#ifndef POSITION_H
#define POSITION_H

#include <cmath>

class Position {
private:
    float latitude;
    float longitude;
public:
    Position(float const &latitude, float const &longitude);

    [[nodiscard]] float getLatitude() const;

    void setLatitude(float latitude);

    [[nodiscard]] float getLongitude() const;

    void setLongitude(float longitude);

    double getDistance(Position position) const;
};

#endif