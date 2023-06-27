#include "WayPoints.h"


// Deserialization from JSON
void Boxes::fromJson(const std::string& jsonStr) {
    Json::Value root;
    std::istringstream iss(jsonStr);
    Json::CharReaderBuilder jsonReaderBuilder;
    std::unique_ptr<Json::CharReader> jsonReader(jsonReaderBuilder.newCharReader());
    std::string errs;
    if (!jsonReader->parse(jsonStr.c_str(), jsonStr.c_str() + jsonStr.length(), &root, &errs)) {
        std::cerr << "Error parsing JSON: " << errs << std::endl;
        return;
    }

    id = root["id"].asInt64();
    user_id = root["user_id"].asInt64();
    name = root["name"].asString();
    weight = root["weight"].asDouble();
    picture = root["picture"].asString();
    created_at = root["created_at"].asString();
}


// Generate SQL statement for loading a record
std::string Boxes::loadStatement() const {
    std::ostringstream oss;
    oss << "SELECT * FROM boxes WHERE id = " << id;
    return oss.str();
}

// Generate SQL statement for saving a record
std::string Boxes::saveStatement() const {
    std::ostringstream oss;
    oss << "UPDATE boxes SET "
        << "user_id = " << user_id << ", "
        << "name = '" << name << "', "
        << "weight = " << weight << ", "
        << "picture = '" << picture << "', "
        << "created_at = '" << created_at << "' "
        << "WHERE id = " << id;
    return oss.str();
}

// Generate SQL statement for creating a record
std::string Boxes::createStatement() const {
    std::ostringstream oss;
    oss << "INSERT INTO boxes (user_id, name, weight, picture, created_at) VALUES ("
        << user_id << ", '"
        << name << "', "
        << weight << ", '"
        << picture << "', '"
        << created_at << "')";
    return oss.str();
}

// Generate SQL statement for deleting a record
std::string Boxes::deleteStatement() const {
    std::ostringstream oss;
    oss << "DELETE FROM boxes WHERE id = " << id;
    return oss.str();
}

