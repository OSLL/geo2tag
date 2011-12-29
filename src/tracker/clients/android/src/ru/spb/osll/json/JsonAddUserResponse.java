package ru.spb.osll.json;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class JsonAddUserResponse extends JsonOldResponse {
	
	private String m_authToken;

	@Override
	public void parseJson(JSONObject obj) {
		super.parseJson(obj);
	  	try {
    		m_authToken = obj.getString("auth_token");
		} catch (JSONException e) {
			Log.e(LOG, e.getMessage());
		}
	}

	public String getAuthToken() {
		return m_authToken;
	}
	
}
