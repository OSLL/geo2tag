package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;

@SuppressWarnings("serial")
public class Subscribe implements Serializable {

	private long channel_id; 
	private long user_id; 
	
	public Subscribe() {
	}
	
	public Subscribe(final long channel_id, final long user_id) {
		this.channel_id = channel_id;
		this.user_id = user_id;
	}
	
	public long getChannelId() {
		return channel_id;
	}
	
	public void setChannelId(long channel_id) {
		this.channel_id = channel_id;
	}
	
	public long getUserId() {
		return user_id;
	}
	
	public void setUserId(long user_id) {
		this.user_id = user_id;
	}
}
