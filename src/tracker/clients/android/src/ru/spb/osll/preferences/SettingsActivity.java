package ru.spb.osll.preferences;

import ru.spb.osll.R;
import ru.spb.osll.preferences.Settings.ITrackerSettings;
import android.app.Activity;
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
		initField(LOGIN, R.id.edit_login);
		initField(PASSWORD, R.id.edit_password);
		initField(CHANNEL, R.id.edit_channel);
		initField(CHANNEL_KEY, R.id.edit_key);
		initField(SERVER_URL, R.id.edit_server_address);
	}
	
	private void savePreferences(){
		Editor prefEditor = new Settings(this).getPreferencesEditor();
		saveField(LOGIN, R.id.edit_login, prefEditor);
		saveField(PASSWORD, R.id.edit_password, prefEditor);
		saveField(CHANNEL, R.id.edit_channel, prefEditor);
		saveField(CHANNEL_KEY, R.id.edit_key, prefEditor);
		saveField(SERVER_URL, R.id.edit_server_address, prefEditor);
		prefEditor.commit();
	}
	
	private Runnable m_saveToast = new Runnable() {
		@Override
		public void run() {
			Toast.makeText(SettingsActivity.this, "Settings saved!", Toast.LENGTH_SHORT).show();
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

}
