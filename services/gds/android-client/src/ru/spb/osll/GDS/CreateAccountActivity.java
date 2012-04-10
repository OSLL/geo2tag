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
import ru.spb.osll.GDS.exception.ExceptionHandler;
import ru.spb.osll.GDS.preferences.Settings;
import ru.spb.osll.GDS.preferences.SettingsActivity;
import ru.spb.osll.json.JsonAddUserRequest;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonBaseResponse;
import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.JsonSubscribeRequest;
import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class CreateAccountActivity extends Activity {

	private static final int SETTINGS_ID = Menu.FIRST;
	
	private EditText m_loginEdit;
	private EditText m_passwordEdit;
	private EditText m_rePasswordEdit;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.create_account_view);
		Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
		
		setTitle("Create a new account");
		
		m_loginEdit = (EditText) findViewById(R.id.edit_reg_login);
		m_passwordEdit = (EditText) findViewById(R.id.edit_reg_password);
		m_rePasswordEdit = (EditText) findViewById(R.id.edit_reg_re_password);

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
	
	private void initButtons() {
		final Button registerBtn= (Button) findViewById(R.id.register_button);
		registerBtn.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				CreateAccountActivity.this.register();
			}
		});
	}
	
	private void showSettings() {
		startActivity(new Intent(this, SettingsActivity.class));
	}
	
	private void register() {
		boolean success = false;
		String login = m_loginEdit.getText().toString();
		String password = m_passwordEdit.getText().toString();
		String re_password = m_rePasswordEdit.getText().toString();
		String serverUrl = new Settings(this).getServerUrl();
		String authToken = "";
		
		// Check fields
		if (login.length() == 0) {
			Toast.makeText(this, "Login can't be empty", Toast.LENGTH_LONG).show();
			return;
		}
		if (password.length() == 0) {
			Toast.makeText(this, "Password can't be empty", Toast.LENGTH_LONG).show();
			return;
		}
		if (password.compareTo(re_password) != 0) {
			Toast.makeText(this, "Passwords should match", Toast.LENGTH_LONG).show();
			return;
		}
		
		// Add user
		JSONObject JSONResponse = null;
		for(int i = 0; i < 1; i++){
			JSONResponse = new JsonAddUserRequest(login, password, login, serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "user added successfully");
				}
			} else {
				handleError(errno);
				return;
			}

		} else {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "response failed");
			}
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}
		
		// Login
		JSONResponse = null;
		for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
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
		for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
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
		
		// Add channel for tracking
		JSONResponse = null;
		for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
			JSONResponse = new JsonApplyChannelRequest(authToken, login,
					login + "'s channel", "", 400, serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "Channel for tracking added successfully");
				}
			} else {
				handleError(errno);
				return;
			}
		} else {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "response failed");
			}
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}
		
		// Subscribe to tracking channel
		JSONResponse = null;
		for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
			JSONResponse = new JsonSubscribeRequest(authToken, login, serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "Subscribed to tracking channel");
				}
			} else {
				handleError(errno);
				return;
			}
		} else {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "response failed");
			}
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}
		
		// Subscribe to Events channel (for doctors)
		JSONResponse = null;
		for(int i = 0; i < GDSUtil.ATTEMPTS; i++){
			JSONResponse = new JsonSubscribeRequest(authToken, "Events", serverUrl).doRequest();
			if (JSONResponse != null) 
				break;
		}
		if (JSONResponse != null) {
			int errno = JsonBaseResponse.parseErrno(JSONResponse);
			if (errno == IResponse.geo2tagError.SUCCESS.ordinal()) {
				if (GDSUtil.DEBUG) {
					Log.v(GDSUtil.LOG, "Subscribed to Events channel");
				}
			} else {
				handleError(errno);
				return;
			}
			success = true;
		} else {
			if (GDSUtil.DEBUG) {
				Log.v(GDSUtil.LOG, "response failed");
			}
			Toast.makeText(this, "Connection error",
					Toast.LENGTH_LONG).show();
			return;
		}

		if (success) {
			Toast.makeText(this, "Account has been created!",
					Toast.LENGTH_LONG).show();
			finish();
		}		
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

}
