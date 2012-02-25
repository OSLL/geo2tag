/*
 * Copyright 2012  Ivan Bezyazychnyy  ivan.bezyazychnyy@gmail.com
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

package ru.spb.osll.GDS;

import org.json.JSONObject;

import ru.spb.osll.GDS.R;
import ru.spb.osll.GDS.exception.ExceptionHandler;
import ru.spb.osll.GDS.preferences.Settings;
import ru.spb.osll.GDS.preferences.Settings.IGDSSettings;
import ru.spb.osll.GDS.preferences.SettingsActivity;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonBaseResponse;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.IRequest.IResponse;
import android.app.Activity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Toast;

public class LoginActivity extends Activity {
	
	public static final int SETTINGS_ID = Menu.FIRST;
	
	public static final String AUTH_TOKEN = "auth_token";
	public static final String LOGIN = "login";
	public static final String CHANNEL = "channel";
	
	private EditText m_loginEdit;
	private EditText m_passwordEdit;
	private CheckBox m_rememberCheck;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.login_view);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
		
		Settings settings = new Settings(this);
		if (settings.isSettingsEmpty()){
			settings.setDefaultSettings();
		}
		
		m_loginEdit = (EditText) findViewById(R.id.edit_login);
		m_passwordEdit = (EditText) findViewById(R.id.edit_password);
		m_rememberCheck = (CheckBox) findViewById(R.id.remember_me_checkbox);
		
		initViews();
		initButtons();
		
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		super.onPause();
	}

	@Override
	protected void onResume() {
		super.onResume();
	}
	
	@Override
    public boolean onCreateOptionsMenu(Menu menu) {
        boolean result = super.onCreateOptionsMenu(menu);
        menu.add(0, SETTINGS_ID, 0, R.string.menu_settings);
        return result;
    }
	
	@Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case SETTINGS_ID:
            showSettings();
            return true;
        }
       
        return super.onOptionsItemSelected(item);
    }
	
	private void initViews() {
		final SharedPreferences settings = new Settings(this).getPreferences();
		m_loginEdit.setText(settings.getString(IGDSSettings.LOGIN, "?????"));
		m_passwordEdit.setText(settings.getString(IGDSSettings.PASSWORD, "?????"));
		m_rememberCheck.setChecked(settings.getBoolean(IGDSSettings.REMEMBER, false));
	}
	
	private void initButtons() {
		final Button signInBtn= (Button) findViewById(R.id.sign_in_button);
		signInBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				LoginActivity.this.signIn();
			}
		});
		
		final Button createAccountBtn= (Button) findViewById(R.id.create_account_button);
		createAccountBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				LoginActivity.this.createAccount();
			}
		});
	}
	
	private void signIn() {
		Log.v(IGDSSettings.LOG, "signing in");
		
		String login = m_loginEdit.getText().toString();
		String password = m_passwordEdit.getText().toString();
		String channel = login;
		String serverUrl = new Settings(this).getPreferences().getString(
				IGDSSettings.SERVER_URL, "");
		String authToken = "";
		
		JSONObject JSONResponse = null;
		for(int i = 0; i < IGDSSettings.ATTEMPTS; i++){
			JSONResponse = new JsonLoginRequest(login, password, serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				Log.v(IGDSSettings.LOG, "user logged in successfully");
			} else {
				handleError(errno);
				return;
			}
			
			authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);

		} else {
			Log.v(IGDSSettings.LOG, "response failed");
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}
		
		// If remember checkbox is checked then save login and password
		// else save them as empty
		// TODO
		
		Intent i = new Intent(this, MainActivity.class);
		i.putExtra(AUTH_TOKEN, authToken);
		i.putExtra(LOGIN, login);
		i.putExtra(CHANNEL, channel);
		startActivity(i);		
	}
	
	private void handleError(int errno) {
		if (errno < 0) {
			Log.v(IGDSSettings.LOG, "bad response received");
			Toast.makeText(this, "Server error (corrupted response)",
					Toast.LENGTH_LONG).show();
		} else if (errno >= IResponse.geo2tagError.values().length) {
			Log.v(IGDSSettings.LOG, "unknown error");
			Toast.makeText(this, "Unknown server error",
					Toast.LENGTH_LONG).show();
		} else if (errno > 0) {
			String error = IResponse.geo2tagError.values()[errno].name();
			Log.v(IGDSSettings.LOG, "error: " + error);
			Toast.makeText(this, "Error: " + error,
					Toast.LENGTH_LONG).show();
		}
	}
	
	private void createAccount() {
		Log.v(IGDSSettings.LOG, "creating account");
		startActivity(new Intent(this, CreateAccountActivity.class));
	}
	
	private void showSettings() {
		Log.v(IGDSSettings.LOG, "opening settings");
		startActivity(new Intent(this, SettingsActivity.class));
	}

}
