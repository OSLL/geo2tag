package ru.spb.osll.web.client.services.objects;

import java.io.Serializable;

@SuppressWarnings("serial")
public class VersionInfo implements Serializable {
	public String getVersion() {
		return version;
	}
	public void setVersion(String version) {
		this.version = version;
	}
	public String getDeployDate() {
		return deployDate;
	}
	public void setDeployDate(String deployDate) {
		this.deployDate = deployDate;
	}
	
	private String version;
	private String deployDate;
}