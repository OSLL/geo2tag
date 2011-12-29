package ru.spb.osll.json;

import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.json.JsonBase;
import android.util.Log;

public class JsonBaseResponse {
	public final String LOG = JsonBase.LOG; 
	
	private int m_errno = -1;
	
	public void parseJson(JSONObject obj){
    	try {
    		m_errno = obj.getInt("errno");
		} catch (JSONException e) {
			Log.e(LOG, e.getMessage());
		}
	}
	
	public int getErrno(){
		return m_errno;
	}
}
