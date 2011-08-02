package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;

@SuppressWarnings("serial")
public class User implements Serializable {

	private long id; 
	private String login;
	private String password;
	private String token;
	
	public User() {
	}
	
	public User(final String login, final String password) {
		this.login = login;
		this.password = password;
	}
	
	public long getId() {
		return id;
	}
	
	public void setId(long id) {
		this.id = id;
	}
	
	public String getLogin() {
		return login;
	}
	public void setLogin(String login) {
		this.login = login;
	}
	
	public String getPassword() {
		return password;
	}
	
	public void setPassword(String password) {
		this.password = password;
	}
	
	public String getToken() {
		return token;
	}

	public void setToken(String token) {
		this.token = token;
	}

}
