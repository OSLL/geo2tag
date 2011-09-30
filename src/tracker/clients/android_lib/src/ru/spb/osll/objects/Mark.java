package ru.spb.osll.objects;


public class Mark {
	private long id;
	private String authToken;
	private String channel;
	private String title;
	private String link;
	private String description;
	private double latitude;
	private double longitude;
	private String time;
	
	public long getId() {
		return id;
	}
	public void setId(long id) {
		this.id = id;
	}
	public String getAuthToken() {
		return authToken;
	}
	public void setAuthToken(String authToken) {
		this.authToken = authToken;
	}
	public String getChannel() {
		return channel;
	}
	public void setChannel(String channel) {
		this.channel = channel;
	}
	public String getTitle() {
		return title;
	}
	public void setTitle(String title) {
		this.title = title;
	}
	public String getLink() {
		return link;
	}
	public void setLink(String link) {
		this.link = link;
	}
	public String getDescription() {
		return description;
	}
	public void setDescription(String description) {
		this.description = description;
	}
	public double getLatitude() {
		return latitude;
	}
	public void setLatitude(double latitude) {
		this.latitude = latitude;
	}
	public double getLongitude() {
		return longitude;
	}
	public void setLongitude(double longitude) {
		this.longitude = longitude;
	}
	public String getTime() {
		return time;
	}
	public void setTime(String time) {
		this.time = time;
	}
	@Override
	public String toString() {
		return "Mark [id=" + id + "\n, authToken=" + authToken + "\n, channel="
				+ channel + "\n, title=" + title + "\n, link=" + link
				+ "\n, description=" + description + "\n, latitude=" + latitude
				+ "\n, longitude=" + longitude + "\n, time=" + time + "]";
	}
	
}
