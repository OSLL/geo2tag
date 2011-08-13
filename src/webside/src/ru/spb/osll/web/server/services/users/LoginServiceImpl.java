package ru.spb.osll.web.server.services.users;

import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.server.db.Users;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class LoginServiceImpl extends RemoteServiceServlet implements LoginService {

	@Override
	public User login(User user) throws IllegalArgumentException {
		User standart = Users.Instance().select(user.getLogin());
		if (standart != null){
			if (standart.getPassword().equals(user.getPassword())){
				standart.setStatus(Response.STATUS_SUCCES);
				standart.setMessage("welcome sds dsdsddsds sd sdsd!!!");
				user = standart;
			} else {
				user.setStatus(Response.STATUS_FAIL);
				user.setMessage("Password wrong");
			}
		} else {
			user.setStatus(Response.STATUS_FAIL);
			user.setMessage("Login wrong");
		}
		return user;
	}

	@Override
	public User addUser(User user) throws IllegalArgumentException {
		user = Users.Instance().insert(user);
		return user;
	}
}
