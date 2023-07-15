#include "DataStore.h"


namespace PaulNovack {

  DataStore::DataStore(const AppConfig& config) {
    connectionPool_ = new MySQLConnectionPool(config.DB_HOST,
            config.DB_USERNAME,
            config.DB_PASSWORD,
            config.DB_DATABASE_NAME,
            config.DB_POOL_SIZE,
            config.DB_HEARTBEAT_INTERVAL);
  }

  DataStore::DataStore(const DataStore& orig) {
  }

  DataStore::~DataStore() {
  }

  std::map<int, WayPoint> DataStore::getWayPoints() {
    std::map<int, WayPoint> WayPointsMap;
    // Get a connection from the connection pool
    sql::Connection* conn = connectionPool_->getConnection();

    try {
      // Create a SQL statement

      sql::Statement* stmt = conn->createStatement();
      std::string query = "SELECT * FROM waypoints";

      // Execute the query
      sql::ResultSet* resultSet = stmt->executeQuery(query);

      while (resultSet->next()) {
        WayPoint wp;
        wp.id = resultSet->getInt("id");
        wp.name = resultSet->getString("name");
        wp.description = resultSet->getString("description");
        wp.latitude = resultSet->getDouble("latitude");
        wp.longitude = resultSet->getDouble("longitude");
        wp.depth = resultSet->getDouble("depth");
        WayPointsMap[wp.id] = wp;
      }

      // Clean up
      delete resultSet;
      delete stmt;
    } catch (const sql::SQLException& e) {
      std::cerr << "SQL error: " << e.what() << std::endl;
    }

    // Release the connection back to the connection pool
    connectionPool_->releaseConnection(conn);

    return WayPointsMap;
  }

  WayPoint DataStore::insertWayPoint(WayPoint wayPoint) {
    sql::Connection* conn = connectionPool_->getConnection();
    sql::PreparedStatement* pstmt;
    WayPoint wp;

    pstmt = conn->prepareStatement("UPDATE boxes SET user_id = ?, "
            "name = ?, weight = ?, picture = ?, created_at = ? "
            "WHERE id = ?");
    //  pstmt->setInt(1, root["user_id"].asInt64);
    //   pstmt->setString(2, root["name"].asString());
    //   pstmt->setfloat(3, root["weight"].asfloat());
    //  pstmt->setString(4, root["picture"].asString());
    //   pstmt->setString(5, root["created_at"].asString());
    //   pstmt->setInt64(6, root["id"].asInt64);
    // pstmt->executeUpdate();
    return wp;
  }

  WayPoint DataStore::updateWayPoint(WayPoint wayPoint) {
    sql::Connection* conn = connectionPool_->getConnection();
    sql::PreparedStatement* pstmt;
    WayPoint wp;

    pstmt = conn->prepareStatement("UPDATE boxes SET user_id = ?, "
            "name = ?, weight = ?, picture = ?, created_at = ? "
            "WHERE id = ?");
    //  pstmt->setInt(1, root["user_id"].asInt64);
    //   pstmt->setString(2, root["name"].asString());
    //   pstmt->setfloat(3, root["weight"].asfloat());
    //  pstmt->setString(4, root["picture"].asString());
    //   pstmt->setString(5, root["created_at"].asString());
    //   pstmt->setInt64(6, root["id"].asInt64);
    // pstmt->executeUpdate();
    return wp;
  }

  bool DataStore::deleteWayPoint(WayPoint wayPoint) {
    sql::Connection* conn = connectionPool_->getConnection();
    sql::PreparedStatement* pstmt;
    WayPoint wp;

    pstmt = conn->prepareStatement("UPDATE boxes SET user_id = ?, "
            "name = ?, weight = ?, picture = ?, created_at = ? "
            "WHERE id = ?");
    //  pstmt->setInt(1, root["user_id"].asInt64);
    //   pstmt->setString(2, root["name"].asString());
    //   pstmt->setfloat(3, root["weight"].asfloat());
    //  pstmt->setString(4, root["picture"].asString());
    //   pstmt->setString(5, root["created_at"].asString());
    //   pstmt->setInt64(6, root["id"].asInt64);
    // pstmt->executeUpdate();
    return true;
  }
} 
