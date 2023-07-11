#include "Navigation.h"

// Example member function implementation
namespace PaulNovack {

  class GPSParser;

  Navigation::Navigation() {
    _destLatitude = 29.544796;
    _destLongitude = -95.140574;

  }

  Navigation::Navigation(const Navigation& orig) {

  }

  Navigation::~Navigation() {
  }

  void Navigation::setGPSParser(GPSParser& gpsParser) {
    _gpsParser = &gpsParser;
  }

  void Navigation::setConfig(AppConfig& config) {
    _config = &config;
    _stepsPerCorrection = _config->STEPS_PER_CORRECTION;
    this->_goBackSecondsSpeedCalculation = _config->GO_BACK_SECONDS_FOR_SPEED;
  }

  void Navigation::setGPSUtils(GPSUtils& gpsUtils) {
    _gu = &gpsUtils;
  }

  int Navigation::ReadDestination() {
    while (true) {
      ifstream inputFile(_config->APP_ROOT + "destination.json");
      if (!inputFile.is_open()) {
        cerr << "Failed to open the file." << endl;
        return 1;
      }

      // Read the file content into a string
      string jsonContent;
      try {
        jsonContent = string(istreambuf_iterator<char>(inputFile), istreambuf_iterator<char>());
      } catch (const exception& e) {
        cerr << "Error while reading file: " << e.what() << endl;
        inputFile.close();
        return 1;
      }

      // Close the file
      inputFile.close();

      // Parse the JSON content
      json jsonData;
      try {
        jsonData = json::parse(jsonContent);
      } catch (const json::parse_error& e) {
        cerr << "Error while parsing JSON: " << e.what() << endl;
        return 1;
      }

      // Extract the latitude and longitude from the JSON data
      try {
        string destLatitudeNew = jsonData["destination"]["latitude"];
        string destLongitudeNew = jsonData["destination"]["longitude"];
        lock_guard<mutex> lock(nav_mutex);
        this->_destLatitude = stof(destLatitudeNew);
        this->_destLongitude = stof(destLongitudeNew);
      } catch (const json::exception& e) {
        cerr << "Error while extracting data from JSON: " << e.what() << endl;
        return 1;
      }

      // PrnavData.the extracted latitude and longitude
      cout << "Destination Latitude: " << _destLatitude << endl;
      cout << "Destination Longitude: " << _destLongitude << endl;
      this_thread::sleep_for(chrono::seconds(10));
    }
  }

  void Navigation::SetGPSData(
          float latitude,
          float longitude,
          float heading,
          float speedKnots,
          float speedMPH,
          bool usingGoBackSecondsSpeedCalculation,
          string hdop,
          string pdop) {
    lock_guard<mutex> lock(nav_mutex);
    this->_latitude = latitude;
    this->_longitude = longitude;
    this->_heading = heading;
    this->_speedKnots = speedKnots;
    this->_speedMPH = speedMPH;
    this->_usingGoBackSecondsSpeedCalculation = usingGoBackSecondsSpeedCalculation;
    this->_hdop = hdop;
    this->_pdop = pdop;
    //this->_goBackSecondsSpeedCalculation = goBackSecondsSpeedCalculation;
    CalculateNavigation();
    bool debug = true;
    if (debug) {
      cout << fixed << setprecision(9);
      cout << "-------------------------------------------------------------" << endl;
      cout << "_latitude: " << _latitude << endl;
      cout << "_longitude: " << _longitude << endl;
      cout << "_heading: " << _heading << endl;
      cout << "_destHeading: " << _destHeading << endl;
      cout << "_destLatitude: " << _destLatitude << endl;
      cout << "_destLongitude: " << _destLongitude << endl;
      cout << "_rHTurn: " << _rHTurn << endl;
      cout << "_lHTurn: " << _lHTurn << endl;
      cout << "_turnDirection: " << _turnDirection << endl;
      cout << "_turnDegrees: " << _turnDegrees << endl;
      cout << "_speedKnots: " << _speedKnots << endl;
      cout << "_speedMPH: " << _speedMPH << endl;
      cout << "_headingDeviation: " << _headingDeviation << endl;
      cout << "_milesToDestination: " << _milesToDestination << endl;
      cout << "_goBackSecondsSpeedCalculation: " << _goBackSecondsSpeedCalculation << endl;
      cout << "_usingGoBackSecondsSpeedCalculation: " << _usingGoBackSecondsSpeedCalculation << endl;
      cout << "_hdop: " << _hdop
              << " (Lower hdop indicates satellite position for better overall position fix)" << endl;
      cout << "_pdop: " << _pdop << " (Lower pdop indicates satellite position for better position Horizontal fix)" << endl;
    }

  }

