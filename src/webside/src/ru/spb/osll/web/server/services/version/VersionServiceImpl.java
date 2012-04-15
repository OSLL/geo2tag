/*
 * Copyright 2011-2012 OSLL
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

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
