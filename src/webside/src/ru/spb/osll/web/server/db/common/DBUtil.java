package ru.spb.osll.web.server.db.common;

import java.sql.*;

import org.apache.log4j.Logger;
import org.scb.gwt.web.server.i18n.GWTI18N;

public class DBUtil {

	private static Connection connection = null;

	public static Connection getConnction() {
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