  TurnData Navigation::getTurnData() {
    lock_guard<mutex> lock(nav_mutex);
    TurnData turnData;
    if (_latitude > 0) {
      _destHeading = calculateBearing(_latitude, _longitude, _destLatitude, _destLongitude);
      calculateDistance(_latitude, _longitude, _destLatitude, _destLongitude);
      _headingDeviation = _destHeading - _heading;
      if (_heading - _destHeading < 0) {
        _lHTurn = _heading - _destHeading + 360;
      } else {
        _lHTurn = _heading - _destHeading;
      }
      if (_destHeading - _heading < 0) {
        _rHTurn = _destHeading - _heading + 360;
      } else {
        _rHTurn = _destHeading - _heading;
      }

      if (_speedMPH > _config->MIN_SPEED_TO_RUN_STEERING && _headingDeviation > _config->DEGREES_TURN_SIZE_XS) {
        if (_rHTurn < _lHTurn) {
          _turnDirection = "RIGHT";
          turnData.direction = "RIGHT";
          turnData.degrees = _rHTurn;
        } else {
          _turnDirection = "LEFT";
          turnData.direction = "LEFT";
          turnData.degrees = _lHTurn;
        }
      } else {
        _turnDirection = "NONE";
        turnData.direction = "NONE";
        turnData.degrees = 0.0;
      }

    } else {
      _turnDirection = "NONE";
      turnData.direction = "NONE";
      turnData.degrees = 0.0;
    }
    return turnData;
  }

  float Navigation::degreesToRadians(float degrees) {
    return degrees * (PI / 180.0);
  }

  void Navigation::CalculateNavigation() {

    if (_latitude > 0) {
      _destHeading = calculateBearing(_latitude, _longitude, _destLatitude, _destLongitude);
      _milesToDestination = calculateDistance(_latitude, _longitude, _destLatitude, _destLongitude);
      _headingDeviation = _destHeading - _heading;
      if (_heading - _destHeading < 0) {
        _lHTurn = _heading - _destHeading + 360;
      } else {
        _lHTurn = _heading - _destHeading;
      }
      if (_destHeading - _heading < 0) {
        _rHTurn = _destHeading - _heading + 360;
      } else {
        _rHTurn = _destHeading - _heading;
      }
      if (_speedMPH > 1.0) {
        if (_rHTurn < _lHTurn) {
          _turnDirection = "RIGHT";
          _turnDegrees = _rHTurn;
        } else {
          _turnDirection = "LEFT";
          _turnDegrees = _lHTurn;
        }
      } else {
        _turnDirection = "NONE";
        _turnDegrees = 0.0;
      }
    }
  }

  NavData Navigation::getNavData() {
    lock_guard<mutex> lock(nav_mutex);
    NavData navData;
    navData._latitude = _latitude;
    navData._longitude = _longitude;
    navData._heading = _heading;
    navData._destHeading = _destHeading;
    navData._destLatitude = _destLatitude;
    navData._destLongitude = _destLongitude;
    navData._rHTurn = _rHTurn;
    navData._lHTurn = _lHTurn;
    navData._turnDirection = _turnDirection;
    navData._turnDegrees = _turnDegrees;
    navData._stepsPerCorrection = _stepsPerCorrection;
    navData._speedKnots = _speedKnots;
    navData._speedMPH = _speedMPH;
    navData._headingDeviation = _headingDeviation;
    navData._milesToDestination = _milesToDestination;
    navData._goBackSecondsSpeedCalculation = _goBackSecondsSpeedCalculation;
    navData._usingGoBackSecondsSpeedCalculation = _usingGoBackSecondsSpeedCalculation;
    navData._hdop = _hdop;
    navData._pdop = _pdop;
    return navData;

  }

  float Navigation::calculateDistance(const float lat1, const float lon1, const float lat2, const float lon2) {
    // Convert latitude and longitude to radians
    float lat1Rad = degreesToRadians(lat1);
    float lon1Rad = degreesToRadians(lon1);
    float lat2Rad = degreesToRadians(lat2);
    float lon2Rad = degreesToRadians(lon2);
    // Calculate the differences
    float latDiff = lat2Rad - lat1Rad;
    float lonDiff = lon2Rad - lon1Rad;
    // Apply the Haversine formula
    float a = sin(latDiff / 2.0) * sin(latDiff / 2.0) +
            cos(lat1Rad) * cos(lat2Rad) *
            sin(lonDiff / 2.0) * sin(lonDiff / 2.0);
    float c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    // Calculate the distance in kilometers
    float distanceKm = EARTH_RADIUS_KM * c;
    // Convert kilometers to miles
    float distanceMiles = distanceKm * MILES_PER_KM;
    return distanceMiles;
  }

  float Navigation::calculateBearing(const float lat1, const float lon1, const float lat2, const float lon2) {
    float thetaA = lat1 * PI / 180.0;
    float thetaB = lat2 * PI / 180.0;
    float deltaL = (lon2 - lon1) * PI / 180.0;
    float X = cos(thetaB) * sin(deltaL);
    float Y = cos(thetaA) * sin(thetaB) - sin(thetaA) * cos(thetaB) * cos(deltaL);
    float beta = atan2(X, Y);
    beta = beta * 180.0 / PI; // Convert to degrees
    if (beta < 0.0) {
      beta += 360.0;
    }
    return beta;
  }

  void Navigation::pushLocationHistoryToFront(const LatLong & element) {
    for (int i = MAX_SIZE - 2; i >= 0; i--) {
      latLongArray[i + 1] = latLongArray[i];
    }
    latLongArray[0] = element;
  }

  void Navigation::setDestination(float Latitude, float Longitude) {
    lock_guard<mutex> lock(nav_mutex);
    this->_destLatitude = Latitude;
    this->_destLongitude = Longitude;
  }

}









