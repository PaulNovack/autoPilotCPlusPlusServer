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

class MySQLConnectionPool {
public:
    MySQLConnectionPool(const std::string& host, const std::string& user, const std::string& password, const std::string& database, int poolSize, int heartbeatInterval);
    ~MySQLConnectionPool();
    sql::Connection* getConnection();
    void releaseConnection(sql::Connection* conn);


private:
    void initializePool();
    void startHeartbeat();
    void stopHeartbeat();
    void checkConnections();

private:
    std::string host_;
    std::string user_;
    std::string password_;
    std::string database_;
    int poolSize_;
    int heartbeatInterval_;
    sql::mysql::MySQL_Driver* driver_;
    std::vector<sql::Connection*> connectionPool_;
    std::mutex mutex_;
    std::condition_variable condition_;
    std::thread heartbeatThread_;
    bool heartbeatRunning_ = true;
};


#endif
