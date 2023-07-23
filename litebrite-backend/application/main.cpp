
#include "runtime.hpp"

#include "infrastructure/webserver/webserver.hpp"

int main() {
    application::RemoveSuccessFile();
    auto server = infrastructure::WebServer::Create({});
    server->Start();
    application::WaitForShutdown();
    server->Stop();
    application::CreateSuccessFile();
    return 0;
}
