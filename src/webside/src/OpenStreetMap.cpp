// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 *
 * Contributed by: Richard Ulrich.
 */

#include "OpenStreetMap.h"
#include <Wt/WApplication>
#include <Wt/WContainerWidget>

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include <string>
#include <sstream>
#include <utility>
#include <iostream>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace {
  // if there is no google api key configured, use the one for
  // http://localhost:8080/
  static const std::string localhost_key
    = "ABQIAAAAWqrN5o4-ISwj0Up_depYvhTwM0brOpm-"
      "All5BF6PoaKBxRWWERS-S9gPtCri-B6BZeXV8KpT4F80DQ";
}

namespace Wt {

WOpenStreetMap::Coordinate::Coordinate()
  : lat_(0), lon_(0)
{ }

WOpenStreetMap::Coordinate::Coordinate(double lat, double lon)
{
  setLatitude(lat);
  setLongitude(lon);
}

#ifndef WT_TARGET_JAVA
WOpenStreetMap::Coordinate::Coordinate(const std::pair<double, double>& lat_long)
{
  setLatitude(lat_long.first);
  setLongitude(lat_long.second);
}
#endif 

void WOpenStreetMap::Coordinate::setLatitude(double latitude)
{
  if (latitude < -90.0 || latitude > 90.0)
    throw std::out_of_range("invalid latitude: "
			    + boost::lexical_cast<std::string>(latitude));

  lat_ = latitude;
}

void WOpenStreetMap::Coordinate::setLongitude(double longitude)
{
  if (longitude < -180.0 || longitude > 180.0)
    throw std::out_of_range("invalid longitude: "
			    + boost::lexical_cast<std::string>(longitude));

  lon_ = longitude;
}

double WOpenStreetMap::Coordinate::distanceTo(const Coordinate &rhs) const
{
  const double lat1 = lat_ * M_PI / 180.0;
  const double lat2 = rhs.latitude() * M_PI / 180.0;
  const double deltaLong = (rhs.longitude() - lon_) * M_PI / 180.0;
  const double angle = std::sin(lat1) * std::sin(lat2)
    + std::cos(lat1) * std::cos(lat2) * std::cos(deltaLong);
  const double earthRadius = 6371.0; // km
  const double dist = earthRadius * std::acos(angle);

  return dist;
}

#ifndef WT_TARGET_JAVA
std::pair<double, double> WOpenStreetMap::Coordinate::operator ()() const
{
  return std::make_pair(lat_, lon_);
}

std::istream& operator>> (std::istream& i, WOpenStreetMap::Coordinate& c)
{
  double lat, lon;
  i >> lat >> std::ws >> lon;
  c.setLatitude(lat);
  c.setLongitude(lon);

  return i;
}
#endif

// example javascript code from :
// http://wiki.openstreetmap.org/wiki/OpenLayers_Simple_Example

WOpenStreetMap::WOpenStreetMap(WContainerWidget *parent)
 : clicked_(this, "click"),
   doubleClicked_(this, "dblclick"),
   mouseMoved_(this, "mousemove")
{
  setImplementation(new WContainerWidget());

  WApplication *app = WApplication::instance();

  // init the OpenStreet javascript api
  const std::string gmuri = "http://www.openlayers.org/api/OpenLayers.js";
  app->require(gmuri, "openlayers");

  if (parent)
    parent->addWidget(this);
}

WOpenStreetMap::~WOpenStreetMap()
{ }

void WOpenStreetMap::render(WFlags<RenderFlag> flags)
{


  if (flags & RenderFull) {
    // initialize the map
    std::stringstream strm;
    strm <<
      "{ function init() { "   // private scope
      """var self = " << jsRef() << ";"
//      """map = new OpenLayers.Map(\"" << id() << "\");"
//      """var mapnik = new OpenLayers.Layer.OSM();"
//      """map.addLayer(mapnik);"
//      """map.setCenter(new OpenLayers.LonLat(13.41, 52.52)"
//      ""  ".transform("
//      ""    "new OpenLayers.Projection(\"EPSG:4326\"),"
//      ""    "new OpenLayers.Projection(\"EPSG:900913\")"
//      ""  "), 15"
//      """);"
//
//      """self.map = map;"
//
//    // additional things
//    for (unsigned int i = 0; i < additions_.size(); i++)
//      strm << additions_[i];
//
      "" "} " // function init
      "" "init();"
      " }"; // private scope
//
//    additions_.clear();
//
    WApplication::instance()->doJavaScript(strm.str());
  }

WCompositeWidget::render(flags);

}

void WOpenStreetMap::clearOverlays()
{
  doGmJavaScript(jsRef() + ".map.clearOverlays();", false);
}

void WOpenStreetMap::doGmJavaScript(const std::string& jscode, bool sepScope)
{
  std::string js = jscode;
  // to keep the variables inside a scope where they don't interfere
  if (sepScope)
    js = "{" + js + "}";

  if (isRendered())
    WApplication::instance()->doJavaScript(js);
  else
    additions_.push_back(js);
}

void WOpenStreetMap::addMarker(const Coordinate& pos, std::string title)
{
  std::stringstream strm;
  int found = title.find(std::string("\n"));
  while (found != std::string::npos)
  {
    title.erase(found, 2);
    found = title.find(std::string("\n"));
  }
  strm << "var marker = new google.maps.Marker(new google.maps.LatLng("
       //<< pos.latitude() << ", " << pos.longitude() << ")); marker.title= \""
       //<< title << "\";"
       << pos.latitude() << ", " << pos.longitude() << "), {title: \""
       << title << "\"});"
       << jsRef() << ".map.addOverlay(marker);";

  doGmJavaScript(strm.str(), false);
}

void WOpenStreetMap::addPolyline(const std::vector<Coordinate>& points,
			     const WColor& color, int width, double opacity)
{
  // opacity has to be between 0.0 and 1.0
  opacity = std::max(std::min(opacity, 1.0), 0.0);

  std::stringstream strm;
  strm << "var waypoints = [];";
  for (size_t i = 0; i < points.size(); ++i)
    strm << "waypoints[" << i << "] = new google.maps.LatLng("
	 << points[i].latitude() << ", " << points[i].longitude() << ");";

  strm << "var poly = new google.maps.Polyline(waypoints, \""
       << color.cssText() << "\", " << width << ", " << opacity << ");"
       << jsRef() << ".map.addOverlay(poly);";

  doGmJavaScript(strm.str(), true);
}

void WOpenStreetMap::openInfoWindow(const Coordinate& pos,
				const WString& myHtml)
{
  std::stringstream strm;
  strm << jsRef() << ".map.openInfoWindow(new google.maps.LatLng("
       << pos.latitude() << ", " << pos.longitude() << "), "
       << WWebWidget::jsStringLiteral(myHtml) << ");";

  doGmJavaScript(strm.str(), false);
}

void WOpenStreetMap::setCenter(const Coordinate& center)
{
  std::stringstream strm;
  strm << jsRef() << ".map.setCenter(new google.maps.LatLng("
       << center.latitude() << ", " << center.longitude() << "));";

  doGmJavaScript(strm.str(), false);
}

void WOpenStreetMap::setCenter(const Coordinate& center, int zoom)
{
  std::stringstream strm;
  strm << jsRef() << ".map.setCenter(new google.maps.LatLng("
       << center.latitude() << ", " << center.longitude() << "), "
       << zoom << ");";

  doGmJavaScript(strm.str(), false);
}

void WOpenStreetMap::panTo(const Coordinate& center)
{
  std::stringstream strm;
  strm << jsRef() << ".map.panTo(new google.maps.LatLng("
       << center.latitude() << ", " << center.longitude() << "));";

  doGmJavaScript(strm.str(), false);
}

void WOpenStreetMap::setZoom(int level)
{
  doGmJavaScript(jsRef() + ".map.setZoom("
		 + boost::lexical_cast<std::string>(level) + ");", false);
}

void WOpenStreetMap::zoomIn()
{
  doGmJavaScript(jsRef() + ".map.zoomIn();", false);
}

void WOpenStreetMap::zoomOut()
{
  doGmJavaScript(jsRef() + ".map.zoomOut();", false);
}

void WOpenStreetMap::savePosition()
{
  doGmJavaScript(jsRef() + ".map.savePosition();", false);
}

void WOpenStreetMap::returnToSavedPosition()
{
  doGmJavaScript(jsRef() + ".map.returnToSavedPosition();", false);
}

void WOpenStreetMap::checkResize()
{
  doGmJavaScript(jsRef() + ".map.checkResize();", false);
}

void WOpenStreetMap::enableDragging()
{
  doGmJavaScript(jsRef() + ".map.enableDragging();", false);
}

void WOpenStreetMap::disableDragging()
{
  doGmJavaScript(jsRef() + ".map.disableDragging();", false);
}

void WOpenStreetMap::enableDoubleClickZoom()
{
  doGmJavaScript(jsRef() + ".map.enableDoubleClickZoom();", false);
}

void WOpenStreetMap::disableDoubleClickZoom()
{
  doGmJavaScript(jsRef() + ".map.disableDoubleClickZoom();", false);
}

void WOpenStreetMap::enableOpenStreetBar()
{
  doGmJavaScript(jsRef() + ".map.enableGoogleBar();", false);
}

void WOpenStreetMap::disableOpenStreetBar()
{
  doGmJavaScript(jsRef() + ".map.disableGoogleBar();", false);
}

void WOpenStreetMap::enableScrollWheelZoom()
{
  doGmJavaScript(jsRef() + ".map.enableScrollWheelZoom();", false);
}

void WOpenStreetMap::disableScrollWheelZoom()
{
  doGmJavaScript(jsRef() + ".map.disableScrollWheelZoom();", false);
}

#ifndef WT_TARGET_JAVA
void WOpenStreetMap::zoomWindow(const std::pair<Coordinate, Coordinate>& bbox)
{
  zoomWindow(bbox.first, bbox.second);
}
#endif

void WOpenStreetMap::zoomWindow(const Coordinate& topLeft,
			    const Coordinate& rightBottom)
{
  Coordinate topLeftC = topLeft;
  Coordinate rightBottomC = rightBottom;

  const Coordinate center
    ((topLeftC.latitude() + rightBottomC.latitude()) / 2.0,
     (topLeftC.longitude() + rightBottomC.longitude()) / 2.0);

  topLeftC = 
    Coordinate(std::min(topLeftC.latitude(), rightBottomC.latitude()),
	       std::min(topLeftC.longitude(), rightBottomC.longitude()));
  rightBottomC = 
    Coordinate(std::max(topLeftC.latitude(), rightBottomC.latitude()),
	       std::max(topLeftC.longitude(), rightBottomC.longitude()));
  std::stringstream strm;
  strm << "var bbox = new google.maps.LatLngBounds(new google.maps.LatLng("
       << topLeftC.latitude()  << ", " << topLeftC.longitude() << "), "
       << "new google.maps.LatLng("
       << rightBottomC.latitude() << ", " << rightBottomC.longitude() << "));"
       << "var zooml = " << jsRef() << ".map.getBoundsZoomLevel(bbox);"
       << jsRef() << ".map.setCenter(new google.maps.LatLng("
       << center.latitude() << ", " << center.longitude() << "), zooml);";

  doGmJavaScript(strm.str(), true);
}

void WOpenStreetMap::setMapTypeControl(MapTypeControl type)
{
  std::string control;
  switch (type) {
  case DefaultControl:
    control = "google.maps.MapTypeControl";
    break;
  case MenuControl:
    control = "google.maps.MenuMapTypeControl";
    break;
  case HierarchicalControl:
    control = "google.maps.HierarchicalMapTypeControl";
    break;
  default:
    control = "";
  }

  std::stringstream strm;
  strm << jsRef() << ".map.removeControl(" << jsRef() << ".mtc);";
       
  if(control != "")
    strm << "var mtc = new " << control << "();"
	 << jsRef() << ".mtc = mtc;"
	 << jsRef() << ".map.addControl(mtc);";
  
  doGmJavaScript(strm.str(), false);
}

}
