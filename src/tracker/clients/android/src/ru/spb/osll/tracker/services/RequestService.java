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

package ru.spb.osll.tracker.services;

import java.util.Date;

import org.json.JSONObject;

import ru.spb.osll.json.*;
import ru.spb.osll.tracker.R;
import ru.spb.osll.tracker.TrackerActivity;
import ru.spb.osll.tracker.TrackerActivity.TrackerReceiver;
import ru.spb.osll.tracker.preferences.Settings;
import ru.spb.osll.tracker.preferences.Settings.ITrackerAppSettings;
import ru.spb.osll.tracker.preferences.Settings.ITrackerNetSettings;
import ru.spb.osll.tracker.utils.AndroidJGeoLogger;
import ru.spb.osll.tracker.utils.TrackerUtil;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.location.Location;
import android.os.IBinder;
import android.util.Log;

public class RequestService extends LocationService {
    
    static {
        ru.spb.osll.log.Log.setLogger(new AndroidJGeoLogger());
    }
        
    private String mAuthToken;
    private SettingsCache mSCache;
    private boolean mTrackerStatus = false;

	@Override
	public void onStart(Intent intent, int startId) {
	    Log.v(TrackerActivity.LOG, "RequestService.onStart()");
		super.onStart(intent, startId);
		mSCache = new SettingsCache(this);

        mTrackerStatus = true;
		new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    mAuthToken = login();
                    applyChannel();
                    subscribeToChannel();
                    track();
                } catch (TrackerException e) {
                    TrackerUtil.disnotify(RequestService.this);
                    sendToastAndLog(e.getMessage());
                    stopSelf();
                }
            }
        }).start();
	}

	@Override
	public void onDestroy() {
	    Log.v(TrackerActivity.LOG, "RequestService.onDestroy()");
        mTrackerStatus = false;
	    super.onDestroy();
	}

	@Override
	public IBinder onBind(Intent intent) {
		return null;
	}
	
    /**
     * login method
     * @return authToken string if login successful, null - otherwise
     */
	private String login() throws TrackerException {
	    final JsonBaseRequest req = new JsonLoginRequest(mSCache.login, mSCache.pass, mSCache.serverUrl);
        final JsonLoginResponse res = new JsonLoginResponse();
	    safeSendingRequest(req, res, R.string.msg_srv_login_failed, Errno.SUCCESS);
	    sendToLog(R.string.msg_srv_login_successful);
	    return res.getAuthString();
	}

	/**
	 * @param authToken
	 * @return <code>TRUE</code> if new channel successefully added or channel already exists
	 */
    private void applyChannel() throws TrackerException {
        final JsonBaseRequest req = new JsonApplyChannelRequest(mAuthToken, mSCache.channel, 
                "G2T tracker channel", "http://osll.spb.ru/", 3000, mSCache.serverUrl);
        final JsonApplyChannelResponse res = new JsonApplyChannelResponse();
        safeSendingRequest(req, res, R.string.msg_srv_channel_preparation_failed, 
                Errno.SUCCESS, Errno.CHANNEL_ALREADY_EXIST_ERROR);
        sendToLog(R.string.msg_srv_channel_prepared);
    }
    
    private void subscribeToChannel() throws TrackerException {
        final JsonBaseRequest req = new JsonSubscribeRequest(mAuthToken, 
                mSCache.channel, mSCache.serverUrl);
        final JsonSubscribeResponse res = new JsonSubscribeResponse();
        safeSendingRequest(req, res, R.string.msg_srv_channel_preparation_failed, 
                Errno.SUCCESS, Errno.CHANNEL_ALREADY_SUBSCRIBED_ERROR);
        sendToLog(R.string.msg_srv_channel_successfully_checked);
    }

    public void track() throws TrackerException {
        int failCount = 0;
        while (mTrackerStatus){
            try {
                boolean success = applyMark(mAuthToken); 
                failCount = success ? 0 : failCount + 1;
                if (failCount > 20) {
                    throw new TrackerException("Connection failed");
                }
                Thread.sleep(mSCache.trackInterval*1000);
            } catch (InterruptedException e) {
                mTrackerStatus = false;
            }
        }
    }

	private boolean applyMark(String authToken){
		if (!isDeviceReady()) {
			return false;
		}

		final Location location = getLocation();
		final double lat = location.getLatitude();
		final double lon = location.getLongitude();
		final double alt = 0.0;
		
		JSONObject JSONResponse = new JsonApplyMarkRequest(
				authToken, mSCache.channel, "title", "unknown",
				"this tag was generated automaticaly by tracker application",
				lat, lon, alt, TrackerUtil.getTime(new Date()), mSCache.serverUrl).doRequest();
		
		if (JSONResponse != null){
            Log.d(TrackerActivity.LOG, JSONResponse.toString());
		    JsonApplyMarkResponse response = new JsonApplyMarkResponse();
		    response.parseJson(JSONResponse);
		    sendToLog(TrackerUtil.convertLocation(lat, lon));
		    return response.getErrno() == Errno.SUCCESS;
		} 
		return false;
	}

    private void sendToLog(int messId) {
        sendToLog(getResources().getString(messId));
    }

    private void sendToLog(String mess) {
        sendMess(TrackerReceiver.ID_APPEND_TO_LOG, mess);
    }

    private void sendToastAndLog(String mess) {
        sendMess(TrackerReceiver.ID_LOG_AND_TOAST, mess);
    }

    private void sendMess(int operationId, String mess) {
        Intent intent = new Intent(TrackerReceiver.ACTION_MESS);
        intent.putExtra(TrackerReceiver.TYPE_OPEATION, operationId);
        intent.putExtra(TrackerReceiver.TYPE_MESS, mess);
        sendBroadcast(intent);
    }

    /*
     * Safe sending requests
     * If after 3 attempts response is't received - throws TrackerException
     */
    private final int ATTEMPTS = 3;
    private void safeSendingRequest(JsonBaseRequest request, 
        JsonBaseResponse response, int errorMsgId, int... possibleErrnos) 
        throws TrackerException {
        final String errorMsg = getResources().getString(errorMsgId);
        
        JSONObject JSONResponse = null;
        for (int i = 0; i < ATTEMPTS && JSONResponse == null; i++) {
            JSONResponse = request.doRequest();
        }
        if (JSONResponse == null){
            throw new TrackerException(errorMsg);
        }
        Log.d(TrackerActivity.LOG, JSONResponse.toString());
        response.parseJson(JSONResponse);
        int errno =  response.getErrno();
        Log.d(TrackerActivity.LOG, "Errno: " + Errno.getErrorByCode(errno));
        for (int err : possibleErrnos) {
            if (err == errno) return;
        }
        
        throw new TrackerException(errorMsg);
    }
    
    @SuppressWarnings("serial")
    private static class TrackerException extends Exception {
        public TrackerException(String error) {
            super(error);
        }
    }
	
	class SettingsCache {
        public String login;
        public String pass;
        public String channel;
        public String serverUrl;    
        public int trackInterval;
        public boolean isShowTick;
        
        public SettingsCache(Context c){
            SharedPreferences settings = new Settings(c).getPreferences();
            login = settings.getString(ITrackerNetSettings.LOGIN, "");
            pass = settings.getString(ITrackerNetSettings.PASSWORD, "");
            channel = settings.getString(ITrackerNetSettings.CHANNEL, "");
            serverUrl = settings.getString(ITrackerNetSettings.SERVER_URL, "");
            trackInterval = settings.getInt(ITrackerNetSettings.TIME_TICK, 30);
            isShowTick = settings.getBoolean(ITrackerAppSettings.IS_SHOW_TICKS, false);
        }       
        
        @Override
        public String toString() {
            return "SettingsCache [login=" + login + ", pass=" + pass
                    + ", channel=" + channel + ", serverUrl=" + serverUrl
                    + ", trackInterval=" + trackInterval + ", isShowTick="
                    + isShowTick + "]";
        }
    }
}
