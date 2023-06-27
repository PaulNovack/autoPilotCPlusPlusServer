#ifndef GPSFILEREADER_H
#define GPSFILEREADER_H

#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <chrono>
#include "GPSParser.h"
#include "Navigation.h"

using namespace std;

namespace PaulNovack {
    class Navigation;

    class GPSParser;

    class GPSFileReader {
    public:
        GPSFileReader();
        GPSFileReader(const GPSFileReader& orig);
        void RunFileReader();
        virtual ~GPSFileReader();
        void setPlugins(Navigation& navigation, GPSParser& gpsParser);
    private:
        Navigation* navigation = nullptr;
        GPSParser *gpsParser = nullptr;
    };
}
#endif /* GPSFILEREADER_H */

