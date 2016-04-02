#include "server_http.hpp"
#include "framework.hpp"

//Added for the default_resource example
#include <fstream>
// #include <boost/filesystem.hpp>

#include <json.hpp>

using namespace std;

//Added for the json-example:

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

std::string launchDriver(std::string master_name,
                         std::string name,
                         std::string resources,
                         std::string docker_image,
                         std::string command) {

  UPID master("master@" + master_name);
  if (!master) {
    std::cerr << "Could not parse:" << master_name << std::endl;
    return "Exited with failure";
  }

  Result<string> user = os::user();
  if (!user.isSome()) {
    if (user.isError()) {
      cerr << "Failed to get username: " << user.error() << endl;
    } else {
      cerr << "No username for uid " << ::getuid() << endl;
    }
    return "Exited with failure";
  }

  MesosLambdaScheduler scheduler(
      name,
      command,
      resources,
      Option<string>(docker_image),
      "docker");

  FrameworkInfo framework;
  framework.set_user(user.get());
  framework.set_name("mesos_lambda");
  framework.set_checkpoint(false);

  MesosSchedulerDriver driver(&scheduler, framework, master_name);

  if(driver.run() == DRIVER_STOPPED) {
    return scheduler.get_response_payload();
  } else {
    return "ERROR: Something went wrong within the docker container.";
  }
}

int main() {
    //HTTP-server at port 8080 using 4 threads
    HttpServer server(8080, 10);

    //Add resources using path-regex and method-string, and an anonymous function
    //POST-example for the path /string, responds the posted string
    server.resource["^/lambda$"]["POST"] = [](HttpServer::Response& response, shared_ptr<HttpServer::Request> request) {
      std::string request_body = request->content.string();
      auto json = nlohmann::json::parse(request_body);

      std::cout << "Processing..." << std::endl;

      // Constants
      std::string master = "10.0.0.76:5050";

      // Request specific
      std::string name = json["name"];
      std::string resources = json["resources"];
      std::string docker_image = json["docker_image"];
      std::string command = json["command"];

      std::cout << "Kicking off container and waiting..." << std::endl;
      std::string output = launchDriver(master, name, resources, docker_image, command);

      std::cout << "Received response..." << std::endl;
      std::cout << output << std::endl;

      response << "HTTP/1.1 200 OK\r\n";
      response << "Content-Length: " << output.length() << "\r\n";
      response << "\r\n" << output;

      std::cout << "End request" << std::endl;
    };

    std::thread server_thread([&server](){
        std::cout << "Server started on port 8080" << std::endl;

        //Start server
        server.start();
    });

    //Wait for server to start so that the client can connect
    std::this_thread::sleep_for(std::chrono::seconds(1));

    server_thread.join();

    return 0;
}
