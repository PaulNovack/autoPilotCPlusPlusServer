#ifndef DATASTORE_H
#define DATASTORE_H

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <string>
#include <cmath>
#include <map>
#include <fstream>
#include <string>
#include <iostream>
#include <mutex>
#include "GPSParser.h"
#include "PaulNovackGlobals.h"
#include "GPSUtils.h"
#include "json.hpp"
#include "AppConfig.h"
#include "mySQLConnectionPool.h"
namespace PaulNovack {

    class DataStore {
    public:
        DataStore(const AppConfig& config);

        DataStore(const DataStore& orig);
        virtual ~DataStore();
        std::map<int, WayPoint> getWayPoints();
        WayPoint insertWayPoint(WayPoint wayPoint);
        WayPoint updateWayPoint(WayPoint wayPoint);
        bool deleteWayPoint(WayPoint wayPoint);
    private:
        MySQLConnectionPool* connectionPool_;
    };
}
#endif /* DATASTORE_H */

