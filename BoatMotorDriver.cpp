#include "BoatMotorDriver.h"

namespace PaulNovack {

  BoatMotorDriver::BoatMotorDriver() {
    std::cout << "BoatMotorDriver Initialized" << std::endl;
    std::cout << "Initializing connection with Arduino....." << std::endl;
    GetStepperController();
  }
  void BoatMotorDriver::setConfig(AppConfig& config) {
    _config = &config;
  }
  void BoatMotorDriver::GetStepperController() {
    // we don't know what port arduino is on
    // it depends on what we plug in first mostly
    // the arduino or the GPS receiver whichever is first will
    // probably get /dev/ttyACM0 and the second plugged in /dev/ttyACM1
    bool gotPort = false;
    for (int portNum = 0; portNum < 5; portNum++) {
      std::string port = "/dev/ttyACM" + std::to_string(portNum);
      int sp = open(port.c_str(), O_RDWR);
      std::string sysCommand = "stty -F " + port + " sane raw -echo 115200";
      system(sysCommand.c_str());
      sleep(2);
      unsigned char c = 'F';
      write(sp, &c, 1);
      while (read(sp, &c, 1)) {
        if (c == 'K') {
          this->motorControllerPort = port;
          std::cout << "Arduino MotorController is on port " << port << std::endl;
          gotPort = true;
          break;
        } else {
          std::cout << "Arduino is not on port " << port << std::endl;
          gotPort = false;
          break;
        }
        std::cout << c;
      }
      if (gotPort) {
        break;
      }
    }
  }

  BoatMotorDriver::BoatMotorDriver(const BoatMotorDriver & orig) {
  }

  BoatMotorDriver::~BoatMotorDriver() {
  }

  void BoatMotorDriver::setNavigation(Navigation & navigation) {
    this->navigation = &navigation;
  }

  void BoatMotorDriver::runCheck() {
    while (true) {
      std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      std::cout << "Running Check at " << _config->CHECK_TURN_TIME << " second intervals if Turn is needed (runCheck)" << std::endl;
      TurnData turnData = this->navigation->getTurnData();
      this->degrees = turnData.degrees;
      this->direction = turnData.direction;
      if (turnData.direction == "LEFT") {
        this->turn("L", turnData.degrees);
      } else if (turnData.direction == "RIGHT") {
        this->turn("R", turnData.degrees);
      } else {
        std::cout << "++++++++++++++++++++++++++++++++++++ NO TURN ++++++++++++++++++++++++++++++++++++" << std::endl;
      }
      std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
      std::this_thread::sleep_for(std::chrono::seconds(_config->CHECK_TURN_TIME));
    }


  }

  void BoatMotorDriver::turn(std::string direction, float degreesDeviation) {
    int turnSize = 0;
    if (degreesDeviation > _config->DEGREES_TURN_SIZE_XL) {
      turnSize = _config->TURN_SIZE_XL;
    } else if (degreesDeviation > _config->DEGREES_TURN_SIZE_LG) {
      turnSize = _config->TURN_SIZE_LG;
    } else if (degreesDeviation > _config->DEGREES_TURN_SIZE_MD) {
      turnSize = _config->TURN_SIZE_MD;
    } else if (degreesDeviation > _config->DEGREES_TURN_SIZE_SM) {
      turnSize = _config->TURN_SIZE_SM;
    } else if (degreesDeviation > _config->DEGREES_TURN_SIZE_XS) {
      turnSize = _config->TURN_SIZE_XS;
    }

    std::cout << "===== Motor Controller will Turn " << direction << " Steps to Turn: "
            << turnSize << std::endl;
    Drive(direction.c_str()[0], turnSize);
  }

  void BoatMotorDriver::Drive(char direction, int steps) {
    // Open the port
    std::ofstream port(this->motorControllerPort);
    std::cout << "Sending to Arduino Direction: " << direction << std::endl;
    std::cout << "Sending to Arduino Steps: " << steps << std::endl;
    if (port) {
      // Send "F" to the port
      port << direction << steps << std::endl;
      port.close();

      // Open the port for reading
      std::ifstream response(this->motorControllerPort);

      if (response) {
        response.close();
      }
    }
  }

}