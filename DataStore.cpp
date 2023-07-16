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

  map<int, WayPoint> DataStore::getWayPoints() {
    map<int, WayPoint> WayPointsMap;
    // Get a connection from the connection pool
    sql::Connection* conn = connectionPool_->getConnection();

    try {
      // Create a SQL statement

      sql::Statement* stmt = conn->createStatement();
      string query = "SELECT * FROM waypoints";

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
      cerr << "SQL error: " << e.what() << endl;
    }

    // Release the connection back to the connection pool
    connectionPool_->releaseConnection(conn);

    return WayPointsMap;
  }

  WayPoint DataStore::updateWayPoint(WayPoint wayPoint) {
    sql::Connection* conn = connectionPool_->getConnection();
    sql::PreparedStatement* pstmt;
   
    pstmt = conn->prepareStatement("update waypoints SET name = ?, "
            "description = ?, latitude = ?, longitude = ?, depth = ? "
            "WHERE id = ?");
    pstmt->setString(1, wayPoint.name);
    pstmt->setString(2, wayPoint.description);
    pstmt->setDouble(3, wayPoint.latitude);
    pstmt->setDouble(4, wayPoint.longitude);
    pstmt->setDouble(5, wayPoint.depth);
    pstmt->setInt(6, wayPoint.id);
    if(!(pstmt->executeUpdate())){
      cout << "Error executing update" << endl;
    }
    cout << "Ok to here" << endl;
    return wayPoint;
  }

  int DataStore::insertWayPoint(WayPoint wayPoint) {
    sql::Connection* conn = connectionPool_->getConnection();
    sql::PreparedStatement* pstmt;
    sql::ResultSet *res;
    pstmt = conn->prepareStatement("insert into waypoints (name, "
            " description, latitude, longitude, depth) values "
            "(?,?,?,?,?) ");
    pstmt->setString(1, wayPoint.name);
    pstmt->setString(2, wayPoint.description);
    pstmt->setDouble(3, wayPoint.latitude);
    pstmt->setDouble(4, wayPoint.longitude);
    pstmt->setDouble(5, wayPoint.depth);

    if(!(pstmt->executeUpdate())){
      cout << "Error executing insert" << endl;
    }
    pstmt = conn->prepareStatement("select last_insert_id();");
    res = pstmt->executeQuery();

    // Get the insert ID
    int insertID = 0;
    if (res->next()) {
      insertID = res->getInt(1);
    }

    return insertID;
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
