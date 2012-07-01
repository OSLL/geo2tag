/*
 * Copyright 2011-2012 OSLL
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import org.gwtopenmaps.openlayers.client.*;
import org.gwtopenmaps.openlayers.client.control.LayerSwitcher;
import org.gwtopenmaps.openlayers.client.control.MousePosition;
import org.gwtopenmaps.openlayers.client.event.MarkerBrowserEventListener;
import org.gwtopenmaps.openlayers.client.layer.Markers;
import org.gwtopenmaps.openlayers.client.layer.OSM;
import org.gwtopenmaps.openlayers.client.popup.FramedCloud;
import org.gwtopenmaps.openlayers.client.popup.Popup;

import ru.spb.osll.web.client.services.objects.WMark;
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

//		OSM osm1 = OSM.Osmarender("Osmarender"); 
//		osm1.setIsBaseLayer(true);

		OSM osm2 = OSM.Mapnik("Mapnik"); 
		osm2.setIsBaseLayer(true);

		OSM osm3 = OSM.CycleMap("CycleMap");
		osm3.setIsBaseLayer(true);

//		OSM osm_4 = OSM.Maplint("Maplint");
//		osm_4.setIsBaseLayer(true);

		final Map map = m_mapWidget.getMap();
//		map.addLayer(osm1);
		map.addLayer(osm2);
		map.addLayer(osm3);
//		map.addLayer(osm_4);

		map.addControl(new LayerSwitcher());
		map.addControl(new MousePosition());
		
		m_markers = new Markers("Markers");
		map.addLayer(m_markers);
		
//		map.addMapZoomListener(new MapZoomListener() {
//			@Override
//			public void onMapZoom(MapZoomEvent eventObject) {
//				m_zoom = map.getZoom();
//			}
//		});
//		map.addMapMoveListener(new MapMoveListener() {
//			@Override
//			public void onMapMove(MapMoveEvent eventObject) {
//				m_center = map.getCenter();
//			}
//		});
		
		setMapCenter(30.606215, 59.870569, 8);
		return m_mapWidget;
	}
	
	@Override
	public void setTags(List<WMark> tags){
		removeMarkers();
		if (tags == null || tags.size() == 0){
			return;
		}
		final Map map = m_mapWidget.getMap(); 
		final WMark initTag = tags.get(0);
		setMapCenter(initTag.getLongitude(), initTag.getLatitude(), 10);
		
		for (WMark tag : tags) {
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
	public void refresh(){
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
	
	private void setMapCenter(double lon, double lat, int zoom){
		LonLat center = new LonLat(lon, lat);
		center.transform("EPSG:4326", "EPSG:900913");
		m_mapWidget.getMap().setCenter(center, zoom);
	}
	
}
