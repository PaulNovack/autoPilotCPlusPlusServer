#include "mySQLConnectionPool.h"
#include <iostream>
#include <chrono>

MySQLConnectionPool::MySQLConnectionPool(const std::string& host, const std::string& user, const std::string& password, const std::string& database, int poolSize, int heartbeatInterval) : host_(host), 
user_(user), password_(password), database_(database), poolSize_(poolSize), heartbeatInterval_(heartbeatInterval), driver_(nullptr), connectionPool_(), mutex_(), condition_(), heartbeatThread_() {
    initializePool();
    startHeartbeat();
}

MySQLConnectionPool::~MySQLConnectionPool() {
    stopHeartbeat();
    // Check if the heartbeat thread is still running before joining it
    if (heartbeatThread_.joinable()) {
        heartbeatThread_.join();
    }
}

sql::Connection* MySQLConnectionPool::getConnection() {
    std::lock_guard<std::mutex> lock(mutex_);
    // Check if there are any available connections in the pool
    if (!connectionPool_.empty()) {
        // Return an available connection from the pool
        return connectionPool_.back();
    } else {
        // Create a new connection and add it to the pool
        sql::Connection* conn = driver_->connect(host_, user_, password_);
        if (conn != nullptr) {
            connectionPool_.push_back(conn);
            return conn;
        } else {
            throw std::runtime_error("Failed to create new database connection");
        }
    }
}

void MySQLConnectionPool::releaseConnection(sql::Connection* conn) {
    // Check if the connection is in use before releasing it back to the pool
    if (conn->isInUse()) {
        throw std::runtime_error("Cannot release a connection that is still in use");
    } else {
        // Release the connection back to the pool
        conn->release();
    }
}

void MySQLConnectionPool::initializePool() {
    for (int i = 0; i < poolSize_; i++) {
        sql::Connection* conn = driver_->connect(host_, user_, password_);
        if (conn != nullptr) {
            connectionPool_.push_back(conn);
        } else {
            throw std::runtime_error("Failed to create new database connection");
        }
    }
}

void MySQLConnectionPool::startHeartbeat() {
    heartbeatThread_ = std::thread([this](){
        while (heartbeatRunning_) {
            checkConnections();
            std::this_thread::sleep_for(std::chrono::seconds(heartbeatInterval_));
        }
    });
}

void MySQLConnectionPool::stopHeartbeat() {
    // Set the heartbeat running flag to false
    heartbeatRunning_ = false;
    // Join the heartbeat thread with the main thread
    if (heartbeatThread_.joinable()) {
        heartbeatThread_.join();
    }
}

void MySQLConnectionPool::checkConnections() {
    std::lock_guard<std::mutex> lock(mutex_);
    // Check each connection in the pool and remove any invalid connections
    for (auto it = connectionPool_.begin(); it != connectionPool_.end(); ) {
        if (!(*it)->isValid()) {
            delete *it;
            it = connectionPool_.erase(it);
        } else {
            ++it;
        }
    }
}
