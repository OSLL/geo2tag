package ru.spb.osll.objects;

public class User {
	private String name;
	private String pass;
	private String token;

	public User(){
	}
	
	public User(String name, String pass, String token){
		this(name, pass);
		this.token = token;
	}
	
	public User(String name, String pass){
		this.name = name;
		this.pass = pass;
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
	}
	public String getPass() {
		return pass;
	}
	public void setPass(String pass) {
		this.pass = pass;
	}
	public String getToken() {
		return token;
	}
	public void setToken(String token) {
		this.token = token;
	}
}
