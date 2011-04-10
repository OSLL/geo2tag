package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.services.RequestService;
import android.util.Log;

public class JsonApplyMarkRequest extends JsonRequest  implements IRequest.IApplyMark {
	private String m_authToken;
	private String m_channel;
	private String m_title;
	private String m_link;
	private String m_description;
	private double m_latitude;
	private double m_longitude;
	private String m_time;
	private String m_serverUrl;
	
	public JsonApplyMarkRequest(String authToken, String channel, String title, String link,
			String description, double latitude, double longitude, String time, String serverUrl){
		m_authToken = authToken;
		m_channel = channel;
		m_title = title;
		m_link = link;
		m_description = description;
		m_latitude = latitude;
		m_longitude = longitude;
		m_time = time;
		m_serverUrl = serverUrl;
	}
	
	
	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(AUTH_TOKEN, m_authToken);
		jsonObject.put(CHANNEL, m_channel);
		jsonObject.put(TITLE, m_title);
		jsonObject.put(LINK, m_link);
		jsonObject.put(DESCRIPTION, m_description);
		jsonObject.put(LATITUDE, m_latitude);
		jsonObject.put(LONGITUDE, m_longitude);
		jsonObject.put(TIME, m_time);
		Log.v(JSON_LOG, jsonObject.toString());
		
		return JsonBase.instance().doRequest(jsonObject, new URI(m_serverUrl + REQUEST)); // TODO
	}
}
