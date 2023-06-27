#pragma once

#include <crow.h>
#include <json.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include "../middleware/mySQLConnectionPool.h"

extern MySQLConnectionPool connectionPool;

crow::response setState(const crow::request& req);