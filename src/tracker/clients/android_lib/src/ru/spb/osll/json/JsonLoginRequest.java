package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.objects.User;

import android.util.Log;

public class JsonLoginRequest extends JsonRequest implements IRequest.ILogin {
	private String m_login;
	private String m_password;
	private String m_serverUrl;

	public JsonLoginRequest(User user, String serverUrl){
		this(user.getLogin(), user.getPassword(), serverUrl);
	}
	
	public JsonLoginRequest(String login, String password, String serverUrl){
		m_login = login;
		m_password = password;
		m_serverUrl = serverUrl;
	}

	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(LOGIN, m_login);
		jsonObject.put(PASSWORD, m_password);
		Log.v(JSON_LOG, jsonObject.toString());
		return JsonBase.instance().doRequest(jsonObject, new URI(m_serverUrl + REQUEST));
	}

}

