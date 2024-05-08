#include "cmdline.hpp"
#include "network/webserver.hpp"

int main(int argc, const char* argv[]) {
    ZDC::CmdLine cmdLine(argc, argv);

    ZDC::NET::WebServer oWebServer;
    if(!oWebServer.startup(8080, true)) {
        fprintf(stderr, "Failed to start webserver on port: %d, errno: %d\n", 8080, errno);
        return errno;
    }
    
    while(true);
    return 0;
}