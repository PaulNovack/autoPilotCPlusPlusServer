#include "BoatMotorDriver.h"

namespace PaulNovack {

  BoatMotorDriver::BoatMotorDriver() {
    cout << "BoatMotorDriver Initialized" << endl;
    cout << "Initializing connection with Arduino....." << endl;
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
      string port = "/dev/ttyACM" + to_string(portNum);
      int sp = open(port.c_str(), O_RDWR);
      string sysCommand = "stty -F " + port + " sane raw -echo 115200";
      system(sysCommand.c_str());
      sleep(2);
      unsigned char c = 'F';
      write(sp, &c, 1);
      while (read(sp, &c, 1)) {
        if (c == 'K') {
          this->motorControllerPort = port;
          cout << "Arduino MotorController is on port " << port << endl;
          gotPort = true;
          break;
        } else {
          cout << "Arduino is not on port " << port << endl;
          gotPort = false;
          break;
        }
        cout << c;
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
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      cout << "Running Check at " << _config->CHECK_TURN_TIME << " second intervals if Turn is needed (runCheck)" << endl;
      TurnData turnData = this->navigation->getTurnData();
      this->degrees = turnData.degrees;
      this->direction = turnData.direction;
      if (turnData.direction == "LEFT") {
        this->turn("L", turnData.degrees);
      } else if (turnData.direction == "RIGHT") {
        this->turn("R", turnData.degrees);
      } else {
        cout << "++++++++++++++++++++++++++++++++++++ NO TURN ++++++++++++++++++++++++++++++++++++" << endl;
      }
      cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl;
      this_thread::sleep_for(chrono::seconds(_config->CHECK_TURN_TIME));
    }


  }

  void BoatMotorDriver::turn(string direction, float degreesDeviation) {
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

    cout << "===== Motor Controller will Turn " << direction << " Steps to Turn: "
            << turnSize << endl;
    Drive(direction.c_str()[0], turnSize);
  }

  void BoatMotorDriver::Drive(char direction, int steps) {
    // Open the port
    ofstream port(this->motorControllerPort);
    cout << "Sending to Arduino Direction: " << direction << endl;
    cout << "Sending to Arduino Steps: " << steps << endl;
    if (port) {
      // Send "F" to the port
      port << direction << steps << endl;
      port.close();

      // Open the port for reading
      ifstream response(this->motorControllerPort);

      if (response) {
        response.close();
      }
    }
  }

}