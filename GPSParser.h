#ifndef GPSPARSER_H
#define GPSPARSER_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <iostream>
#include "PaulNovackGlobals.h"
#include "Navigation.h"
#include "GPSUtils.h"
#include "AppConfig.h"

using namespace std;

namespace PaulNovack {
    class Navigation;

    class AppConfig;

    class GPSParser {
    public:
        GPSParser();
        GPSParser(const GPSParser& orig);
        void setNavigation(Navigation& navigation);
        void setGPSUtils(GPSUtils& gu);
        void setConfig(AppConfig& config);
        string secondsToTime(int seconds);
        float convertToDecimal(const string coordinate, const string direction);
        virtual ~GPSParser();
        bool parse(const string& message);
        float getSpeedInKnots();
        float getSpeedInMPH();
        float calcLatitude;
        float calcLongitude;
        float calcSpeedInNMPH;
        float calcSpeedInMPH;
        float calcCourse;
        // _timeSinceStart is used for if we are calculating speed based on travel 
        // over n seconds instead of GPS output speed which is pretty inaccurate
        // when stationary or moving slowly
        int _timeSinceStart;

        // How far back in seconds to try and calculate speed
        // works getting a coordinate a couple seonds back
        // and calculating speed from that rather than direct speed
        // output from GPS  to disable just set to 0 then uses GPS speed
        // output

        float _gpsSpeedInNMPH;
        float _gpsCourse;
        string _gpsGllLatitude;
        string _gpsLatitudeDirection;
        string _gpsGllLongitude;
        string _gpsLongitudeDirection;
        string _gpsDataValidFlag;
        string _gpsFixQuality;
        string _gpsSatellites;
        string _gpsHdop;
        string _gpsAltitude;
        string _gpsTime;
        string _gpsStatus;
        string _messageType;
        string _hdop;
        string _pdop;
        int _goBackSecondsSpeedCalculation;
        long _validReadingCount; // The number of valid latitude longitude readings from gps receiver
        float getHistoryDecimalLongitude();
        float getHistoryDecimalLatitude();
        static const int MAX_SIZE = 300;
        PaulNovack::LatLong latLongArray[MAX_SIZE] = {};
        void pushLocationHistoryToFront(const LatLong& element);

        Navigation* _navigation = nullptr;
        GPSUtils* _gu = nullptr;
        AppConfig* _config = nullptr;

    private:
        void processGGA(const vector<string>& tokens);
        void processVTG(const vector<string>& tokens);
        void processGLL(const vector<string>& tokens);
        void processRMC(const vector<string>& tokens);
        void processGSA(const vector<string>& tokens);

    };
}
#endif // GPSPARSER_H
