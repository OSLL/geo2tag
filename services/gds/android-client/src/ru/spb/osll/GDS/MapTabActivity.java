package ru.spb.osll.GDS;

import java.util.List;

import ru.spb.osll.GDS.events.EventsManager;
import ru.spb.osll.GDS.events.EventsReceiver;
import ru.spb.osll.GDS.events.EventsService;
import ru.spb.osll.GDS.maps.EventsItemizedOverlay;
import ru.spb.osll.GDS.maps.PositionOverlay;
import ru.spb.osll.objects.Mark;
import android.content.Context;
import android.content.IntentFilter;
import android.graphics.drawable.Drawable;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.util.Log;
import android.widget.Toast;

import com.google.android.maps.ItemizedOverlay;
import com.google.android.maps.MapActivity;
import com.google.android.maps.MapView;
import com.google.android.maps.Overlay;
import com.google.android.maps.OverlayItem;

public class MapTabActivity extends MapActivity {

	EventsManager m_eventsManager;
	String m_authToken;
	MapView m_mapView;
	EventsItemizedOverlay m_eventsOverlay;
	PositionOverlay m_positionOverlay;
	
	private LocationManager m_locationManager;
	private LocationListener m_locationListener = new LocationListener() {
		public void onLocationChanged(Location location0) {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "MapTabActivity: locationChanged");
			}
			updatePosition();
		}
		public void onStatusChanged(String provider, int status, Bundle extras) {}
		public void onProviderEnabled(String provider) {}
		public void onProviderDisabled(String provider) {}
	};
	
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.map_tab_view);
		
		Bundle extras = getIntent().getExtras();
		if (extras != null) {
		    m_authToken = extras.getString(GDSUtil.AUTH_TOKEN);
		}
		if (m_authToken == null) {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "problem with extracting data in MapTabActivity");
			}
			Toast.makeText(this, "Can't create events tab", Toast.LENGTH_LONG).show();
			finish();
			return;
		}
		
		registerReceiver(m_eventsReceiver, new IntentFilter(EventsReceiver.ACTION_EVENTS));
		
		m_mapView = (MapView) findViewById(R.id.mapview);
		//m_mapView.setVisibility(View.GONE);
		m_mapView.setBuiltInZoomControls(true);
		
		List<Overlay> mapOverlays = m_mapView.getOverlays();
		Drawable eventDrawable = this.getResources().getDrawable(
				R.drawable.event64);
		m_eventsOverlay = new EventsItemizedOverlay(eventDrawable, this);
		mapOverlays.add(m_eventsOverlay);
		
		m_locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, m_locationListener);
		Drawable positionDrawable = this.getResources().getDrawable(
				R.drawable.position32);
		m_positionOverlay = new PositionOverlay(positionDrawable);
		mapOverlays.add(m_positionOverlay);
		updatePosition();
	    
	    m_eventsManager = new EventsManager();
	    m_eventsManager.setData(m_authToken);
	    m_eventsManager.startEventsService(this);
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		updatePosition();
		m_eventsManager.requestEvents(this);
	}
	
	@Override
	protected void onDestroy() {
		super.onDestroy();
		m_eventsManager.stopEventsService(this);
		unregisterReceiver(m_eventsReceiver);
	}
	
	@Override
	protected boolean isRouteDisplayed() {
		return false;
	}
	
	@Override
	protected boolean isLocationDisplayed() {
		return true;
	}
	
	public void updatePosition() {
		Location location = LocationService.getLocation(MapTabActivity.this);
		if (location == null) {
			if (GDSUtil.DEBUG) {
				Log.v(EventsManager.LOG,
						"can't get location to update position on map");
			}
		} else {
			m_positionOverlay.updatePosition(location);
			m_mapView.invalidate();
		}
	}
	
	private EventsReceiver m_eventsReceiver = new EventsReceiver() {
		@Override
		public void onEvents(final Mark[] marks) {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					for (Mark mark : marks) {
						if (GDSUtil.DEBUG) {
							Log.v(GDSUtil.LOG, mark.toString());
						}
					}
					m_eventsOverlay.setEvents(marks);
					m_mapView.invalidate();
				}
			});
		}
		@Override
		public void onErrorOccured(String error) {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
				}
			});
		}
	};
}