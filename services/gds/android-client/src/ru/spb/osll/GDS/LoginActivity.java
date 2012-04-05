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
import ru.spb.osll.GDS.preferences.SettingsActivity;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonBaseResponse;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.IRequest.IResponse;
import android.app.Activity;
import android.content.Intent;
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
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "LoginActivity onPause");
		}
	}

	@Override
	protected void onResume() {
		super.onResume();
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "LoginActivity onResume");
		}
		initViews();
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
		Settings settings = new Settings(this);
		if (settings.isRememberMe()) {
			m_loginEdit.setText(settings.getLogin());
			m_passwordEdit.setText(settings.getPassword());
			m_rememberCheck.setChecked(true);
		} else {
			m_loginEdit.setText("");
			m_passwordEdit.setText("");
			m_rememberCheck.setChecked(false);
		}
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
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "signing in");
		}
		
		String login = m_loginEdit.getText().toString();
		String password = m_passwordEdit.getText().toString();
		String channel = login;
		Settings settings = new Settings(this);
		String serverUrl = settings.getServerUrl();
		//String serverUrl = new Settings(this).getPreferences().getString(
		//		IGDSSettings.SERVER_URL, "");
		String authToken = "";
		
		JSONObject JSONResponse = null;
		for(int i = 0; i < 1; i++){
			JSONResponse = new JsonLoginRequest(login, password, serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "user logged in successfully");
				}
			} else {
				handleError(errno);
				return;
			}
			
			authToken = JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN);

		} else {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "response failed");
			}
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}
		
		// Add Events channel
		JSONResponse = null;
		for (int i = 0; i < GDSUtil.ATTEMPTS; i++){
			JSONResponse = new JsonApplyChannelRequest(authToken, "Events",
					"Channel with Events", "", 40000, serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "Channel Events added successfully");
				}
			} else if (errno == IResponse.geo2tagError.
					CHANNEL_ALREADY_EXIST_ERROR.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "Channel Events already exists");
				}
			}
			else {
				handleError(errno);
				return;
			}
		} else {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "response failed");
			}
		}
		
		settings.setLogin(m_loginEdit.getText().toString());
		settings.setPassword(m_passwordEdit.getText().toString());
		settings.setAuthToken(authToken);
		if (m_rememberCheck.isChecked()) {
			settings.setRememberMe(true);
		} else {
			settings.setRememberMe(false);
		}
		
		Intent i = new Intent(this, MainActivity.class);
		i.putExtra(GDSUtil.AUTH_TOKEN, authToken);
		i.putExtra(GDSUtil.LOGIN, login);
		i.putExtra(GDSUtil.CHANNEL, channel);
		startActivity(i);		
	}
	
	private void handleError(int errno) {
		if (errno < 0) {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "bad response received");
			}
			Toast.makeText(this, "Server error (corrupted response)",
					Toast.LENGTH_LONG).show();
		} else if (errno >= IResponse.geo2tagError.values().length) {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "unknown error");
			}
			Toast.makeText(this, "Unknown server error",
					Toast.LENGTH_LONG).show();
		} else if (errno > 0) {
			String error = IResponse.geo2tagError.values()[errno].name();
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "error: " + error);
			}
			Toast.makeText(this, "Error: " + error,
					Toast.LENGTH_LONG).show();
		}
	}
	
	private void createAccount() {
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "creating account");
		}
		startActivity(new Intent(this, CreateAccountActivity.class));
	}
	
	private void showSettings() {
		if (GDSUtil.DEBUG) {
			Log.v(GDSUtil.LOG, "opening settings");
		}
		startActivity(new Intent(this, SettingsActivity.class));
	}

}
