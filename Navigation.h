#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <string>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include "GPSParser.h"
#include "PaulNovackGlobals.h"
#include "GPSUtils.h"
#include "json.hpp"
#include "AppConfig.h"

using json = nlohmann::json;

using namespace std;

namespace PaulNovack {

    class GPSParser;

    class Navigation {
    public:

        Navigation();
        Navigation(const Navigation& orig);
        virtual ~Navigation();
        int ReadDestination();
        void CalculateNavigation();
        void SetGPSData(
                float latitude,
                float longitude,
                float heading,
                float speedKnots,
                float speedMPH,
                bool goBackSecondsSpeedCalculation,
                string hdop,
                string pdop);
        void setGPSParser(GPSParser& gpsParser);
        void setGPSUtils(GPSUtils& gpsUtils);

        GPSParser* _gpsParser = nullptr;
        GPSUtils* _gu = nullptr;
        AppConfig* _config = nullptr;
        TurnData getTurnData();
        void setConfig(AppConfig& config);
        NavData getNavData();
        void setDestination(float Latitude,float Longitude);
    private:
        
        static const int MAX_SIZE = 300;
        LatLong latLongArray[MAX_SIZE];
        void pushLocationHistoryToFront(const LatLong& element);

        float calculateBearing(float lat1, float lon1, float lat2, float lon2);
        float degreesToRadians(float degrees);
        float calculateDistance(float lat1, float lon1, float lat2, float lon2);

        mutex nav_mutex;
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
#endif // NAVIGATION_H


