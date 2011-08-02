package ru.spb.osll.web.client.services.users;

import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface LoginServiceAsync {
	void login(User user, AsyncCallback<User> callback)
		throws IllegalArgumentException;

	void addUser(User user, AsyncCallback<User> callback)
		throws IllegalArgumentException;
}
