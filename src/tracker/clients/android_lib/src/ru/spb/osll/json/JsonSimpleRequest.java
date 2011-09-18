package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class JsonSimpleRequest extends JsonRequest {
	private JSONObject m_json;
	private String m_request;
	
	public JsonSimpleRequest(JSONObject json, String request){
		m_json = json;
		m_request = request;	
	}
	
	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		Log.v(JSON_LOG, m_json.toString());
		return JsonBase.instance().doRequest(m_json, new URI(m_request));
	}

}
