package ru.spb.osll.web.client.services.version;

import ru.spb.osll.web.client.services.objects.VersionInfo;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface VersionServiceAsync {

	void getVersionInfo(AsyncCallback<VersionInfo> callback);

}
