package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;

@SuppressWarnings("serial")
public class Response implements Serializable {
	public static final int STATUS_SUCCES = 0;
	public static final int STATUS_FAIL = -1;
	
	private int m_status = STATUS_FAIL;
	private String m_message;
	
	public int getStatus() {
		return m_status;
	}
	public void setStatus(int status) {
		m_status = status;
	}
	public String getMessage() {
		return m_message;
	}
	public void setMessage(String message) {
		m_message = message;
	}
}
