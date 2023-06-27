#include "GPSParser.h"

using namespace PaulNovack;

namespace PaulNovack {

  class AppConfig;

  class Navigation;

  GPSParser::GPSParser() {
    _messageType = "NONE";

    this->_validReadingCount = 0;
  }

  GPSParser::GPSParser(const GPSParser& orig) {

  }

  void GPSParser::setNavigation(Navigation& navigation) {
    _navigation = &navigation;
  }

  void GPSParser::setConfig(AppConfig& config) {
    _config = &config;
    this->_goBackSecondsSpeedCalculation = _config->GO_BACK_SECONDS_FOR_SPEED;
  }

  GPSParser::~GPSParser() {
  }

  float GPSParser::getSpeedInMPH() {
    if (_gpsSpeedInNMPH == 0.0)return 0.0;
    return _gpsSpeedInNMPH * 1.15078;
  }

  float GPSParser::getSpeedInKnots() {
    return _gpsSpeedInNMPH;
  }

  void GPSParser::setGPSUtils(GPSUtils& gu) {
    _gu = &gu;
  }

  bool GPSParser::parse(const string& message) {
    // Split the message by comma
    bool validGPSPort = false;
    vector<string> tokens;
    istringstream iss(message);
    string token;
    while (getline(iss, token, ',')) {
      tokens.push_back(token);
    }

    // Process the tokens
    if (tokens.size() > 0) {
      string messageType = tokens[0];
      if (messageType == "$GPGGA") {
        this->_messageType = "GGA";
        // Process GGA message
        validGPSPort = true;
        processGGA(tokens);
      } else if (messageType == "$GPVTG") {
        // Process RMC message
        validGPSPort = true;
        this->_messageType = "VTG";
        processVTG(tokens);
      } else if (messageType == "$GPGLL") {
        // Process GLL message
        this->_messageType = "GLL";
        validGPSPort = true;
        processGLL(tokens);
      } else if (messageType == "$GPRMC") {
        // Process GLL message
        this->_messageType = "RMC";
        validGPSPort = true;
        processRMC(tokens);
      } else if (messageType == "$GPGSA") {
        // Process GLL message
        this->_messageType = "GSA";
        validGPSPort = true;
        processGSA(tokens);
      } else {
        this->_messageType = "NONE";
        // Unsupported message type
        //cout << "Unsupported message type: " << messageType << endl;
      }
    }
    return validGPSPort;
  }

  void GPSParser::processRMC(const vector<string>& tokens) {
    // Extract relevant information from GGA message
    if (tokens.size() >= 10 && tokens[7] != "") {
      _gpsCourse = stof(tokens[7]);
    }
  }

  void GPSParser::processGSA(const vector<string>& tokens) {
    // Extract relevant information from GGA message
    if (tokens.size() >= 10 && tokens[15] != "") {
      _pdop = tokens[15];
    }
    if (tokens.size() >= 10 && tokens[16] != "") {
      _hdop = tokens[16];
    }
  }

  void GPSParser::processGGA(const vector<string>& tokens) {
    // Extract relevant information from GGA message
    if (tokens.size() >= 10 && tokens[1] != "") {
      this->_gpsTime = tokens[1];
      this->_gpsFixQuality = tokens[6];
      this->_gpsSatellites = tokens[7];
      this->_gpsHdop = tokens[8];
      this->_gpsAltitude = tokens[9];
    }
  }

  string GPSParser::secondsToTime(int seconds) {
    int hour = seconds / 3600; // Number of hours
    int minute = (seconds % 3600) / 60; // Number of minutes
    int second = seconds % 60; // Number of seconds

    string amPm = (hour >= 12) ? "PM" : "AM";

    // Convert hour to 12-hour format
    if (hour > 12) {
      hour -= 12;
    } else if (hour == 0) {
      hour = 12;
    }

    // Create formatted time string
    string timeStr = to_string(hour) + ":" +
            ((minute < 10) ? "0" : "") + to_string(minute) + ":" +
            ((second < 10) ? "0" : "") + to_string(second) + " " + amPm;

    return timeStr;
  }

