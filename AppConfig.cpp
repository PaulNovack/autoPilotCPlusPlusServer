#include "AppConfig.h"


namespace PaulNovack {

  AppConfig::AppConfig(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
      cerr << "Failed to open .env file." << endl;
      return;
    }

    string line;
    while (getline(file, line)) {
      size_t delimiterPos = line.find('=');
      if (delimiterPos != string::npos) {
        string key = line.substr(0, delimiterPos);
        string value = line.substr(delimiterPos + 1);

        if (key == "GO_BACK_SECONDS_FOR_SPEED")
          GO_BACK_SECONDS_FOR_SPEED = stoi(value);
        else if (key == "MIN_SPEED_TO_RUN_STEERING")
          MIN_SPEED_TO_RUN_STEERING = stof(value);
        else if (key == "APP_ROOT")
          APP_ROOT = value;
        else if (key == "DEGREES_TURN_SIZE_XL")
          DEGREES_TURN_SIZE_XL = stof(value);
        else if (key == "DEGREES_TURN_SIZE_LG")
          DEGREES_TURN_SIZE_LG = stof(value);
        else if (key == "DEGREES_TURN_SIZE_MD")
          DEGREES_TURN_SIZE_MD = stof(value);
        else if (key == "DEGREES_TURN_SIZE_SM")
          DEGREES_TURN_SIZE_SM = stof(value);
        else if (key == "DEGREES_TURN_SIZE_XS")
          DEGREES_TURN_SIZE_XS = stof(value);
        else if (key == "TURN_SIZE_XL")
          TURN_SIZE_XL = stoi(value);
        else if (key == "TURN_SIZE_LG")
          TURN_SIZE_LG = stoi(value);
        else if (key == "TURN_SIZE_MD")
          TURN_SIZE_MD = stoi(value);
        else if (key == "TURN_SIZE_SM")
          TURN_SIZE_SM = stoi(value);
        else if (key == "TURN_SIZE_XS")
          TURN_SIZE_XS = stoi(value);
        else if (key == "CHECK_TURN_TIME")
          CHECK_TURN_TIME = stoi(value);
        else if (key == "MAX_TURN_STEPS")
          MAX_TURN_STEPS = stoi(value);
        else if (key == "STEPS_PER_CORRECTION")
          STEPS_PER_CORRECTION = stoi(value);
        else if (key == "DB_DATABASE_NAME")
          DB_DATABASE_NAME = value;
        else if (key == "DB_USERNAME")
          DB_USERNAME = value;
        else if (key == "DB_PASSWORD")
          DB_PASSWORD = value;
        else if (key == "DB_HOST")
          DB_HOST = value;
        else if (key == "DB_HEARTBEAT_INTERVAL")
          DB_HEARTBEAT_INTERVAL = stoi(value);
        else if (key == "DB_POOL_SIZE")
          DB_POOL_SIZE = stoi(value);

      }
    }
    file.close();
  }
}