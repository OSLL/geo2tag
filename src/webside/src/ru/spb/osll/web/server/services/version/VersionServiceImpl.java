package ru.spb.osll.web.server.services.version;

import org.apache.log4j.Logger;

import java.io.FileInputStream;
import java.util.Properties;

import ru.spb.osll.web.client.services.objects.VersionInfo;
import ru.spb.osll.web.client.services.version.VersionService;
import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class VersionServiceImpl extends RemoteServiceServlet implements VersionService {

	@Override
	public VersionInfo getVersionInfo() throws IllegalArgumentException {
		VersionInfo versionInfo = null;
		try {
			final String path = System.getenv("CATALINA_HOME") + "/geo2tag-build";
			final Properties geo2tagBuild = new Properties();
			final FileInputStream in = new FileInputStream(path);
			geo2tagBuild.load(in);
			in.close();
			
			versionInfo = new VersionInfo();
			versionInfo.setVersion(geo2tagBuild.getProperty("version"));
			versionInfo.setDeployDate(geo2tagBuild.getProperty("deployDate"));
		} catch (Exception e) {
			Logger.getLogger(getClass()).error("getVersionInfo", e.getCause());
		}
		return versionInfo;
	}
}
