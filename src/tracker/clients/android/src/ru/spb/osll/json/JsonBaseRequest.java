package ru.spb.osll.json;

import java.io.IOException;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public abstract class JsonBaseRequest {
	public String JSON_LOG = JsonBase.LOG;

	private String m_serverUrl;

	public JsonBaseRequest(){
	}

	public JsonBaseRequest(String serverUrl){
		m_serverUrl = serverUrl;
	}
	
	public void setServerUrl(String url){
		m_serverUrl = url;
	}
	
	public String getServerUrl(){
		return m_serverUrl;
	}
	
	public JSONObject doRequest(){
		JSONObject resultJSON = null; 
		try {
			resultJSON = doRequestInternal(); 
		} catch (Exception e) {
			if (e != null && e.getMessage() != null){
				Log.v(JSON_LOG, e.getMessage());
			} else {
				Log.v(JSON_LOG, "unknown error...");
			}
		}
		return resultJSON;
	}	

	abstract protected JSONObject doRequestInternal() 
		throws JSONException, IOException, URISyntaxException;	
	
}
