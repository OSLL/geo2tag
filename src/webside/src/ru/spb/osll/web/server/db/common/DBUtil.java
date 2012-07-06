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

package ru.spb.osll.web.server.db.common;

import java.sql.*;

import org.apache.log4j.Logger;
import org.scb.gwt.web.server.i18n.GWTI18N;

public class DBUtil {

	private static Connection connection = null;

	public static Connection getConnection() {
		if (connection == null) {
			try {
				DB dbconf = GWTI18N.create(DB.class);
				final String host = dbconf.host();
				final String port = dbconf.port();
				final String base = dbconf.base();
				final String user = dbconf.user();
				final String pass = dbconf.pass();

				final String preurl = "jdbc:postgresql://%s:%s/%s";
				Class.forName("org.postgresql.Driver").newInstance();
				System.setProperty("jdbc.drivers", "org.postgresql.Driver");
				final String url = String.format(preurl, host, port, base);
				connection = DriverManager.getConnection(url, user, pass);
				return connection;
			} catch (Exception e) {
				Logger.getLogger(DBUtil.class).error(e.getLocalizedMessage());
			}
		}
		return connection;
	}

}
