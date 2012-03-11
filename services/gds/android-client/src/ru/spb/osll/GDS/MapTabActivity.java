package ru.spb.osll.GDS;

import ru.spb.osll.GDS.events.EventsManager;
import ru.spb.osll.GDS.events.EventsReceiver;
import ru.spb.osll.GDS.preferences.Settings.IGDSSettings;
import ru.spb.osll.objects.Mark;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import com.google.android.maps.MapActivity;
import com.google.android.maps.MapView;

public class MapTabActivity extends MapActivity {
	
	EventsManager m_eventsManager;
	String m_authToken;
	MapView m_mapView;
	
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.map_tab_view);
		
		Bundle extras = getIntent().getExtras();
		if (extras != null) {
		    m_authToken = extras.getString(LoginActivity.AUTH_TOKEN);
		}
		if (m_authToken == null) {
			Log.v(IGDSSettings.LOG, "problem with extracting data in MapTabActivity");
			Toast.makeText(this, "Can't create events tab", Toast.LENGTH_LONG).show();
			finish();
			return;
		}
		
		registerReceiver(m_eventsReceiver, new IntentFilter(EventsReceiver.ACTION_EVENTS));
		
		m_mapView = (MapView) findViewById(R.id.mapview);
		m_mapView.setVisibility(View.GONE);
		m_mapView.setBuiltInZoomControls(true);
	    
	    m_eventsManager = new EventsManager();
	    m_eventsManager.setData(m_authToken);
	    m_eventsManager.startEventsService(this);
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
	
	private EventsReceiver m_eventsReceiver = new EventsReceiver() {
		@Override
		public void onEvents(final Mark[] marks) {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					for (Mark mark : marks) {
						Log.v(IGDSSettings.LOG, mark.toString());
					}
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