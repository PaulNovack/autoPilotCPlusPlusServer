#include "mySQLConnectionPool.h"


MySQLConnectionPool::MySQLConnectionPool(const string& host,
        const string& user,
        const string& password,
        const string& database,
        int poolSize,
        int heartbeatInterval)
: host_(host),
user_(user),
password_(password),
database_(database),
poolSize_(poolSize),
heartbeatInterval_(heartbeatInterval) {
  driver_ = mysql::get_mysql_driver_instance();
  initializePool();
  startHeartbeat();
}

MySQLConnectionPool::~MySQLConnectionPool() {
  stopHeartbeat();
  for (auto& conn : connectionPool_) {
    delete conn;
  }
}

Connection* MySQLConnectionPool::getConnection() {
  unique_lock<mutex> lock(mutex_);
  while (connectionPool_.empty()) {
    condition_.wait(lock);
  }

  Connection* conn = connectionPool_.back();
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

void MySQLConnectionPool::releaseConnection(Connection* conn) {
  unique_lock<mutex> lock(mutex_);
  connectionPool_.push_back(conn);
  condition_.notify_one();
}

void MySQLConnectionPool::initializePool() {
  for (int i = 0; i < poolSize_; ++i) {
    Connection* conn = driver_->connect(host_, user_, password_);
    conn->setSchema(database_);
    connectionPool_.push_back(conn);
  }
  cout << "Initialize Pool" << endl;
}

void MySQLConnectionPool::startHeartbeat() {
  heartbeatThread_ = thread([this]() {
    while (heartbeatRunning_) {
      this_thread::sleep_for(chrono::seconds(heartbeatInterval_));
      checkConnections();
    }
  });
  cout << "Started Heartbeat" << endl;
}

void MySQLConnectionPool::stopHeartbeat() {
  if (heartbeatThread_.joinable()) {
    heartbeatRunning_ = false;
    heartbeatThread_.join();
  }
}

void MySQLConnectionPool::checkConnections() {
  lock_guard<mutex> lock(mutex_);
  for (auto& conn : connectionPool_) {
    if (!conn->isValid()) {
      delete conn;
      conn = driver_->connect(host_, user_, password_);
      conn->setSchema(database_);
      cout << "Reconnected Connection" << endl;
    }
    cout << "Checked Connection" << endl;
  }
  cout << "Checked all Connections" << endl;
}
