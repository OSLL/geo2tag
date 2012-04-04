package ru.spb.osll.web.server.services.users;

import javax.servlet.http.HttpSession;

import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.server.db.Users;
import ru.spb.osll.web.server.Session;
import ru.spb.osll.web.server.json.JsonBase;
import ru.spb.osll.web.server.json.JsonLoginRequest;
import org.json.JSONObject;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class LoginServiceImpl extends RemoteServiceServlet
	implements LoginService, Session.HasSession {
	private String m_authToken;
	private String AUTH_TOKEN = "auth_token";
	@Override
	public User login(User user) throws IllegalArgumentException {
		JSONObject JSONResponse = null;
		JSONResponse = new JsonLoginRequest(user.getLogin(),
				user.getPassword(), JsonBase.getServerUrl()).doRequest();

		if (JSONResponse != null) {
			m_authToken = JsonBase.getString(JSONResponse, AUTH_TOKEN);
			user.setToken(m_authToken);
			user.setStatus(Response.STATUS_SUCCES);
			Session.Instance().addValue(this, USER_ID, user.getId());
		} else {
			user.setStatus(Response.STATUS_FAIL);
		}
		return user;
	}

	@Override
	public boolean logout() throws IllegalArgumentException {
		Session.Instance().removeValue(this, USER_ID);
		return true;
	}

	@Override
	public User isAuthorized() throws IllegalArgumentException {
		final Long id = (Long)Session.Instance().getValue(this, USER_ID);
		return id == null ? null : Users.Instance().select(id);
	}
	
	@Override
	public User addUser(User user) throws IllegalArgumentException {
		final Users users = Users.Instance();
		final User standart = users.select(user.getLogin());
		if (standart == null){
			user = users.insert(user);
			user.setStatus(Response.STATUS_SUCCES);
		} else {
			user.setStatus(Response.STATUS_FAIL);
		}
		return user;
	}

	@Override
	public HttpSession getSession(){
		return getThreadLocalRequest().getSession();
	}
	
	private static final String USER_ID = "user.id";
}
