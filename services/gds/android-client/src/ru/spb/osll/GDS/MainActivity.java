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

import java.util.Date;

import org.json.JSONObject;

import ru.spb.osll.GDS.exception.ExceptionHandler;
import ru.spb.osll.GDS.preferences.Settings;
import ru.spb.osll.GDS.preferences.SettingsActivity;
import ru.spb.osll.GDS.tracking.TrackingManager;
import ru.spb.osll.GDS.tracking.TrackingReceiver;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBaseResponse;
import ru.spb.osll.json.IRequest.IResponse;
import android.app.ProgressDialog;
import android.app.TabActivity;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.location.Location;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.ImageButton;
import android.widget.Button;
import android.widget.TabHost;
import android.widget.TabHost.TabSpec;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends TabActivity {
	
	public static final int SETTINGS_ID = Menu.FIRST;
	public static final int SIGNOUT_ID = Menu.FIRST + 1;
	
	public static final int INTERVAL = 7;
	
	private Settings m_settings;
	private String m_authToken = null;
	private String m_login = null;
	private String m_channel = null;
	private TabHost m_TabHost;
	private TrackingManager m_trackingManager;
	private Button m_trackingButton;
	private TextView m_logView;
	private TextView m_statusView;
	private ProgressDialog m_progress;
	
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
		registerReceiver(m_trackingReceiver, new IntentFilter(TrackingReceiver.ACTION_TRACKING));
		
		m_settings = new Settings(this);
		Bundle extras = getIntent().getExtras();
		if (extras != null) {
		    m_authToken = extras.getString(GDSUtil.AUTH_TOKEN);
		    m_login = extras.getString(GDSUtil.LOGIN);
		    m_channel = extras.getString(GDSUtil.CHANNEL); 
		}
		if (m_authToken == null || m_login == null || m_channel == null) {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "problem with extracting data");
			}
			Toast.makeText(this, "Can't sign in", Toast.LENGTH_LONG).show();
			finish();
			return;
		}
		
	    m_TabHost = getTabHost();
	    m_TabHost.addTab(m_TabHost.newTabSpec("tab1").setIndicator("SOS").setContent(R.id.sos_tab));
	    ((ImageButton) findViewById(R.id.sos_button)).setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				sos();
			}
		});
	    
	    TabSpec tabSpec = m_TabHost.newTabSpec("tab2");
	    tabSpec.setIndicator("Map");
	    Context ctx = this.getApplicationContext();
	    Intent i = new Intent(ctx, MapTabActivity.class);
		i.putExtra(GDSUtil.AUTH_TOKEN, m_authToken);	
	    tabSpec.setContent(i);
	    m_TabHost.addTab(tabSpec);		
	    m_TabHost.setCurrentTab(1);
	    m_TabHost.addTab(m_TabHost.newTabSpec("tab3").setIndicator("Tracking").setContent(R.id.tracking_tab));
	    m_trackingButton = (Button) findViewById(R.id.tracking_button);
	    m_trackingButton.setText("Start tracking");
	    m_trackingButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				MainActivity.this.changeTrackingMode();
			}
		});
		m_logView = (TextView) findViewById(R.id.tracking_log);
		m_statusView = (TextView) findViewById(R.id.tracking_status);
		m_statusView.setText("Tracking stoped");
		
		startService(new Intent(this, LocationService.class));	 
		LocationService.getLocation(MainActivity.this);
	    
		m_trackingManager = new TrackingManager(m_authToken, m_channel);
			
	}

	@Override
	protected void onDestroy() {
		stopService(new Intent(this, LocationService.class));
		m_trackingManager.stopTracking(this);
		unregisterReceiver(m_trackingReceiver);
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
	
	private void sos() {
		m_progress = ProgressDialog.show(this, "SOS", "Sending SOS", true, false);
		Thread thread = new Thread() {
			@Override
			public void run() {
				SystemClock.sleep(2 * 1000);
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "SOS thread started");
				}
				Location location = LocationService.getLocation(MainActivity.this);
				while (location == null) {
					if (GDSUtil.DEBUG) {
						Log.v(GDSUtil.LOG, "can't get location, trying again...");
					}
					SystemClock.sleep(2 * 1000);
					location = LocationService.getLocation(MainActivity.this);
				}
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "location determined! sending location...");
				}
				String serverUrl = m_settings.getServerUrl();
				JSONObject JSONResponse = null;
				String description = m_settings.getDescription();
				for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
					JSONResponse = new JsonApplyMarkRequest(m_authToken, "Events", "SOS", "",
							description, location.getLatitude(), location.getLongitude(), 0,
							GDSUtil.getUtcTime(new Date()), serverUrl).doRequest();
					if (JSONResponse != null) 
						break;
				}
				if (JSONResponse != null) {
					int errno = JsonBaseResponse.parseErrno(JSONResponse);
					if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
						if (GDSUtil.DEBUG) {
							Log.v(GDSUtil.LOG, "Mark sent successfully");
						}
						//broadcastMarkSent(location);
					} else {
						//handleError(errno);
						return;
					}
				} else {
					if (GDSUtil.DEBUG) {
						Log.v(TrackingManager.LOG, "response failed");
					}
					//broadcastError("Failed to send location");
					return;
				}
				m_handler.sendEmptyMessage(0);
			}
		};
		thread.start();
	}
	
	private Handler m_handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
        	m_progress.dismiss();
        	if (GDSUtil.DEBUG) {
        		Log.v(GDSUtil.LOG, "SOS have been sent!");
        	}
            Toast.makeText(MainActivity.this, "SOS have been sent!", Toast.LENGTH_LONG).show();
        }
	};
	
	private TrackingReceiver m_trackingReceiver = new TrackingReceiver() {
		@Override
		public void onMarkSent(String lonlat) {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					m_statusView.setText("Tracking started");
					m_trackingButton.setText("Stop tracking");
				}
			});
			appendToLogView("send mark: " + lonlat);
		}
		@Override
		public void onErrorOccured(String error) {
			runOnUiThread(new Runnable() {
				@Override
				public void run() {
					checkTrackingMode();
				}
			});
			appendToLogView(error);
		}
	};
	
	private void changeTrackingMode() {
		if (m_trackingManager.isTracking(this)) {
			m_trackingManager.stopTracking(this);
			m_statusView.setText("Tracking stoped");
			m_trackingButton.setText("Start tracking");
		} else {
			m_trackingManager.startTracking(this);
			m_statusView.setText("Starting tracking...");
			m_trackingButton.setText("Stop tracking");
		}
	}
	
	private void checkTrackingMode() {
		if (m_trackingManager.isTracking(this)) {
			m_statusView.setText("Tracking stoped");
			m_trackingButton.setText("Start tracking");
		} else {
			m_trackingManager.startTracking(this);
			m_trackingButton.setText("Stop tracking");
		}
	}
	
	private void showSettings() {
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "opening settings");
		}
		startActivity(new Intent(this, SettingsActivity.class));
	}
	
	private void signOut() {
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "signing out");
		}
		finish();
		
	}
	
	private static int lineCount = 0;
	private static final int maxLines = 20;
	public void appendToLogView(final String mess){
		if (lineCount > maxLines){
			clearLogView();
			lineCount = 0;
		}
		appendToLogViewInternal(mess);
		lineCount++;
	}
	private void appendToLogViewInternal(final String mess){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				m_logView.append("\n" + mess);
			}
		});
	}

	private void clearLogView(){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				m_logView.setText("");
			}
		});
	}

}
