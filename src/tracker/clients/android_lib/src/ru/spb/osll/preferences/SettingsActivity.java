package ru.spb.osll.preferences;

import ru.spb.osll.R;
import ru.spb.osll.TrackerActivity;
import ru.spb.osll.airomo.Ala;
import ru.spb.osll.gui.RadioButtonDialog;
import ru.spb.osll.preferences.Settings.ITrackerAppSettings;
import ru.spb.osll.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

public class SettingsActivity extends Activity implements ITrackerNetSettings, ITrackerAppSettings {
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings);
	
		setTitle("Tracker Settings");
		initializeFields();
		initializeButtons();
		
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
	
	private int m_timeTick;
	private int m_history;
	private void initializeFields(){
		final Ala ALA = TrackerActivity.alaInstance();
		if(ALA == null)
			return;
		
		initField(R.id.edit_login, ALA.getUser());
		initField(R.id.edit_password, ALA.getPass());
		initField(R.id.edit_channel, ALA.getChannel());
		initField(R.id.edit_key, "");	// TODO
		initField(R.id.edit_server_address, ALA.getServerUrl());
		
		initCheckBox(IS_HIDE_APP, R.id.checkbox_hide);
		initCheckBox(IS_SHOW_TICKS, R.id.checkbox_tick);
	}
	
	private void initializeButtons(){
		final Ala ALA = TrackerActivity.alaInstance();
		if(ALA == null)
			return;
		
		m_timeTick = ALA.getTrackInterval();
		Button btnTick = (Button) findViewById(R.id.button_settings_tick);
		btnTick.setText(Integer.toString(m_timeTick));
		btnTick.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				new TimeTickDialog(SettingsActivity.this, getIdx(m_timeTick, argsTick)).show();
			}
		});

		m_history = ALA.getHistoryLimit();
		Button btnHist = (Button) findViewById(R.id.button_settings_history);
		btnHist.setText(Integer.toString(m_history));
		btnHist.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				new HistoryDialog(SettingsActivity.this, getIdx(m_history, argsHist)).show();
			}
		});
	}
	
	private void savePreferences(){
		final Ala ALA = TrackerActivity.alaInstance();
		if(ALA == null) return;
		
		ALA.setUser(fieldVal(R.id.edit_login));
		ALA.setPass(fieldVal(R.id.edit_password));
		ALA.setChannel(fieldVal(R.id.edit_channel));
		//ALA.setChannelKey();
		ALA.setServerUrl(fieldVal(R.id.edit_server_address));
		ALA.setTrackInterval(m_timeTick);
		ALA.setHistoryLimit(m_history);
		
		Editor prefEditor = new Settings(this).getPreferencesEditor();
		saveCheckBox(IS_HIDE_APP, R.id.checkbox_hide, prefEditor);
		saveCheckBox(IS_SHOW_TICKS, R.id.checkbox_tick, prefEditor);
		prefEditor.commit();
	}
	
	private Runnable m_saveToast = new Runnable() {
		@Override
		public void run() {
			Toast.makeText(SettingsActivity.this, TrackerUtil.MESS_SETTINGS_SAVED, Toast.LENGTH_SHORT).show();
		}
	};

	private void initField(int idField, String val){
		((EditText) findViewById(idField)).setText(val);
	}

	private void initCheckBox(String key, int id){
		SharedPreferences settings = new Settings(this).getPreferences();
		((CheckBox) findViewById(id)).setChecked(settings.getBoolean(key, false));
	}

	private String fieldVal(int idField){
		return ((EditText) findViewById(idField)).getText().toString().trim();
	}

	private void saveCheckBox(String key, int id, Editor prefEditor){
		boolean status = ((CheckBox) findViewById(id)).isChecked();
		prefEditor.putBoolean(key, status);
	}

	String[] argsTick = {"1", "2", "3", "4", "5", "10", "20", "30", "40", "50", "60"};
	String[] argsHist = {"10", "20", "30", "40", "50", "60", "70", "80", "90", "100"};
	private int getIdx(int val, String[] args){
		for (int i = 0; i < args.length; i++){
			if (args[i].equals(Integer.toString(val)))
				return i;
		}
		return 0;
	}

	class TimeTickDialog extends RadioButtonDialog{
		public TimeTickDialog(Context context, int selectedItem) {
			super(context, "Tick interval", argsTick, selectedItem);
		}
		@Override
		protected void itemSelected(DialogInterface dialog, int item) {
			super.itemSelected(dialog, item);
			m_timeTick = Integer.parseInt(argsTick[item]);
			((Button) findViewById(R.id.button_settings_tick)).setText(Integer.toString(m_timeTick));
		}	
	}
	
	class HistoryDialog extends RadioButtonDialog{
		public HistoryDialog(Context context, int selectedItem) {
			super(context, "History size", argsHist, selectedItem);
		}
		@Override
		protected void itemSelected(DialogInterface dialog, int item) {
			super.itemSelected(dialog, item);
			m_history = Integer.parseInt(argsHist[item]);
			((Button) findViewById(R.id.button_settings_history)).setText(Integer.toString(m_history));
		}	
	}
}
