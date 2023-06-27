#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <string>
#include <cmath>
#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include "GPSParser.h"
#include "PaulNovackGlobals.h"
#include "GPSUtils.h"
#include "json.hpp"
#include "AppConfig.h"
#include "Navigation.h"
#include "crow.h"

using namespace std;

namespace PaulNovack {

    class WebServer {
    public:
        WebServer();
        void Run();
        WebServer(const WebServer& orig);
        void setNavigation(Navigation& navigation);
        virtual ~WebServer();
    private:
        Navigation* _navigation;
    };
}

#endif /* WEBSERVER_H */

