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

package ru.spb.osll.GDS.preferences;

import ru.spb.osll.GDS.R;
import ru.spb.osll.GDS.exception.ExceptionHandler;
import android.app.Activity;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;

public class SettingsActivity extends Activity {
	
	private Settings m_settings;
	private EditText m_descriptionEdit;
	private EditText m_serverUrlEdit;
	private EditText m_radiusEdit;
	private EditText m_trackingPeriodEdit;
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
	
		setTitle("GeoDoctorSearch settings");
		
		m_settings = new Settings(this);
		m_descriptionEdit = (EditText) findViewById(R.id.edit_description);
		m_serverUrlEdit = (EditText) findViewById(R.id.edit_server_address);
		m_radiusEdit = (EditText) findViewById(R.id.edit_radius);
		m_trackingPeriodEdit = (EditText) findViewById(R.id.edit_tracking_period);
		
		initializeFields();
		
		Button btnOk = (Button) findViewById(R.id.button_ok);
		btnOk.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				savePreferences();
				runOnUiThread(m_saveToast);
				finish();
			}
		});
		
		Button btnCancel = (Button) findViewById(R.id.button_cancel);
		btnCancel.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
			}
		});
	}

	private void initializeFields(){
		m_descriptionEdit.setText(m_settings.getDescription());
		m_serverUrlEdit.setText(m_settings.getServerUrl());
		m_radiusEdit.setText(String.valueOf(m_settings.getRadius()));
		m_trackingPeriodEdit.setText(
				String.valueOf(m_settings.getTrackingPeriod()));
		
	}
	
	private void savePreferences(){
		m_settings.setDescription(m_descriptionEdit.getText().toString().trim());
		m_settings.setServerUrl(m_serverUrlEdit.getText().toString().trim());
		m_settings.setRadius(Integer.parseInt(m_radiusEdit.getText().toString()));
		m_settings.setTrackingPeriod(
				Integer.parseInt(m_trackingPeriodEdit.getText().toString()));
	}
	
	private Runnable m_saveToast = new Runnable() {
		@Override
		public void run() {
			//Toast.makeText(SettingsActivity.this, TrackerUtil.MESS_SETTINGS_SAVED, Toast.LENGTH_SHORT).show();
		}
	};

	private void initCheckBox(String key, int id){
		SharedPreferences settings = new Settings(this).getPreferences();
		((CheckBox) findViewById(id)).setChecked(settings.getBoolean(key, false));
	}

	private void saveCheckBox(String key, int id, Editor prefEditor){
		boolean status = ((CheckBox) findViewById(id)).isChecked();
		prefEditor.putBoolean(key, status);
	}

	String[] args = {"1", "2", "3", "4", "5", "10", "20", "30", "40", "50", "60"};
	private int getIdx(int val){
		for (int i = 0; i < args.length; i++){
			if (args[i].equals(Integer.toString(val)))
				return i;
		}
		return 0;
	}
	
	//private int m_timeTick;
	
//	private void initializeTimeTickBtn(){
//	m_timeTick = new Settings(this).getPreferences().getInt(TIME_TICK, 0);
//
//	Button btnTick = (Button) findViewById(R.id.button_settings_tick);
//	btnTick.setText(Integer.toString(m_timeTick));
//	btnTick.setOnClickListener(new View.OnClickListener() {
//		@Override
//		public void onClick(View v) {
//			//new TimeTickDialog(SettingsActivity.this, getIdx(m_timeTick)).show();
//		}
//	});
//}
	
	/*
	class TimeTickDialog extends RadioButtonDialog{
		public TimeTickDialog(Context context, int selectedItem) {
			super(context, "Tick interval", args, selectedItem);
		}
		@Override
		protected void itemSelected(DialogInterface dialog, int item) {
			super.itemSelected(dialog, item);
			m_timeTick = Integer.parseInt(args[item]);
			((Button) findViewById(R.id.button_settings_tick)).setText(Integer.toString(m_timeTick));
		}	
	}*/
}
