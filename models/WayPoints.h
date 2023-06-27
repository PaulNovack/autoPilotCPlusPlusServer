#ifndef BOXES_H
#define BOXES_H
#include "json.h"
#include <string>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <crow.h>
#include <boost/variant.hpp>
#include "../middleware/mySQLConnectionPool.h"

class Boxes {
public:
    Boxes(MySQLConnectionPool& connectionPool); // Constructor

    void fromJson(const std::string& jsonStr);
    std::string loadStatement() const;
    std::string saveStatement() const;
    std::string createStatement() const;
    std::string deleteStatement() const;
    typedef boost::variant<long long, long long, std::string, double, std::string, std::string> BoxVariant;
    // Getters

    long long getId() const {
        return id;
    }

    long long getUserId() const {
        return user_id;
    }

    std::string getName() const {
        return name;
    }

    double getWeight() const {
        return weight;
    }

    std::string getPicture() const {
        return picture;
    }

    std::string getCreatedAt() const {
        return created_at;
    }

    // Setters

    void setId(long long newId) {
        id = newId;
    }

    void setUserId(long long newUserId) {
        user_id = newUserId;
    }

    void setName(const std::string& newName) {
        name = newName;
    }

    void setWeight(double newWeight) {
        weight = newWeight;
    }

    void setPicture(const std::string& newPicture) {
        picture = newPicture;
    }

    void setCreatedAt(const std::string& newCreatedAt) {
        created_at = newCreatedAt;
    }
private:
    long long id;
    long long user_id;
    std::string name;
    double weight;
    std::string picture;
    std::string created_at;
    MySQLConnectionPool& connectionPool; // Reference to MySQLConnectionPool
};

#endif
