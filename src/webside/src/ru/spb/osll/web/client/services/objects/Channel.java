package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;

@SuppressWarnings("serial")
public class Channel implements Serializable {
	
	private long id; 
	private String name;
	private String description;
	private String url;
	
	public Channel() {
	}
	
	public Channel(final String name, final String decsription, final String url) {
		this.name = name;
		this.description = decsription;
		this.url = url;
	}
	
	public long getId() {
		return id;
	}
	
	public void setId(long id) {
		this.id = id;
	}
	
	public String getName() {
		return name;
	}
	public void setName(String name) {
		this.name = name;
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

}
