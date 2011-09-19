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
		} catch (JSONException e) {
			Log.v(JSON_LOG, e.getMessage());
		} catch (IOException e) {
			Log.v(JSON_LOG, e.getMessage());
		} catch (URISyntaxException e) {
			Log.v(JSON_LOG, e.getMessage());
		}	catch (Exception e) {
			if (e.getMessage() != null){
				Log.v(JSON_LOG, e.getMessage());
			}
		}
		return resultJSON;
	}	

	abstract protected JSONObject doRequestInternal() 
		throws JSONException, IOException, URISyntaxException;	
	
}
