package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import org.gwtopenmaps.openlayers.client.LonLat;
import org.gwtopenmaps.openlayers.client.Map;
import org.gwtopenmaps.openlayers.client.MapOptions;
import org.gwtopenmaps.openlayers.client.MapWidget;
import org.gwtopenmaps.openlayers.client.Marker;
import org.gwtopenmaps.openlayers.client.control.LayerSwitcher;
import org.gwtopenmaps.openlayers.client.control.MousePosition;
import org.gwtopenmaps.openlayers.client.event.MarkerBrowserEventListener;
import org.gwtopenmaps.openlayers.client.layer.Markers;
import org.gwtopenmaps.openlayers.client.layer.OSM;
import org.gwtopenmaps.openlayers.client.popup.FramedCloud;
import org.gwtopenmaps.openlayers.client.popup.Popup;

import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.tools.HTMLUtil;

import com.google.gwt.user.client.ui.Widget;

public class OSMWidget extends BaseMapWidget {

	private MapWidget m_mapWidget;
	private Markers m_markers;
	private Popup m_curPopup;
	private List<Popup> m_labels;
	
	@Override
	protected Widget onInitializeMap() {
		m_labels= new ArrayList<Popup>();
		
		MapOptions defaultMapOptions = new MapOptions();
		m_mapWidget = new MapWidget("100%", "650px", defaultMapOptions);

		OSM osm_1 = OSM.Osmarender("Osmarender"); 
		osm_1.setIsBaseLayer(true);

		OSM osm_2 = OSM.Mapnik("Mapnik"); 
		osm_2.setIsBaseLayer(true);

		OSM osm_3 = OSM.CycleMap("CycleMap");
		osm_3.setIsBaseLayer(true);

//		OSM osm_4 = OSM.Maplint("Maplint");
//		osm_4.setIsBaseLayer(true);

		Map map = m_mapWidget.getMap();
		map.addLayer(osm_1);
		map.addLayer(osm_2);
		map.addLayer(osm_3);
//		map.addLayer(osm_4);

		map.addControl(new LayerSwitcher());
		map.addControl(new MousePosition());
		
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
		final Map map = m_mapWidget.getMap(); 
		map.setCenter(lonLat, 12);
		
		for (Tag tag : tags) {
			final LonLat longLat = new LonLat(tag.getLongitude(), tag.getLatitude());
			longLat.transform("EPSG:4326", "EPSG:900913");
	
			final String html  = HTMLUtil.toHTML(tag);
			final Marker marker = new Marker(longLat);
			marker.addBrowserEventListener("mousedown", new MarkerBrowserEventListener() {
				@Override
				public void onBrowserEvent(MarkerBrowserEvent markerBrowserEvent) {
					if (m_curPopup != null){
						map.removePopup(m_curPopup);
					}
					m_curPopup = getPopupFrame(longLat, html);
					m_mapWidget.getMap().addPopup(m_curPopup);
				}
			});
			
			final Popup l = getSub(longLat, tag.getLabel());
			m_labels.add(l);
			map.addPopup(l);
			m_markers.addMarker(marker);
		}
	}

	@Override
	public void clear(){
		removeMarkers();
	}
	
	private void removeMarkers(){
		m_mapWidget.getMap().removeLayer(m_markers);
		m_markers = new Markers("Markers");
		m_mapWidget.getMap().addLayer(m_markers);
		
		if (m_curPopup != null){
			m_mapWidget.getMap().removePopup(m_curPopup);
		}
		for(Popup popup : m_labels){
			m_mapWidget.getMap().removePopup(popup);
		}
	}
	
	private Popup getSub(LonLat lonLat, String title){
		String label = HTMLUtil.bold(HTMLUtil.text(title, 2));
		final Popup popup = new Popup("title", lonLat, null, label, false);
		popup.setAutoSize(true);
		popup.setOpacity(0.6);
		return popup;
	}
	
	private FramedCloud getPopupFrame(LonLat lonLat, String html){
		final FramedCloud frame = new FramedCloud("fullmess", lonLat, null, html, null, true);
		frame.setAutoSize(true);
		frame.setPanMapIfOutOfView(true);
		return frame; 
	}
}
