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

package ru.spb.osll.web.client.services.tags;

import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.services.objects.WChannel;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("TagService")
public interface TagService extends RemoteService {

	List<WMark> getTags(WUser u) 
			throws IllegalArgumentException;

	List<WMark> getTags(WUser u, Date dateFrom, Date dateTo)
			throws IllegalArgumentException;
	
	List<WMark> getTags(WChannel ch) 
			throws IllegalArgumentException;

	List<WMark> getTags(WChannel ch, Date dateFrom, Date dateTo)
			throws IllegalArgumentException;

	List<WMark> getTags(List<WChannel> channels, Date dateFrom, Date dateTo)
			throws IllegalArgumentException;

	
	public static class Util {
		private static TagServiceAsync instance;
		public static TagServiceAsync getInstance(){
			if (instance == null) {
				instance = GWT.create(TagService.class);
			}
			return instance;
		}
	}
}
