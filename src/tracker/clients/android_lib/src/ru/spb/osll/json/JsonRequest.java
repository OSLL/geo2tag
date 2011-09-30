package ru.spb.osll.json;

import java.io.IOException;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public abstract class JsonRequest {
	public static String JSON_LOG = "JsonLog";
	
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
