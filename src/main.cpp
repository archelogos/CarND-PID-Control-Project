#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid_steering;
  // Initialize the pid variable. A proportional–integral–derivative controller (PID controller)
  double Kp_pid_steering = 0.1;  // proportional coefficient
  double Ki_pid_steering = 0.003;  // integral coefficient
  double Kd_pid_steering = 3.0;  // differential coefficient
  pid_steering.Init(Kp_pid_steering, Ki_pid_steering, Kd_pid_steering);

  // Throttle pid
  PID pid_throttle;
  double Kp_pid_throttle = 0.1;  // proportional coefficient
  double Ki_pid_throttle = 0.001;  // integral coefficient
  double Kd_pid_throttle = 3.0;  // differential coefficient 
  pid_throttle.Init(Kp_pid_throttle, Ki_pid_throttle, Kd_pid_throttle);

  h.onMessage([&pid_steering,&pid_throttle](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          double throttle;
          /*
          * Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          // Update PID Error
          pid_steering.UpdateError(cte);
          steer_value = pid_steering.TotalError();

          // setup desired speed
          double min_speed = 20;
          double max_speed = 80;
          double desired_speed = min_speed;

          double abs_cte = fabs(cte);
          if (abs_cte <= 0.01)
            desired_speed = min_speed + 45;
          else if (0.01 < abs_cte <= 0.1)
            desired_speed = min_speed + 40;
          else if (0.1 < abs_cte <= 0.2)
            desired_speed = min_speed + 35;
          else if (0.2 < abs_cte <= 0.3)
            desired_speed = min_speed + 25;
          else if (0.3 < abs_cte <= 0.4)
            desired_speed = min_speed + 20;
          else if (0.4 < abs_cte <= 0.5)
            desired_speed = min_speed + 15;
          else if (0.5 < abs_cte <= 0.8)
            desired_speed = min_speed;
          else if (0.8 < abs_cte <= 1.0)
            desired_speed = min_speed-10;
          else
            desired_speed = min_speed-15;

          pid_throttle.UpdateError(speed-desired_speed);
          throttle = pid_throttle.TotalError();

          // DEBUG
          std::cout << "CTE: " << cte << " Steering Value: " << steer_value << " Throttle: " << throttle << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          //msgJson["throttle"] = throttle; // update throttle
          msgJson["throttle"] = 0.3; // update throttle
          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
