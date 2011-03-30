package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonApplyChannelRequest implements IRequest.IApplyChannel {

	private String m_authToken;
	private String m_name;
	private String m_description;
	private String m_url;
	private Integer m_activeRadius;
	
	public JsonApplyChannelRequest(String authToken, String name, String description, String url, Integer activeRadius){
		m_authToken = authToken;
		m_name = name;
		m_description = description;
		m_url = url;
		m_activeRadius = activeRadius;
	}
	
	public JSONObject doRequest(){
		JSONObject resultJSON = null; 
		try {
			JSONObject jsonObject = new JSONObject();
			jsonObject.put(AUTH_TOKEN, m_authToken);
			jsonObject.put(NAME, m_name);
			jsonObject.put(DESCRIPTION, m_description);
			jsonObject.put(URL, m_url);
			jsonObject.put(ACTIVE_RADIUS, m_activeRadius);
			
			resultJSON = JsonBase.instance().doRequest(jsonObject, new URI(REQUEST));
		} catch (JSONException e) {
			e.printStackTrace();
			// TODO Auto-generated catch block
		} catch (IOException e) {
			e.printStackTrace();
			// TODO: handle exception
		} catch (URISyntaxException e) {
			e.printStackTrace();
			// TODO Auto-generated catch block
		}
		return resultJSON;
	}	
}
