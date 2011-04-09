package ru.spb.osll.preferences;

import ru.spb.osll.R;
import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

public class SettingsActivity extends Activity {

	
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
		// TODO
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
	
}
