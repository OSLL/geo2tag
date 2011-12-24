package ru.spb.osll.json;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class JsonOldResponse extends JsonBaseResponse {

	// status
	public static String STATUS_OK 			= "Ok";
	public static String STATUS_ERR 		= "Error";
	
	// status messages
	public static String SM_USER_EXTSTS 	= "Username already exists!";
	public static String SM_CHANNEL_EXTSTS 	= "Channel exists!";
	public static String SM_CHANNEL_ADDED 	= "Channel added";
	
	
	private String m_status;
	private String m_statusMessage;
	
	@Override
	public void parseJson(JSONObject obj) {
    	try {
    		m_status = obj.getString("status");
    		m_statusMessage = obj.getString("status_description");
		} catch (JSONException e) {
			Log.e(LOG, e.getMessage());
		}
	}

	public String getStatus() {
		return m_status;
	}

	public String getStatusMessage() {
		return m_statusMessage;
	}
	
}
