package ru.spb.osll.web.server.db.common;

import com.google.gwt.i18n.client.Constants;

public interface DB extends Constants {
	String host();
	String port();
	String base();
	String user();
	String pass();
}
