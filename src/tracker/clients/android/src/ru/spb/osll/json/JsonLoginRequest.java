package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

public class JsonLoginRequest extends JsonRequest implements IRequest.ILogin {
	private String m_login;
	private String m_password;
	private String m_addUser;
	
	public JsonLoginRequest(String login, String password, boolean addUser){
		m_login = login;
		m_password = password;
		m_addUser = addUser;
	}

	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(LOGIN, m_login);
		jsonObject.put(PASSWORD, m_password);
		return JsonBase.instance().doRequest(jsonObject, new URI(REQUEST));
	}

}

