package ru.spb.osll.GDS;

import android.os.Bundle;
import android.view.View;

import com.google.android.maps.MapActivity;
import com.google.android.maps.MapView;

public class MapTabActivity extends MapActivity {
	@Override
	protected void onCreate(Bundle icicle) {
		super.onCreate(icicle);
		setContentView(R.layout.map_tab_view);
		MapView mapView = (MapView) findViewById(R.id.mapview);
	    mapView.setVisibility(View.GONE);
	}
	@Override
	protected boolean isRouteDisplayed() {
		return false;
	}
}