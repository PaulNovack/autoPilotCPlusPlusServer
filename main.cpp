#include <fstream>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>
#include <chrono>
#include "GPSParser.h"
#include "Navigation.h"
#include "GPSFileReader.h"
#include "BoatMotorDriver.h"
#include <filesystem>
#include "PaulNovackGlobals.h"
#include "AppConfig.h"
#include "WebServer.h"

using namespace PaulNovack;

using namespace std;

bool isDirectory(const string& path) {
  struct stat fileInfo;
  if (stat(path.c_str(), &fileInfo) != 0) {
    // Error occurred while retrieving file information
    return false;
  }
  return S_ISDIR(fileInfo.st_mode);
}

int main(int argc, char* argv[]) {
  // Create instances of Navigation and GPSParser FileReader and GPSReader
  if (argc > 1 && string(argv[1]) == "--help") {
    cout << "Usage: " << argv[0] << " <application_root>" << endl;
    cout << "Provide the application root directory as an argument." << endl;
    return 0;
  }

  // Check if the application root is provided as an argument
  if (argc < 2) {
    cerr << "Error: Application root not provided." << endl;
    cerr << "Usage: " << argv[0] << " <application_root>" << endl;
    return 1;
  }

  string applicationRoot = argv[1];

  // Check if the provided directory exists
  if (!isDirectory(applicationRoot)) {
    cerr << "Error: Directory does not exist: " << applicationRoot << endl;
    return 1;
  }
  AppConfig config(applicationRoot + "/.env");

  GPSParser gpsParser;
  Navigation navigation;
  navigation.setConfig(config);
  BoatMotorDriver boatMotorDriver;

  GPSUtils gpsUtils;
  gpsParser.setConfig(config);
  gpsParser.setNavigation(navigation);
  gpsParser.setGPSUtils(gpsUtils);
  navigation.setGPSParser(gpsParser);
  navigation.setGPSUtils(gpsUtils);
  boatMotorDriver.setConfig(config);
  boatMotorDriver.setNavigation(navigation);



  // start the MotorDriver watching the GPS coordinates in thread
  thread boatMotorDriverThread(&PaulNovack::BoatMotorDriver::runCheck, &boatMotorDriver);
  thread destinationReaderThread(&PaulNovack::Navigation::ReadDestination, &navigation);



  PaulNovack::GPSFileReader gpsFileReader;
  gpsFileReader.setPlugins(navigation, gpsParser);
  //  BoatAutoPilotCrowWebServer boatAutoPilotCrowWebServer;
  // boatAutoPilotCrowWebServer.setPlugins(navigation,gpsParser);
  thread gpsMockReaderThread(&PaulNovack::GPSFileReader::RunFileReader, &gpsFileReader);


  // Read from a file simulate a GPS Reciever
  // gpsFileReader.RunFileReader();
  WebServer webserver;
  webserver.setNavigation(navigation);
  
  webserver.Run();
  
  return 0;

}

