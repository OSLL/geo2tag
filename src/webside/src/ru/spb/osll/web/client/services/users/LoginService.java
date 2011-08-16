package ru.spb.osll.web.client.services.users;

import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("LoginService")
public interface LoginService extends RemoteService {

	User login(User user) throws IllegalArgumentException;

	boolean logout() throws IllegalArgumentException;

	User isAuthorized() throws IllegalArgumentException;
	
	User addUser(User user) throws IllegalArgumentException;
	
	public static class Util {
		private static LoginServiceAsync instance;
		public static LoginServiceAsync getInstance(){
			if (instance == null) {
				instance = GWT.create(LoginService.class);
			}
			return instance;
		}		
	}
}
