package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import static ru.spb.osll.json.IRequest.ILogin.*;

public class JsonLoginRequest extends JsonBaseRequest {
	private String m_login;
	private String m_password;
	
	public JsonLoginRequest(String login, String password, String serverUrl){
		super(serverUrl);
		m_login = login;
		m_password = password;
	}

	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(LOGIN, m_login);
		jsonObject.put(PASSWORD, m_password);
		Log.v(JSON_LOG, jsonObject.toString());
		return JsonBase.instance().doRequest(jsonObject, new URI(getServerUrl() + REQUEST));
	}

}

