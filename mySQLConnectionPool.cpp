#include "mySQLConnectionPool.h"
#include <iostream>
#include <chrono>

MySQLConnectionPool::MySQLConnectionPool(const std::string& host,
        const std::string& user,
        const std::string& password,
        const std::string& database,
        int poolSize,
        int heartbeatInterval)
: host_(host),
user_(user),
password_(password),
database_(database),
poolSize_(poolSize),
heartbeatInterval_(heartbeatInterval) {
  driver_ = sql::mysql::get_mysql_driver_instance();
  initializePool();
  startHeartbeat();
}

MySQLConnectionPool::~MySQLConnectionPool() {
  stopHeartbeat();
  for (auto& conn : connectionPool_) {
    delete conn;
  }
}

sql::Connection* MySQLConnectionPool::getConnection() {
  std::unique_lock<std::mutex> lock(mutex_);
  while (connectionPool_.empty()) {
    condition_.wait(lock);
  }

  sql::Connection* conn = connectionPool_.back();
  connectionPool_.pop_back();

  // Check connection validity
  if (!conn->isValid()) {
    delete conn;
    conn = driver_->connect(host_, user_, password_);
    conn->setSchema(database_);
    connectionPool_.push_back(conn);
  }

  return conn;
}

void MySQLConnectionPool::releaseConnection(sql::Connection* conn) {
  std::unique_lock<std::mutex> lock(mutex_);
  connectionPool_.push_back(conn);
  condition_.notify_one();
}

void MySQLConnectionPool::initializePool() {
  for (int i = 0; i < poolSize_; ++i) {
    sql::Connection* conn = driver_->connect(host_, user_, password_);
    conn->setSchema(database_);
    connectionPool_.push_back(conn);
  }
  std::cout << "Initialize Pool" << std::endl;
}

void MySQLConnectionPool::startHeartbeat() {
  heartbeatThread_ = std::thread([this]() {
    while (heartbeatRunning_) {
      std::this_thread::sleep_for(std::chrono::seconds(heartbeatInterval_));
      checkConnections();
    }
  });
  std::cout << "Started Heartbeat" << std::endl;
}

void MySQLConnectionPool::stopHeartbeat() {
  if (heartbeatThread_.joinable()) {
    heartbeatRunning_ = false;
    heartbeatThread_.join();
  }
}

void MySQLConnectionPool::checkConnections() {
  std::lock_guard<std::mutex> lock(mutex_);
  for (auto& conn : connectionPool_) {
    if (!conn->isValid()) {
      delete conn;
      conn = driver_->connect(host_, user_, password_);
      conn->setSchema(database_);
      std::cout << "Reconnected Connection" << std::endl;
    }
    std::cout << "Checked Connection" << std::endl;
  }
  std::cout << "Checked all Connections" << std::endl;
}
