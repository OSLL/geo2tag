package ru.spb.osll.error;

public class Error {
	private String m_errorMess;
	
	public Error(String errorMess){
		m_errorMess = errorMess;
	}
	
	public String getMessage(){
		return m_errorMess;
	}
}
