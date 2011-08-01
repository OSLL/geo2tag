package ru.spb.osll.web.client.localization;

import com.google.gwt.core.client.GWT;

public class Localizer {
	private static GTLocalization res;
	
	public static GTLocalization res(){
		if (res == null){
			res = GWT.create(GTLocalization.class);
		}
		return res;
	}

}
