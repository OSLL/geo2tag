package ru.spb.osll.web.client.ui.widgets;

import java.util.List;

import org.gwtopenmaps.openlayers.client.Icon;
import org.gwtopenmaps.openlayers.client.LonLat;
import org.gwtopenmaps.openlayers.client.Map;
import org.gwtopenmaps.openlayers.client.MapOptions;
import org.gwtopenmaps.openlayers.client.MapWidget;
import org.gwtopenmaps.openlayers.client.Marker;
import org.gwtopenmaps.openlayers.client.Pixel;
import org.gwtopenmaps.openlayers.client.Size;
import org.gwtopenmaps.openlayers.client.control.LayerSwitcher;
import org.gwtopenmaps.openlayers.client.control.MousePosition;
import org.gwtopenmaps.openlayers.client.layer.Markers;
import org.gwtopenmaps.openlayers.client.layer.OSM;

import com.google.gwt.user.client.ui.Widget;

import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.ui.core.SimpleComposite;

public abstract class OSMWidget extends SimpleComposite {
	
	abstract protected List<Tag> getTags();
	
	@Override
    public Widget onInitialize() {

        MapOptions defaultMapOptions = new MapOptions();
        MapWidget mapWidget = new MapWidget("800px", "600px", defaultMapOptions);

        OSM osm_1 = OSM.Osmarender("Osmarender");   // Label for menu 'LayerSwitcher'
            osm_1.setIsBaseLayer(true);

        OSM osm_2 = OSM.Mapnik("Mapnik");   // Label for menu 'LayerSwitcher'
            osm_2.setIsBaseLayer(true);

        OSM osm_3 = OSM.CycleMap("CycleMap"); 
            osm_3.setIsBaseLayer(true);

        //OSM osm_4 = OSM.Maplint("Maplint"); 
         //   osm_4.setIsBaseLayer(true);

        Map map = mapWidget.getMap();
            map.addLayer(osm_1);
            map.addLayer(osm_2);
            map.addLayer(osm_3);
            //map.addLayer(osm_4);
            map.addControl(new LayerSwitcher());
            map.addControl(new MousePosition());

         // map.setCenter(new LonLat(6.95, 50.94), 12);            
                                                                   
            LonLat lonLat = new LonLat(6.95, 50.94); 
            lonLat.transform("EPSG:4326", "EPSG:900913");              
            map.setCenter(lonLat, 12);   
            
            Markers markers = new Markers( "Markers" );
            map.addLayer(markers);

            Size size = new Size(21,25);
            Pixel offset = new Pixel((int)-(size.getWidth()/2), (int) -size.getHeight());
            List<Tag> tags = getTags();
            for (Tag tag:tags) {
            	LonLat longLat = new LonLat(tag.getLongitude(), tag.getLatitude());
            	longLat.transform("EPSG:4326", "EPSG:900913");
            	Icon icon = new Icon("http://www.openlayers.org/dev/img/marker.png", size, offset);
            	markers.addMarker(new Marker(longLat, icon));
            } 
        return mapWidget; 
    } 

	@Override
	protected String getName() {
		return "Test OSM";
	}
}
