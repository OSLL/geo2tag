package ru.spb.osll.web.client.ui.widgets;

import java.util.List;

import org.gwtopenmaps.openlayers.client.*;
import org.gwtopenmaps.openlayers.client.control.LayerSwitcher;
import org.gwtopenmaps.openlayers.client.control.MousePosition;
import org.gwtopenmaps.openlayers.client.layer.Markers;
import org.gwtopenmaps.openlayers.client.layer.OSM;

import ru.spb.osll.web.client.services.objects.Tag;

import com.google.gwt.user.client.ui.Widget;

public class OSMWidget extends BaseMapWidget {

	private MapWidget m_mapWidget;
	private Markers m_markers;
	
	@Override
	protected Widget onInitializeMap() {
		MapOptions defaultMapOptions = new MapOptions();
		m_mapWidget = new MapWidget("100%", "550px", defaultMapOptions);

		OSM osm_1 = OSM.Osmarender("Osmarender"); // Label for menu 'LayerSwitcher'
		osm_1.setIsBaseLayer(true);

		OSM osm_2 = OSM.Mapnik("Mapnik"); // Label for menu 'LayerSwitcher'
		osm_2.setIsBaseLayer(true);

		OSM osm_3 = OSM.CycleMap("CycleMap");
		osm_3.setIsBaseLayer(true);

		// OSM osm_4 = OSM.Maplint("Maplint");
		// osm_4.setIsBaseLayer(true);

		Map map = m_mapWidget.getMap();
		map.addLayer(osm_1);
		map.addLayer(osm_2);
		map.addLayer(osm_3);
		// map.addLayer(osm_4);
		map.addControl(new LayerSwitcher());
		map.addControl(new MousePosition());

		// map.setCenter(new LonLat(6.95, 50.94), 12);
		m_markers = new Markers("Markers");
		map.addLayer(m_markers);

		return m_mapWidget;
	}
	
	@Override
	public void setTags(List<Tag> tags){
		removeMarkers();
		if (tags == null || tags.size() == 0){
			return;
		}
		
		final Tag initTag = tags.get(0);
		LonLat lonLat = new LonLat(initTag.getLongitude(), initTag.getLatitude());
		lonLat.transform("EPSG:4326", "EPSG:900913");
		m_mapWidget.getMap().setCenter(lonLat, 12);
		
		Size size = new Size(21, 25);
		Pixel offset = new Pixel((int) -(size.getWidth() / 2), (int) -size.getHeight());

		for (Tag tag : tags) {
			LonLat longLat = new LonLat(tag.getLongitude(), tag.getLatitude());
			longLat.transform("EPSG:4326", "EPSG:900913");
			Icon icon = new Icon("http://www.openlayers.org/dev/img/marker.png", size, offset);
			m_markers.addMarker(new Marker(longLat, icon));
		}
	}

	@Override
	public void refresh(){
		removeMarkers();
	}
	
	private void removeMarkers(){
		m_mapWidget.getMap().removeLayer(m_markers);
		m_markers = new Markers("Markers");
		m_mapWidget.getMap().addLayer(m_markers);
	}
}
