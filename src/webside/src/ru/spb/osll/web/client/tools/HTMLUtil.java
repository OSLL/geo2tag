package ru.spb.osll.web.client.tools;

import ru.spb.osll.web.client.services.objects.Tag;


public class HTMLUtil {
	
	public static String toHTML(Tag tag) {
		StringBuffer sb = new StringBuffer().append("<b>").
			append(tag.getLabel()).append("</b><br>");
		if (tag.getUrl() != null){
			sb.append(htmlLink(tag.getUrl(), tag.getDescription())).append("<br>");
		} else {
			sb.append(tag.getDescription()).append("<br>");
		}
		sb.append(tag.getTime().toGMTString()).append("<br>");
		return sb.toString();
	}
		
	private static String htmlLink(String url, String linkName){
		return new StringBuffer().append("<a href=\"").
			append(url).append("\">").append(linkName).
			append("</a>").toString();
	}

	public static String text(String text, int size){
		return new StringBuffer().append("<font size=\"").
			append(size).append("\">").
			append(text).append("</font>").
			toString();
	}
	
	public static String bold(String text){
		return new StringBuffer().append("<b>").
			append(text).append("</b>").
			toString();
	}

}
