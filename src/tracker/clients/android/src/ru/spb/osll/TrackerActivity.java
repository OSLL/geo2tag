package ru.spb.osll;

import ru.spb.osll.preferences.Settings;
import ru.spb.osll.preferences.SettingsActivity;
import ru.spb.osll.preferences.Settings.ITrackerAppSettings;
import ru.spb.osll.services.LocationService;
import ru.spb.osll.services.RequestService;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Activity;
import android.content.Intent;
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
		
		Button btnService = (Button) findViewById(R.id.start_button);
		btnService.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	startTracker();
            }
        });

		final Button stopBtn= (Button) findViewById(R.id.stop_button);
		stopBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				stopTracker();
			}
		});
		
		final Button settingsBtn= (Button) findViewById(R.id.settings_button);
		settingsBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				if (RequestService.isActive()){
					showToast(TrackerUtil.MESS_SETTINGS_NOT_AVAILABLE);
				} else {
					startActivity(new Intent(TrackerActivity.this, SettingsActivity.class));
				}
			}
		});

		final Button creenBtn = (Button) findViewById(R.id.screeen_down_button);
		creenBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
			}
		});

	}
	
	private void startTracker(){
		if (RequestService.isActive()){
			showToast(TrackerUtil.MESS_TRACKER_ALREADY_RUNNING);
		} else if (TrackerUtil.isOnline(this)){
			showToast(TrackerUtil.MESS_TRACKER_START);
			clearLogView();
			TrackerUtil.notify(this);
			startService(new Intent(this, RequestService.class));
			startService(new Intent(this, LocationService.class));
			
			if (Settings.getPreferences(this).getBoolean(ITrackerAppSettings.IS_HIDE_APP, true)){
				finish();
			}
		} else if (!TrackerUtil.isOnline(this)){
			showToast(TrackerUtil.MESS_FAIL_CONNECTION);
		}
	}
	
	private void stopTracker(){
		if (RequestService.isActive()){
			showToast(TrackerUtil.MESS_TRACKER_STOP);
			TrackerUtil.disnotify(this);
			stopService(new Intent(this, RequestService.class));
			stopService(new Intent(this, LocationService.class));
		}
	}

	
	// ------------------------------------------------------------------
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
