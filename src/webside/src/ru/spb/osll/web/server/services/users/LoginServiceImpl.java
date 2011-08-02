package ru.spb.osll.web.server.services.users;

import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.server.db.Users;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class LoginServiceImpl extends RemoteServiceServlet implements LoginService {

	@Override
	public User login(User user) throws IllegalArgumentException {
		user = Users.selectUser(user.getLogin());
		return user;
	}

	@Override
	public User addUser(User user) throws IllegalArgumentException {
		user = Users.insertUser(user);
		return null;
	}
}
