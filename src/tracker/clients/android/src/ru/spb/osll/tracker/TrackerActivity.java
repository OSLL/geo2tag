/*
 * Copyright 2010-2012  Vasily Romanikhin  bac1ca89@gmail.com
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

package ru.spb.osll.tracker;

import ru.spb.osll.tracker.R; 
import ru.spb.osll.tracker.exception.ExceptionHandler;
import ru.spb.osll.tracker.preferences.Settings;
import ru.spb.osll.tracker.preferences.SettingsActivity;
import ru.spb.osll.tracker.preferences.Settings.ITrackerAppSettings;
import ru.spb.osll.tracker.services.RequestService;
import ru.spb.osll.tracker.utils.TrackerUtil;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
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

	private TextView mLogView;
    private BroadcastReceiver mTrackerReceiver = new TrackerReceiver();
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
		registerReceiver(mTrackerReceiver, new IntentFilter(TrackerReceiver.ACTION_MESS));
		
		mLogView = (TextView) findViewById(R.id.TextField);
		initialization();
		
		Settings settings = new Settings(this);
		if (settings.isSettingsEmpty()){
			settings.setDefaultSettrings();
		}
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		unregisterReceiver(mTrackerReceiver);
	}

	private void initialization(){
		Log.v(LOG, "TrackerActivity - initialization");
		
		Button btnService = (Button) findViewById(R.id.start_button);
		btnService.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            	startTracker();
            }
        });

		final Button stopBtn = (Button) findViewById(R.id.stop_button);
		stopBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				stopTracker();
			}
		});
		
		final Button settingsBtn = (Button) findViewById(R.id.settings_button);
		settingsBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
			    if (TrackerUtil.isServiceRunning(TrackerActivity.this, RequestService.class)) {
					showToast(R.string.msg_settigns_not_available);
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
		if (TrackerUtil.isServiceRunning(this, RequestService.class)){
			showToast(R.string.msg_tracker_already_running);
		} else if (TrackerUtil.isOnline(this)){
			showToast(R.string.msg_tracker_start);
			clearLogView();
			TrackerUtil.notify(this);
			startService(new Intent(this, RequestService.class));
			
			if (Settings.getPreferences(this).getBoolean(ITrackerAppSettings.IS_HIDE_APP, true)){
				TrackerUtil.hideApplication(TrackerActivity.this);
			}
		} else if (!TrackerUtil.isOnline(this)){
			showToast(R.string.msg_fail_connection);
		}
	}
	
	private void stopTracker(){
		if (TrackerUtil.isServiceRunning(this, RequestService.class)){
			showToast(R.string.msg_tracker_stop);
			TrackerUtil.disnotify(this);
			stopService(new Intent(this, RequestService.class));
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
	
	private void showToast(final int resId){
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(TrackerActivity.this, resId, Toast.LENGTH_SHORT).show();              
            }
        });
    }


	private static int lineCount = 0;
	private static final int maxLines = 16;
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
				mLogView.append("\n" + mess);
			}
		});
	}

	private void clearLogView(){
		runOnUiThread(new Runnable() {
			@Override
			public void run() {
				mLogView.setText("");
			}
		});
	}	
	
	public class TrackerReceiver extends BroadcastReceiver {
		public static final String 	ACTION_MESS   = "action.mess";
		public static final String TYPE_MESS      = "type.mess";
		public static final String TYPE_OPEATION  = "type.operation";
		public static final int ID_SHOW_TOAST     = 0;
		public static final int ID_APPEND_TO_LOG  = 1;
        public static final int ID_LOG_AND_TOAST  = 2;
		
		@Override
		public void onReceive(Context context, Intent intent) {
			final String mess = intent.getStringExtra(TYPE_MESS);
			final int type = intent.getIntExtra(TYPE_OPEATION, -1);
            switch (type) {
                case ID_SHOW_TOAST:
                    showToast(mess);
                    break;
                case ID_APPEND_TO_LOG:
                    appendToLogView(mess);
                    break;
                case ID_LOG_AND_TOAST:
                    showToast(mess);
                    appendToLogView(mess);
                    break;
            }
		}
	}
}
