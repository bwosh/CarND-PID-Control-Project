#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "piddata.h"

// for convenience
using nlohmann::json;
using std::string;

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != string::npos) {
    return "";
  }
  else if (b1 != string::npos && b2 != string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main() {
  uWS::Hub h;

  int raceLoopIterations = 14000;
  int validateEvery = 100;
  static bool useSearch = false;
  
  // Kp:0.2 Kd:3.0, Ki:0.0001
  // +16/5000i/-v optimzation loops: Kp 0.232878, Kd:3.74842, Ki:0.000100122
  // +129/400i/5v [Kp 0.252443, Kd:4.1115, Ki:0.000115009]
  // +55/800i/5v [Kp 0.29369, Kd:4.15896, Ki:0.000115388]
  // +4/6000i/100v [Kp: 0.241336, Kd:3.94704, Ki:0.000115009] (MSE: 0.167375)
  // +5000GradientDescent/1i/5000: [Kp 0.23655, Kd:3.94704, Ki:0.000115009]
  // +20SGD/1000i: [Kp 0.184724, Kd:3.77704, Ki:0]
  PID pid(0.184724, 3.77704, 0);
  Search hyperParamsSearch(&pid, raceLoopIterations, validateEvery);
  PIDData piddata(&pid, &hyperParamsSearch);

  h.onMessage([&piddata](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                     uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2') {
      auto s = hasData(string(data).substr(0, length));

      if (s != "") {
        auto j = json::parse(s);

        string event = j[0].get<string>();

        if (event == "telemetry") {
          // j[1] is the data JSON object

          double cte = std::stod(j[1]["cte"].get<string>());
          double speed = std::stod(j[1]["speed"].get<string>());
          double angle = std::stod(j[1]["steering_angle"].get<string>());

          piddata.GetPID()->UpdateError(cte);
          double new_angle = piddata.GetPID()->TotalError();

          if(useSearch)
            piddata.GetSearch()->nextIter();

          double throttle = 0;
          if(speed<15)
            throttle = 0.2;
          if(!useSearch)
            throttle = 0.3;

          json msgJson;
          msgJson["steering_angle"] = new_angle;
          msgJson["throttle"] = throttle;
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }  // end "telemetry" if
      } else {
        // Manual driving
        string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }  // end websocket message if
  }); // end h.onMessage

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                         char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port)) {
    std::cout << "Listening to port " << port << std::endl;
  } else {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  
  h.run();
}