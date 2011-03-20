#include <Wt/WApplication>
#include <syslog.h>
#include "Webside.h"

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
    Webside *webside;
    webside = new Webside(app->root());
//    Webside webside(app->root());
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
