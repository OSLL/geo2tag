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

package ru.spb.osll.web.client.tools;

import ru.spb.osll.web.client.services.objects.WMark;


public class HTMLUtil {
	
	public static String toHTML(WMark tag) {
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
