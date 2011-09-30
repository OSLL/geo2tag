package ru.spb.osll.web.client.ui.core;

import java.util.HashMap;
import java.util.Map;

import com.google.gwt.user.client.ui.Widget;

public class SiteMap {
	SimpleMenu m_menu;
	private Map<String, Widget> m_siteMap = new HashMap<String, Widget>();
	
	public SiteMap(SimpleMenu menu){
		m_menu = menu;
	}

	protected void addWidget(Widget w){
		m_siteMap.put(getToken(w), w);
	}

	public Widget getWidget(String token){
		return m_siteMap.get(token);
	}

	public String getToken(Widget w){
		if (w == null){
			return "";
		}
		final String group = m_menu.getGroup(w);
		final String token = getTokenByClass(w.getClass());
		return group == null ? token : group + "_" + token;
	}
	
	public static String getTokenByClass(Class<?> cwClass) {
		String className = cwClass.getName();
		className = className.substring(className.lastIndexOf('.') + 1);
		return className;
	}

}
