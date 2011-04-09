package ru.spb.osll.preferences;

import ru.spb.osll.R;
import ru.spb.osll.preferences.Settings.ITrackerSettings;
import android.app.Activity;
import android.content.SharedPreferences;
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
		// TODO
	}
	
	private Runnable m_saveToast = new Runnable() {
		@Override
		public void run() {
			Toast.makeText(SettingsActivity.this, "sasasa", Toast.LENGTH_SHORT).show();
		}
	};

	private void initField(String key, int idField){
		SharedPreferences settings = new Settings(this).getPreferences();
		String str = settings.getString(key, "?????");
		((EditText) findViewById(idField)).setText(str);
	}
}
