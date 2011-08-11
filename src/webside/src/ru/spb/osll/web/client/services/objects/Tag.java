package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;
import java.sql.Timestamp;

@SuppressWarnings("serial")
public class Tag implements Serializable {

	private long id;
	private java.sql.Timestamp time;
    private float latitude;
    private float longitude;
	private String label;
	private String description;
	private String url;
	private long user_id;
	
	public Tag() {
	}
	
	public Tag(final String label, final String url, final long user_id) {
		this.label = label;
		this.url = url;
		this.user_id = user_id;
	}
	
	public long getId() {
		return id;
	}
	
	public void setId(long id) {
		this.id = id;
	}
	
	public Timestamp getTime() {
		return time;
	}
	public void setTime(Timestamp time) {
		this.time = time;
	}
	
	public float getLatitude() {
		return latitude;
	}
	
	public void setLatitude(float latitude) {
		this.latitude = latitude;
	}
	
	public float getLongitude() {
		return longitude;
	}
	
	public void setLongitude(float longitude) {
		this.longitude = longitude;
	}
	
	public String getLabel() {
		return label;
	}
	
	public void setLabel(String label) {
		this.label = label;
	}
	
	public String getDescription() {
		return description;
	}
	
	public void setDescription(String description) {
		this.description = description;
	}
	
	public String getUrl() {
		return url;
	}
	
	public void setUrl(String url) {
		this.url = url;
	}
	
	public long getUserId() {
		return user_id;
	}
	
	public void setUserId(long user_id) {
		this.user_id = user_id;
	}
	
}
