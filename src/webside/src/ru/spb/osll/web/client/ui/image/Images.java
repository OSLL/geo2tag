package ru.spb.osll.web.client.ui.image;

import com.google.gwt.core.client.GWT;

public class Images {
	
	private static ResImages res;
	
	public static ResImages res(){
		if (res == null){
			res = GWT.create(ResImages.class);
		}
		return res;
	}

}
