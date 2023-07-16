#include "WebServer.h"

namespace crow {
  // Any middleware requires following 3 members:

  // struct context;
  //      storing data for the middleware; can be read from another middleware or handlers

  // before_handle
  //      called before handling the request.
  //      if res.end() is called, the operation is halted. 
  //      (still call after_handle of this middleware)
  //      2 signatures:
  //      void before_handle(request& req, response& res, context& ctx)
  //          if you only need to access this middlewares context.
  //      template <typename AllContext>
  //      void before_handle(request& req, response& res, context& ctx, AllContext& all_ctx)
  //          you can access another middlewares' context by calling `all_ctx.template get<MW>()'
  //          ctx == all_ctx.template get<CurrentMiddleware>()

  // after_handle
  //      called after handling the request.
  //      void after_handle(request& req, response& res, context& ctx)
  //      template <typename AllContext>
  //      void after_handle(request& req, response& res, context& ctx, AllContext& all_ctx)

  struct CORS {

    struct context {
    };

    void before_handle(request& req, response& res, context& ctx) {
      // Handle CORS headers here
      res.add_header("Access-Control-Allow-Origin", "*");
      res.add_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
      res.add_header("Access-Control-Allow-Headers", "Content-Type");
      res.add_header("Access-Control-Max-Age", "86400"); // Optional: Specify max age for preflight requests
      res.add_header("Content-Type", "application/json");
    }

    void after_handle(request& req, response& res, context& ctx) {
    }
  };

}

namespace PaulNovack {

  class Navigation;

  WebServer::WebServer() {

  }

  void WebServer::setNavigation(Navigation& navigation) {
    _navigation = &navigation;
  }

  void WebServer::setDataStore(DataStore& ds) {
    _ds = &ds;
  }

  void WebServer::Run() {

    crow::App<crow::CORS> app;
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
            ([this](const crow::request& req, crow::response & res) {
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
            ([this](const crow::request& req, crow::response & res) {
              try {
                // Parse the JSON data from the request body
                json jsonData = json::parse(req.body);

                float destinationLatitude = jsonData["destinationLatitude"];
                float destinationLongitude = jsonData["destinationLongitude"];
                json response;
                response["destinationLatitude"] = destinationLatitude;
                response["destinationLongitude"] = destinationLongitude;
                _navigation->setDestination(destinationLatitude, destinationLongitude);
                res.body = response.dump();
                res.code = 200;
              } catch (const std::exception& e) {
                // Handle any parsing errors or other exceptions
                res.code = 400;
                res.body = "Error: " + std::string(e.what());
              }

              // Send the response
              res.end();
            });

    CROW_ROUTE(app, "/getWaypoints")([this](const crow::request& req, crow::response & res) {
      std::map<int, WayPoint> waypoints = _ds->getWayPoints();
      json jsonResponse = json::array();
      for (const auto& pair : waypoints) {
        int id = pair.first;
                const WayPoint& waypoint = pair.second;
                json waypointJson;
                waypointJson["id"] = id;
                waypointJson["name"] = waypoint.name;
                waypointJson["description"] = waypoint.description;
                waypointJson["latitude"] = waypoint.latitude;
                waypointJson["longitude"] = waypoint.longitude;
                waypointJson["depth"] = waypoint.depth;
                jsonResponse.push_back(waypointJson);
      } // Set the response body with the JSON data
      res.body = jsonResponse.dump();

      // Set the response status to 200 OK
      res.code = 200;
      res.end();
    });
    CROW_ROUTE(app, "/insertWaypoint")
            .methods("POST"_method)
            ([this](const crow::request& req, crow::response & res) {
              crow::json::rvalue wayPoint = crow::json::load(req.body);

              WayPoint wp;
              // Extract values from the JSON and set variables
              wp.depth = wayPoint["depth"].d();
              wp.description = wayPoint["description"].s();
              wp.latitude = wayPoint["latitude"].d();
              wp.longitude = wayPoint["longitude"].d();
              wp.name = wayPoint["name"].s();
              wp.id = _ds->insertWayPoint(wp);

              json waypointJson;
              waypointJson["depth"] = wp.depth;
              waypointJson["description"] = wp.description;
              waypointJson["latitude"] = wp.latitude;
              waypointJson["longitude"] = wp.longitude;
              waypointJson["name"] = wp.name;
              waypointJson["id"] = wp.id;
              res.body = waypointJson.dump();
              res.code = 200;
              res.end();
            });
    CROW_ROUTE(app, "/updateWaypoint")
            .methods("POST"_method)
            ([this](const crow::request& req, crow::response & res) {
              crow::json::rvalue wayPoint = crow::json::load(req.body);

              WayPoint wp;
              // Extract values from the JSON and set variables
              wp.depth = wayPoint["depth"].d();
              wp.description = wayPoint["description"].s();
              wp.latitude = wayPoint["latitude"].d();
              wp.longitude = wayPoint["longitude"].d();
              wp.name = wayPoint["name"].s();
              wp.id = wayPoint["id"].i();
              
              wp = _ds->updateWayPoint(wp);


              json waypointJson;
              waypointJson["depth"] = wp.depth;
              waypointJson["description"] = wp.description;
              waypointJson["latitude"] = wp.latitude;
              waypointJson["longitude"] = wp.longitude;
              waypointJson["name"] = wp.name;
              waypointJson["id"] = wp.id;
              res.body = waypointJson.dump();
              res.code = 200;
              res.end();
            });
    CROW_ROUTE(app, "/deleteWaypoint")
            .methods("POST"_method)
            ([this](const crow::request& req, crow::response & res) {
              crow::json::rvalue wayPoint = crow::json::load(req.body);

              WayPoint wp;
              // Extract values from the JSON and set variables
              wp.depth = wayPoint["depth"].d();
              wp.description = wayPoint["description"].s();
              wp.latitude = wayPoint["latitude"].d();
              wp.longitude = wayPoint["longitude"].d();
              wp.name = wayPoint["name"].s();
              wp.id = wayPoint["id"].i();
              _ds->deleteWayPoint(wp);
              json waypointJson;
              res.body = waypointJson.dump();
              res.code = 200;
              res.end();
            });
    CROW_ROUTE(app, "/")([this]() {
      return "AutoPilot Server.  Nothing here routes return JSON for UI.";
    });

    app.port(18080).multithreaded().run_async();
  }

  WebServer::WebServer(const WebServer& orig) {
  }

  WebServer::~WebServer() {
  }
}