  void GPSParser::processVTG(const vector<string>& tokens) {
    if (tokens.size() >= 6 && tokens[1] != "") {
      _gpsCourse = stof(tokens[1]);
    }
    if (tokens.size() >= 6 && tokens[5] != "") {
      _gpsSpeedInNMPH = stof(tokens[5]);
    } else {
      _gpsSpeedInNMPH = 0.0;
    }

  }

  void GPSParser::processGLL(const vector<string>& tokens) {
    // Extract relevant information from GLL message
    // GLL is the last message sent every second so
    // we can update the Navigation class after it is sent have 
    // then complete data for that second
    if (tokens.size() >= 6 && tokens[1] != "") {
      _gpsGllLatitude = tokens[1];
      _gpsLatitudeDirection = tokens[2];
      _gpsGllLongitude = tokens[3];
      _gpsLongitudeDirection = tokens[4];
      _gpsTime = tokens[5];
      _gpsDataValidFlag = tokens[6];
      if (_gpsGllLatitude != "") {
        _validReadingCount++;
      }
      LatLong latLong;
      latLong.latitude = _gu->getDecimalDegreeFromGPS(_gpsGllLatitude, _gpsLatitudeDirection);
      latLong.longitude = _gu->getDecimalDegreeFromGPS(_gpsGllLongitude, _gpsLongitudeDirection);
      pushLocationHistoryToFront(latLong);

      // Initialize all variables 0.0 this just makes it easy to see what is passed
      // to function mostly for readability
      float latitude = 0.0;
      float longitude = 0.0;
      float heading = 0.0;
      float speedKnots = 0.0;
      float speedMPH = 0.0;
      bool usingGoBackSpeedCorrection = false;
      int goBackSecondsSpeedCalculation = _goBackSecondsSpeedCalculation;
      // Go thru and set all variables with equations to convert from GPS format
      // and improve accuracy by taking multiple values over time period 
      // to calculate more accurately for Navigation Class
      latitude = _gu->getDecimalDegreeFromGPS(_gpsGllLatitude, _gpsLatitudeDirection);
      longitude = _gu->getDecimalDegreeFromGPS(_gpsGllLongitude, _gpsLongitudeDirection);
      heading = _gpsCourse;

      if (_goBackSecondsSpeedCalculation != 0
              && _validReadingCount >= _goBackSecondsSpeedCalculation) {
        float oldLatitude = latLongArray[_goBackSecondsSpeedCalculation - 1].latitude;
        float oldLongitude = latLongArray[_goBackSecondsSpeedCalculation - 1].longitude;
        float thisLatitude = _gu->getDecimalDegreeFromGPS(_gpsGllLatitude, _gpsLatitudeDirection);
        float thisLongitude = _gu->getDecimalDegreeFromGPS(_gpsGllLongitude, _gpsLongitudeDirection);
        speedKnots = _gu->calculateNMPH(goBackSecondsSpeedCalculation, oldLatitude, oldLongitude, thisLatitude, thisLongitude);
        speedMPH = _gu->calculateMPH(goBackSecondsSpeedCalculation, oldLatitude, oldLongitude, thisLatitude, thisLongitude);

        usingGoBackSpeedCorrection = true;
      } else {
        speedKnots = 0.0; // to inaccurate _gu->calculateMPH(goBackSecondsSpeedCalculation, oldLatitude, oldLongitude, thisLatitude, thisLongitude);
        speedMPH = 0.0; // to inaccurate __gu->calculateNMPH(goBackSecondsSpeedCalculation, oldLatitude, oldLongitude, thisLatitude, thisLongitude);
      }



      _navigation->SetGPSData(
              latitude,
              longitude,
              heading,
              speedKnots,
              speedMPH,
              usingGoBackSpeedCorrection,
              _hdop,
              _pdop);
    }
  }

  void GPSParser::pushLocationHistoryToFront(const LatLong& element) {
    for (int i = MAX_SIZE - 1; i >= 1; --i) {
      latLongArray[i] = latLongArray[i - 1];
    }
    latLongArray[0] = element;
  }
}