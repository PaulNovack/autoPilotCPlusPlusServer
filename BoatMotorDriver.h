#ifndef BOATMOTORDRIVER_H
#define BOATMOTORDRIVER_H

#include "Navigation.h"
#include <iostream>
#include <fstream>
#include <ostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "PaulNovackGlobals.h"
#include "AppConfig.h"

using namespace std;

namespace PaulNovack {
    class Navigation;
    
    class AppConfig;

    class BoatMotorDriver {
    public:
        BoatMotorDriver();
        BoatMotorDriver(const BoatMotorDriver& orig);
        virtual ~BoatMotorDriver();
        void setNavigation(Navigation& navigation);
        void runCheck();
        void turn(std::string direction, float degreesDeviation);
        void setConfig(AppConfig& config);
        AppConfig* _config = nullptr;
    private:
        void turnRight();
        void turnLeft();
        Navigation *navigation = nullptr;
        float degrees;
        std::string direction;
        std::string motorControllerPort;
        void GetStepperController();
        void Drive(char direction, int steps);

    };
}
#endif /* BOATMOTORDRIVER_H */

