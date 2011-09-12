package ru.spb.osll.web.client.services.version;

import ru.spb.osll.web.client.services.objects.VersionInfo;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("VersionService")
public interface VersionService extends RemoteService {

	VersionInfo getVersionInfo() throws IllegalArgumentException;
	
	public static class Util {
		private static VersionServiceAsync instance;
		public static VersionServiceAsync getInstance(){
			if (instance == null) {
				instance = GWT.create(VersionService.class);
			}
			return instance;
		}
	}
}
