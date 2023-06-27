#include "GPSUtils.h"

namespace PaulNovack {

  GPSUtils::GPSUtils() {
  }

  GPSUtils::GPSUtils(const GPSUtils& orig) {
  }

  GPSUtils::~GPSUtils() {
  }

  string GPSUtils::trim_leading_zeros(string str) {
    while (str.length() > 0 && str[0] == '0') {
      str.erase(0, 1);
    }
    return str;
  }

  float GPSUtils::getDecimalDegreeFromGPS(string value, string hemisphere) {
    if (value == "")return 0.0;
    if (value.length() < 6)return 0.00;
    value = trim_leading_zeros(value);
    string degreesStr = value.substr(0, 2);
    string minutesStr = value.substr(2);
    if (degreesStr.length() < 2 || minutesStr.length() < 5)return 0.0;
    float degrees = stof(degreesStr);
    float minutes = stof(minutesStr);
    if (hemisphere == "S" || hemisphere == "W") {
      return -(degrees + (minutes / 60.0));
    }
    return ( degrees + (minutes / 60.0));
  }

  float GPSUtils::degreesToRadians(float degrees) {
    return degrees * (PaulNovack::PI / 180.0);
  }

  float GPSUtils::calculateDistanceM(const float lat1, const float lon1, const float lat2, const float lon2) {
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
    float distanceKm = PaulNovack::EARTH_RADIUS_KM * c;
    // Convert kilometers to miles
    float distanceMiles = distanceKm * PaulNovack::MILES_PER_KM;
    return distanceMiles;
  }

  float GPSUtils::calculateMPH(int secondsElapsed, float lat1, float lon1,
          float lat2, float lon2) {
    float distance = calculateDistanceM(lat1, lon1, lat2, lon2);
    float hoursElapsed = secondsElapsed / 3600.0;
    return distance / hoursElapsed;
  }

  float GPSUtils::calculateDistanceNM(const float lat1, const float lon1, const float lat2, const float lon2) {
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
    float distanceKm = PaulNovack::EARTH_RADIUS_KM * c;
    // Convert kilometers to miles
    float distanceMiles = distanceKm * PaulNovack::NAUTICAL_MILES_PER_KM;
    return distanceMiles;
  }

  float GPSUtils::calculateNMPH(int secondsElapsed, float lat1, float lon1,
          float lat2, float lon2) {
    float distance = calculateDistanceNM(lat1, lon1, lat2, lon2);
    float hoursElapsed = secondsElapsed / 3600.0;
    return distance / hoursElapsed;
  }

}


