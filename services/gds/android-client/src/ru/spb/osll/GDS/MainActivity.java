/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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

package ru.spb.osll.GDS;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.json.JSONObject;

import ru.spb.osll.GDS.exception.ExceptionHandler;
import ru.spb.osll.GDS.preferences.Settings;
import ru.spb.osll.GDS.preferences.SettingsActivity;
import ru.spb.osll.GDS.preferences.Settings.IGDSSettings;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBaseResponse;
import ru.spb.osll.json.IRequest.IResponse;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.widget.Toast;

public class MainActivity extends Activity {
	
	public static final int SETTINGS_ID = Menu.FIRST;
	public static final int SIGNOUT_ID = Menu.FIRST + 1;
	
	public static final int INTERVAL = 7;
	
	private String m_authToken = null;
	private String m_login = null;
	private String m_channel = null;
	private LocationManager m_locationManager;
	private boolean m_isDeviceReady = false;
	private Thread m_trackThread;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
		
		Bundle extras = getIntent().getExtras();
		if (extras != null) {
		    m_authToken = extras.getString(LoginActivity.AUTH_TOKEN);
		    m_login = extras.getString(LoginActivity.LOGIN);
		    m_channel = extras.getString(LoginActivity.CHANNEL); 
		}
		if (m_authToken == null || m_login == null || m_channel == null) {
			Log.v(IGDSSettings.LOG, "problem with extracting data");
			Toast.makeText(this, "Can't sign in", Toast.LENGTH_LONG).show();
			finish();
			return;
		}
		
		m_locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		m_locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
		getLocation();
	}

	@Override
	protected void onDestroy() {
		stopTracking();
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
	}
	
	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean result = super.onCreateOptionsMenu(menu);
        menu.add(0, SETTINGS_ID, 0, R.string.menu_settings);
        menu.add(0, SIGNOUT_ID, 0, R.string.menu_sign_out);
        return result;
    }
	
	@Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case SETTINGS_ID:
            showSettings();
            return true;
        case SIGNOUT_ID:
        	signOut();
        }
       
        return super.onOptionsItemSelected(item);
    }
	
	private void showSettings() {
		Log.v(IGDSSettings.LOG, "opening settings");
		startActivity(new Intent(this, SettingsActivity.class));
	}
	
	private void signOut() {
		Log.v(IGDSSettings.LOG, "signing out");
		finish();
		
	}
	
	public Location getLocation() {
		Location location = null;
		String provider = LocationManager.NETWORK_PROVIDER;
		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

		if (locationManager != null) {
			if (locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
				provider = LocationManager.GPS_PROVIDER;
			}
			location = locationManager.getLastKnownLocation(provider);
		}
		return location;
	}
	
	protected void onLocationDeviceStatusChanged(boolean isReady) {
		Log.v(IGDSSettings.LOG, "onLocationDeviceStatusChanged: " + isReady);
		if (isReady) {
			startTracking();
		} else {
			stopTracking();
		}
	}
	
	private LocationListener locationListener = new LocationListener() {
		public void onLocationChanged(Location location) {
			if (!m_isDeviceReady && location != null){
				m_isDeviceReady = true;
				onLocationDeviceStatusChanged(true);
			} else if (m_isDeviceReady && location == null) {
				m_isDeviceReady = false;
				onLocationDeviceStatusChanged(false);
			}
		}
		public void onStatusChanged(String provider, int status, Bundle extras) {}
		public void onProviderEnabled(String provider) {}
		public void onProviderDisabled(String provider) {}
	};
	
	protected void stopTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
	}
	
	protected void startTracking(){
		if (m_trackThread != null){
			m_trackThread.interrupt();
		}
		m_trackThread = new Thread(new Runnable() {
			@Override
			public void run() {
				while (!Thread.currentThread().isInterrupted()){
					Location location = getLocation();
					Log.v(IGDSSettings.LOG, "coords: " + location.getLatitude()
							+ ", " + location.getLongitude());
					
					sendMark(location);
					
					SystemClock.sleep(INTERVAL * 1000);
				}
			}
		});
		m_trackThread.start();
	}
	
	private void sendMark(Location location) {
		String serverUrl = new Settings(this).getPreferences().getString(
				IGDSSettings.SERVER_URL, "");
		JSONObject JSONResponse = null;
		for(int i = 0; i < IGDSSettings.ATTEMPTS; i++){
			JSONResponse = new JsonApplyMarkRequest(m_authToken, m_channel, "gds tracker", "",
					"gds tracker", location.getLatitude(), location.getLongitude(), 0,
					getTime(new Date()), serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				Log.v(IGDSSettings.LOG, "Mark sent successfully");
			} else {
				handleError(errno);
				return;
			}
		} else {
			Log.v(IGDSSettings.LOG, "response failed");
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}
	}
	
	private void handleError(int errno) {
		if (errno < 0) {
			Log.v(IGDSSettings.LOG, "bad response received");
			Toast.makeText(this, "Server error (corrupted response)",
					Toast.LENGTH_LONG).show();
		} else if (errno >= IResponse.geo2tagError.values().length) {
			Log.v(IGDSSettings.LOG, "unknown error");
			Toast.makeText(this, "Unknown server error",
					Toast.LENGTH_LONG).show();
		} else if (errno > 0) {
			String error = IResponse.geo2tagError.values()[errno].name();
			Log.v(IGDSSettings.LOG, "error: " + error);
			Toast.makeText(this, "Error: " + error,
					Toast.LENGTH_LONG).show();
		}
	}
	
	private static DateFormat dateFormat = new SimpleDateFormat("dd MM yyyy HH:MM:ss.SSS");
	public static String getTime(Date date){
		return dateFormat.format(date);
	}

}
