/*
 * Copyright 2010-2011  Vasily Romanikhin  bac1ca89@gmail.com
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

package ru.spb.osll.GDS;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

public class LocationService extends Service {
	
	public static String LOG = "GDS-LocationService";
	
	private LocationManager myLocationManager;
	
	@Override
	public void onCreate() {
		super.onCreate();
		if (GDSUtil.DEBUG) {
			Log.v(LOG, "location service create");
		}
	
		myLocationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		myLocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
	}

	@Override
	public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
		if (GDSUtil.DEBUG) {
			Log.v(LOG, "location service start");
		}
	}

	@Override
	public void onDestroy() {
		super.onDestroy();
		if (GDSUtil.DEBUG) {
			Log.v(LOG, "location service destroy");
		}
		myLocationManager.removeUpdates(locationListener);
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}

	private LocationListener locationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			if (location != null){
				if (GDSUtil.DEBUG) {
					Log.v(LOG, "onLocationChanged  " + location.toString());
				}
			}
		}

		public void onStatusChanged(String provider, int status, Bundle extras) {
			if (GDSUtil.DEBUG) {
				Log.v(LOG, "onStatusChanged " + provider + " " + status + " " + extras);
			}
		}

		public void onProviderEnabled(String provider) {
			//Log.v(TrackerActivity.LOG, "onProviderEnabled " + provider);
		}

		public void onProviderDisabled(String provider) {
			//Log.v(TrackerActivity.LOG, "onProviderDisabled " + provider);
		}
	};

	public static Location getLocation(Context context) {
		Location location = null;
		String provider = LocationManager.NETWORK_PROVIDER;
		LocationManager locationManager = (LocationManager) context.getSystemService(Context.LOCATION_SERVICE);

		if (locationManager != null) {
			if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
				provider = LocationManager.GPS_PROVIDER;
			}
			location = locationManager.getLastKnownLocation(provider);
		}
		return location;
	}
	
}
