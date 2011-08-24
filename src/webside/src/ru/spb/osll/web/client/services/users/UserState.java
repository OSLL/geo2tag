package ru.spb.osll.web.client.services.users;

import ru.spb.osll.web.client.services.objects.User;

public class UserState {

	private User m_currenUser;

	public User getCurUser(){
		return m_currenUser;
	}	
	public void setCurUser(User user){
		m_currenUser = user;
	}
	

	public static UserState Instanse(){
		if (instance == null){
			instance = new UserState();
		}
		return instance;
	}
	private static UserState instance;
	private UserState(){};

}
