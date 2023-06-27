#include "GPSFileReader.h"

namespace PaulNovack {

  GPSFileReader::GPSFileReader() {
  }

  GPSFileReader::GPSFileReader(const GPSFileReader& orig) {
  }

  GPSFileReader::~GPSFileReader() {
  }

  void GPSFileReader::RunFileReader() {
    ifstream gpsDataFile("/home/pnovack/2023Code/boatAutopilot/autoPilotCPlusPlusServer/GPSTracks/TripToLowes.txt");
    if (!gpsDataFile.is_open()) {
      cout << "Failed to open the GPS data file." << endl;
    }

    // Read and process each line of the GPS data
    string gpsMessage;
    long i = 0;
    while (getline(gpsDataFile, gpsMessage)) {
      gpsParser->parse(gpsMessage);
      if (gpsParser->_messageType == "GLL") {
        // pass data to navigation that is calculate in Parser
        this_thread::sleep_for(chrono::milliseconds(1000));

      }
    }
    gpsDataFile.close();
  }

  void GPSFileReader::setPlugins(Navigation& navigation, GPSParser& gpsParser) {
    this->navigation = &navigation;
    this->gpsParser = &gpsParser;
  }
}