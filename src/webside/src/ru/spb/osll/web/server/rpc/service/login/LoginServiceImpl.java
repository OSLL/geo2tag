package ru.spb.osll.web.server.rpc.service.login;

import ru.spb.osll.web.client.rpc.services.login.LoginService;
import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class LoginServiceImpl extends RemoteServiceServlet implements LoginService {

	@Override
	public User login(User user) throws IllegalArgumentException {
		user.setId(1);
		return user;
	}
}
