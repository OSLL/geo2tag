package ru.spb.osll.preferences;

import ru.spb.osll.R;
import ru.spb.osll.gui.RadioButtonDialog;
import ru.spb.osll.preferences.Settings.ITrackerSettings;
import ru.spb.osll.utils.TrackerUtil;
import android.app.Activity;
import android.content.Context;
import android.content.DialogInterface;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class SettingsActivity extends Activity implements ITrackerSettings {
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings);
	
		initializeFields();
		initializeTimeTickBtn();
		
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
	private void initializeFields(){
		initField(LOGIN, R.id.edit_login);
		initField(PASSWORD, R.id.edit_password);
		initField(CHANNEL, R.id.edit_channel);
		initField(CHANNEL_KEY, R.id.edit_key);
		initField(SERVER_URL, R.id.edit_server_address);
	}
	
	private void initializeTimeTickBtn(){
		m_timeTick = new Settings(this).getPreferences().getInt(TIME_TICK, 0);

		Button btnTick = (Button) findViewById(R.id.button_settings_tick);
		btnTick.setText(Integer.toString(m_timeTick));
		btnTick.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				new TimeTickDialog(SettingsActivity.this, getIdx(m_timeTick)).show();
			}
		});

	}
	
	private void savePreferences(){
		Editor prefEditor = new Settings(this).getPreferencesEditor();
		saveField(LOGIN, R.id.edit_login, prefEditor);
		saveField(PASSWORD, R.id.edit_password, prefEditor);
		saveField(CHANNEL, R.id.edit_channel, prefEditor);
		saveField(CHANNEL_KEY, R.id.edit_key, prefEditor);
		saveField(SERVER_URL, R.id.edit_server_address, prefEditor);
		prefEditor.putInt(TIME_TICK, m_timeTick);
		
		prefEditor.commit();
	}
	
	private Runnable m_saveToast = new Runnable() {
		@Override
		public void run() {
			Toast.makeText(SettingsActivity.this, TrackerUtil.MESS_SETTINGS_SAVED, Toast.LENGTH_SHORT).show();
		}
	};

	private void initField(String key, int idField){
		SharedPreferences settings = new Settings(this).getPreferences();
		String str = settings.getString(key, "?????");
		((EditText) findViewById(idField)).setText(str);
	}

	private void saveField(String key, int idField, Editor prefEditor){
		String str = ((EditText) findViewById(idField)).getText().toString().trim();
		prefEditor.putString(key, str);
	}
	

	String[] args = {"1", "2", "3", "4", "5", "10", "20", "30", "40", "50", "60"};
	private int getIdx(int val){
		for (int i = 0; i < args.length; i++){
			if (args[i].equals(Integer.toString(val)))
				return i;
		}
		return 0;
	}
	
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
	}
}
