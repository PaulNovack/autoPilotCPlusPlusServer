#ifndef GPSUTILS_H
#define GPSUTILS_H

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <cmath>
#include "PaulNovackGlobals.h"

using namespace std;

namespace PaulNovack {

    class GPSUtils {
    public:
        GPSUtils();
        GPSUtils(const GPSUtils& orig);
        virtual ~GPSUtils();
        string trim_leading_zeros(string str);
        float getDecimalDegreeFromGPS(string value, string hemisphere);
        float degreesToRadians(float degrees);
        float calculateDistanceM(const float lat1, const float lon1, const float lat2, const float lon2);
        float calculateMPH(int secondsElapsed, float lat1, float lon1,
                float lat2, float lon2);
        float calculateDistanceNM(const float lat1, const float lon1, const float lat2, const float lon2);
        float calculateNMPH(int secondsElapsed, float lat1, float lon1,
                float lat2, float lon2);
    private:

    };
}

#endif /* GPSUTILS_H */

