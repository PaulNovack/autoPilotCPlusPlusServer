#include "AppConfig.h"
#include <iostream>
#include <fstream>

namespace PaulNovack {

  AppConfig::AppConfig(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
      std::cerr << "Failed to open .env file." << std::endl;
      return;
    }

    std::string line;
    while (std::getline(file, line)) {
      size_t delimiterPos = line.find('=');
      if (delimiterPos != std::string::npos) {
        std::string key = line.substr(0, delimiterPos);
        std::string value = line.substr(delimiterPos + 1);

        if (key == "GO_BACK_SECONDS_FOR_SPEED")
          GO_BACK_SECONDS_FOR_SPEED = std::stoi(value);
        else if (key == "MIN_SPEED_TO_RUN_STEERING")
          MIN_SPEED_TO_RUN_STEERING = std::stof(value);
        else if (key == "APP_ROOT")
          APP_ROOT = value;
        else if (key == "DEGREES_TURN_SIZE_XL")
          DEGREES_TURN_SIZE_XL = std::stof(value);
        else if (key == "DEGREES_TURN_SIZE_LG")
          DEGREES_TURN_SIZE_LG = std::stof(value);
        else if (key == "DEGREES_TURN_SIZE_MD")
          DEGREES_TURN_SIZE_MD = std::stof(value);
        else if (key == "DEGREES_TURN_SIZE_SM")
          DEGREES_TURN_SIZE_SM = std::stof(value);
        else if (key == "DEGREES_TURN_SIZE_XS")
          DEGREES_TURN_SIZE_XS = std::stof(value);
        else if (key == "TURN_SIZE_XL")
          TURN_SIZE_XL = std::stoi(value);
        else if (key == "TURN_SIZE_LG")
          TURN_SIZE_LG = std::stoi(value);
        else if (key == "TURN_SIZE_MD")
          TURN_SIZE_MD = std::stoi(value);
        else if (key == "TURN_SIZE_SM")
          TURN_SIZE_SM = std::stoi(value);
        else if (key == "TURN_SIZE_XS")
          TURN_SIZE_XS = std::stoi(value);
        else if (key == "CHECK_TURN_TIME")
          CHECK_TURN_TIME = std::stoi(value);
        else if (key == "MAX_TURN_STEPS")
          MAX_TURN_STEPS = std::stoi(value);
        else if (key == "STEPS_PER_CORRECTION")
          STEPS_PER_CORRECTION = std::stoi(value);
        else if (key == "DB_DATABASE_NAME")
          DB_DATABASE_NAME = value;
        else if (key == "DB_USERNAME")
          DB_USERNAME = value;
        else if (key == "DB_PASSWORD")
          DB_PASSWORD = value;
        else if (key == "DB_HOST")
          DB_HOST = value;
        else if (key == "DB_HEARTBEAT_INTERVAL")
          DB_HEARTBEAT_INTERVAL = std::stoi(value);
        else if (key == "DB_POOL_SIZE")
          DB_POOL_SIZE = std::stoi(value);

      }
    }
    file.close();
  }
}