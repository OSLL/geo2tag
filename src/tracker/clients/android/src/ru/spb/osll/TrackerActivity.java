package ru.spb.osll;

import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.SettingsActivity;
import ru.spb.osll.services.LocationService;
import ru.spb.osll.services.RequestService;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class TrackerActivity extends Activity {
	public static String LOG = "Tracker";
	
	TextView m_logView;
	public static TrackerActivity Instance;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		Instance = this;
		m_logView = (TextView) findViewById(R.id.TextField);
		initialization();
		
		Settings settings = new Settings(this);
		if (settings.isSettingsEmpty()){
			settings.setDefaultSettrings();
		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	// ----------------------------------------------------------------
	private void initialization(){
		Log.v(LOG, "TrackerActivity - initialization");
		
		final Button btnService = (Button) findViewById(R.id.TestButton);
		if (RequestService.isActive()){
			btnService.setText("STOP TRACKER");
		} else {
			btnService.setText("START TRACKER");
		}
		
		btnService.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	startTracker();
            }
        });

		final Button settingsBtn= (Button) findViewById(R.id.settingsButton);
		settingsBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				startActivity(new Intent(TrackerActivity.this, SettingsActivity.class));
			}
		});
		
		final Button jsonBtn= (Button) findViewById(R.id.testJsonButton);
		jsonBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				//
			}
		});
		
		
	}
	
	private void startTracker(){
		try {
			if (RequestService.isActive()){
				stopService(new Intent(this, RequestService.class));
				stopService(new Intent(this, LocationService.class));
				TrackerUtil.disnotify(this);
			} else if (isOnline()){
				startService(new Intent(this, RequestService.class));
				startService(new Intent(this, LocationService.class));
				TrackerUtil.notify(this);
			}
		} catch (Exception e){
			m_logView.append("\n" + e.getMessage());
		}
	}
		
	// ------------------------------------------------------------------
	private boolean isOnline() {
	    ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
	    NetworkInfo nInfo = cm.getActiveNetworkInfo();
	    if (nInfo != null && nInfo.isConnected()) {
	    	showToast("ONLINE");
	    	appendToLogView("ONLINE");
	    	Log.v(LOG, "ONLINE");
	        return true;
	    }
	    else {
	    	showToast("OFFLINE");
	        Log.v(LOG, "OFFLINE");
	        return false;
	    }
	}

	public void showToast(final String mess){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(TrackerActivity.this, mess, Toast.LENGTH_SHORT).show();				
			}
		});
	}

	public void showToast(final int mess){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(TrackerActivity.this, mess, Toast.LENGTH_SHORT).show();				
			}
		});
	}

	public void appendToLogView(final String mess){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				m_logView.append("\n" + mess);
			}
		});
	}

	public void clearLogView(){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				m_logView.setText("");
			}
		});
	}

	
}
