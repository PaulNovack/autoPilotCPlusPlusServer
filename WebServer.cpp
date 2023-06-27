#include "WebServer.h"

namespace PaulNovack {

  class Navigation;

  WebServer::WebServer() {

  }

  void WebServer::setNavigation(Navigation& navigation) {
    _navigation = &navigation;
  }

  void WebServer::Run() {

    crow::SimpleApp app;
    app.loglevel(crow::LogLevel::ERROR);
    CROW_ROUTE(app, "/getNavigationData")
            .methods("GET"_method)
            ([this] {
              crow::json::wvalue x({});
              NavData navData = _navigation->getNavData();
              x["destHeading"] = navData._destHeading;
              x["destLatitude"] = navData._destLatitude;
              x["destLongitude"] = navData._destLongitude;
              x["goBackSecondsSpeedCalculation"] = navData._goBackSecondsSpeedCalculation;
              x["hdop"] = navData._hdop;
              x["heading"] = navData._heading;
              x["headingDeviation"] = navData._headingDeviation;
              x["lHTurn"] = navData._lHTurn;
              x["latitude"] = navData._latitude;
              x["longitude"] = navData._longitude;
              x["milesToDestination"] = navData._milesToDestination;
              x["pdop"] = navData._pdop;
              x["rHTurn"] = navData._rHTurn;
              x["speedMPH"] = navData._speedMPH;
              x["stepsPerCorrection"] = navData._stepsPerCorrection;
              x["turnDegrees"] = navData._turnDegrees;
              return x;
            });

    CROW_ROUTE(app, "/setWaypoint")
            .methods("POST"_method)
            ([](const crow::request& req, crow::response & res) {
              try {
                // Parse the JSON data from the request body
                json jsonData = json::parse(req.body);

                string name = jsonData["name"];
                string description = jsonData["description"];
                float latitude = jsonData["latitude"];
                float longitude = jsonData["longitude"];

                // Create a JSON response with the extracted values
                json response;
                response["name"] = name;
                response["description"] = description;
                response["latitude"] = latitude;
                response["longitude"] = longitude;

                // Set the response body with the JSON data
                res.body = response.dump();

                // Set the Content-Type header to application/json
                res.add_header("Content-Type", "application/json");

                // Set the response status to 200 OK
                res.code = 200;
              } catch (const std::exception& e) {
                // Handle any parsing errors or other exceptions
                res.code = 400;
                res.body = "Error: " + std::string(e.what());
              }

              // Send the response
              res.end();
            });

    CROW_ROUTE(app, "/setDestination")
            .methods("POST"_method)
            ([](const crow::request& req, crow::response & res) {
              try {
                // Parse the JSON data from the request body
                json jsonData = json::parse(req.body);

                // Extract the destinationLatitude and destinationLongitude values
                float destinationLatitude = jsonData["destinationLatitude"];
                float destinationLongitude = jsonData["destinationLongitude"];

                // Create a JSON response with the extracted values
                json response;
                response["destinationLatitude"] = destinationLatitude;
                response["destinationLongitude"] = destinationLongitude;

                // Set the response body with the JSON data
                res.body = response.dump();

                // Set the Content-Type header to application/json
                res.add_header("Content-Type", "application/json");

                // Set the response status to 200 OK
                res.code = 200;
              } catch (const std::exception& e) {
                // Handle any parsing errors or other exceptions
                res.code = 400;
                res.body = "Error: " + std::string(e.what());
              }

              // Send the response
              res.end();
            });

    CROW_ROUTE(app, "/getWaypoints")([this]() {
      return "getWaypoints to do gets from database stored waypoints.....";
    });
    CROW_ROUTE(app, "/findWaypoints/<string>")([this]( string search) {
      return "getWaypoints to do gets from database stored waypoints.....";
    });
    CROW_ROUTE(app, "/")([this]() {
      return "AutoPilot Server.  Nothing here routes return JSON for UI.";
    });

    app.port(18080).run();
  }

  WebServer::WebServer(const WebServer& orig) {
  }

  WebServer::~WebServer() {
  }
}
