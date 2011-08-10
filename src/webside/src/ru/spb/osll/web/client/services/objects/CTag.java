package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;

@SuppressWarnings("serial")
public class CTag implements Serializable {

	private long channel_id; 
	private long tag_id; 
	
	public CTag() {
	}
	
	public CTag(final long channel_id, final long tag_id) {
		this.channel_id = channel_id;
		this.tag_id = tag_id;
	}
	
	public long getChannelId() {
		return channel_id;
	}
	
	public void setChannelId(long channel_id) {
		this.channel_id = channel_id;
	}
	
	public long getTagId() {
		return tag_id;
	}
	
	public void setTagId(long tag_id) {
		this.tag_id = tag_id;
	}
}

