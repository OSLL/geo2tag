package ru.spb.osll.json;

import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.services.RequestService;

public class JsonAddUserRequest extends JsonRequest implements IRequest.IAddUser {

	private String m_login;
	private String m_password;
	
	public JsonAddUserRequest(String login, String password){
		m_login = login;
		m_password = password;
	}

	@Override
	protected JSONObject doRequestInternal() throws JSONException, IOException,
			URISyntaxException {
		JSONObject jsonObject = new JSONObject();
		jsonObject.put(LOGIN, m_login);
		jsonObject.put(PASSWORD, m_password);
		return JsonBase.instance().doRequest(jsonObject, new URI(RequestService.SERVER + REQUEST));
	}

}
