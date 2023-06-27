#include "State_route.h"

crow::response setState(const crow::request& req) {
    crow::json::wvalue response;

    sql::Connection* conn = connectionPool.getConnection();

    try {
        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM boxes"));

        while (res->next()) {
            int id = res->getInt("id");
            std::string name = res->getString("name");
            // Add the retrieved data to the JSON response
            response[name.c_str()] = id;
        }
    } catch (sql::SQLException& e) {
        response["error"] = e.what();
    }

    connectionPool.releaseConnection(conn);

    return crow::response(response);
}
