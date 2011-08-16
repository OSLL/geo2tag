package ru.spb.osll.web.server;

import javax.servlet.http.HttpSession;

public class Session {
	
	public void addValue(HasSession servlet, String key, Object value){
		servlet.getSession().setAttribute(key, value);
	}

	public Object getValue(HasSession servlet, String key){
		 return servlet.getSession().getAttribute(key);
	}

	public void removeValue(HasSession servlet, String key){
		 servlet.getSession().removeAttribute(key);
	}
	
	public static synchronized Session Instance(){
		if (instance == null){
			instance = new Session();
		}
		return instance;
	}
	private static Session instance;
	private Session(){};

	public interface HasSession{
		HttpSession getSession();
	}
}
