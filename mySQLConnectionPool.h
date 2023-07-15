#ifndef MYSQL_CONNECTION_POOL_H
#define MYSQL_CONNECTION_POOL_H

#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/resultset.h>
#include <boost/variant.hpp>

using namespace std;
using namespace sql;

class MySQLConnectionPool {
public:
    MySQLConnectionPool(const string& host, const string& user, const string& password, const string& database, int poolSize, int heartbeatInterval);
    ~MySQLConnectionPool();
    Connection* getConnection();
    void releaseConnection(Connection* conn);


private:
    void initializePool();
    void startHeartbeat();
    void stopHeartbeat();
    void checkConnections();

private:
    string host_;
    string user_;
    string password_;
    string database_;
    int poolSize_;
    int heartbeatInterval_;
    sql::mysql::MySQL_Driver* driver_;
    vector<sql::Connection*> connectionPool_;
    mutex mutex_;
    condition_variable condition_;
    thread heartbeatThread_;
    bool heartbeatRunning_ = true;
};

#endif
