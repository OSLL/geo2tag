package ru.spb.osll;

import ru.spb.osll.airomo.Ala;
import ru.spb.osll.airomo.receiver.AlaReceiver;
import ru.spb.osll.exception.ExceptionHandler;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Activity;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class TrackerActivity extends Activity {
	public static String LOG = "Tracker";
	private TextView m_logView;
	private Ala ALA;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		ALA = new Ala();
		
		setContentView(R.layout.main);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
		registerReceiver(m_alaAlaReceiver, new IntentFilter(AlaReceiver.ACTION_ALA));
		
		m_logView = (TextView) findViewById(R.id.TextField);
		initialization();
	}

	@Override
	protected void onDestroy() {
		unregisterReceiver(m_alaAlaReceiver);
		super.onDestroy();
	}

	private AlaReceiver m_alaAlaReceiver = new AlaReceiver() {
		@Override
		public void onNewMark(String lonlat) {
			appendToLogView(lonlat);
		}
		@Override
		public void onErrorOccured(String error) {
			appendToLogView(error);
		}
	};
	
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
				if (ALA.isTracking(TrackerActivity.this)){
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
				TrackerUtil.hideApplication(TrackerActivity.this);
			}
		});
	}
	
	private void startTracker(){
		if (ALA.isTracking(TrackerActivity.this)){
			showToast(TrackerUtil.MESS_TRACKER_ALREADY_RUNNING);
		} else {
			showToast(TrackerUtil.MESS_TRACKER_START);
			clearLogView();
			ALA.startTrack(TrackerActivity.this);
		} 
	}
	
	private void stopTracker(){
		if (ALA.isTracking(this)){
			showToast(TrackerUtil.MESS_TRACKER_STOP);
			ALA.stopTrack(TrackerActivity.this);
		}
	}

	private void showToast(final String mess){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				Toast.makeText(TrackerActivity.this, mess, Toast.LENGTH_SHORT).show();				
			}
		});
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
