#include <Wt/WApplication>
#include "defines.h"
#include <syslog.h>
#include <fstream>
#include "Webside.h"

/*void readParams()
{
    std::ifstream in("/opt/geo2tag/geo2tag.conf");
    std::string serverUrl;
    int serverPort;
    in >> serverUrl >> serverPort;
    syslog(LOG_INFO,"Reading from /opt/geo2tag/geo2tag.conf : %s , %i",serverUrl.c_str(),serverPort);
    setServerUrl(QString(serverUrl.c_str()));
    setServerPort(serverPort);
//    syslog(LOG_INFO,"After options set : %s , %i",getServerUrl().toStdString().c_str(),getServerPort());
}*/

WApplication *createApplication(const WEnvironment& env)
{
  /*
   * You could read information from the environment to decide whether
   * the user has permission to start a new application
   */
  WApplication *app = new WApplication(env);
  openlog("geo2tag_webside", LOG_CONS | LOG_NDELAY, LOG_USER);
  std::string cssPath("css/wt");
  //app->useStyleSheet(cssPath + "/wt.css");
  //app->useStyleSheet(cssPath + "/wt_ie.css", "lt IE 7");
  ///app->useStyleSheet("css/home.css");
  //app->useStyleSheet("css/sourceview.css");
  app->useStyleSheet("wt/wt.css");
  app->useStyleSheet("style.css");
  app->setTitle("Geo2tag");
  Webside * webside;
  webside = new Webside(app->root());
  return app;
}


int main(int argc, char **argv)
{
  /*
   * Your main method may set up some shared resources, but should then
   * start the server application (FastCGI or httpd) that starts listening
   * for requests, and handles all of the application life cycles.
   *
   * The last argument to WRun specifies the function that will instantiate
   * new application objects. That function is executed when a new user surfs
   * to the Wt application, and after the library has negotiated browser
   * support. The function should return a newly instantiated application
   * object.
   */
  return WRun(argc, argv, &createApplication);
}
