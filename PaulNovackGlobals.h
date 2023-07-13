#ifndef GLOBALS_H
#define GLOBALS_H
#include <sys/stat.h>

using namespace std;

namespace PaulNovack {
    constexpr float PI = 3.14159265358979323846f;
    const float EARTH_RADIUS_KM = 6371.0; // Earth's radius in kilometers
    const float MILES_PER_KM = 0.621371;
    const float NAUTICAL_MILES_PER_KM = 0.539956803;

    struct TurnData {
        float degrees;
        std::string direction;
    };

    struct LatLong {
        float latitude;
        float longitude;
    };

    struct WayPoint {
        int id;
        std::string name;
        std::string description;
        float latitude;
        float longitude;
        float depth;
    };

    struct NavData {
        float _latitude;
        float _longitude;
        float _heading;
        float _destHeading;
        float _destLatitude;
        float _destLongitude;
        float _rHTurn;
        float _lHTurn;
        string _turnDirection;
        float _turnDegrees;
        int _stepsPerCorrection;
        float _speedKnots;
        float _speedMPH;
        float _headingDeviation;
        float _milesToDestination;
        int _goBackSecondsSpeedCalculation;
        bool _usingGoBackSecondsSpeedCalculation;
        string _hdop;
        string _pdop;
    };



}

#endif // LATLONG_H