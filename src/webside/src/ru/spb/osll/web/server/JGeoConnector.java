/*
 * Copyright 2012 OSLL
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

package ru.spb.osll.web.server;

import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;

import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;
import ru.spb.osll.objects.User;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WUser;

public class JGeoConnector {

	public static WUser toWUser(User user) {
		final WUser wUser = new WUser();
		wUser.setLogin(user.getName());
		wUser.setPassword(user.getPass());
		wUser.setToken(user.getToken());

		return wUser;
	}

	public static WMark toWTag(Mark mark){
		final WMark wMark = new WMark();
		SimpleDateFormat date = new SimpleDateFormat("dd MM yyyy hh:mm:ss.SSS");
		wMark.setDescription(mark.getDescription());
		wMark.setId(mark.getId());
		wMark.setLabel(mark.getTitle());
		wMark.setLatitude((float) mark.getLatitude());
		wMark.setLongitude((float) mark.getLongitude());
		try {
			wMark.setTime(new Timestamp(date.parse(mark.getTime()).getTime()));
		} catch (ParseException e) {
			e.printStackTrace();
		}
		wMark.setAltitude(mark.getAltitude());
		wMark.setChannel(mark.getChannel());
		wMark.setUser(mark.getUser());
		wMark.setUrl(mark.getLink());

		return wMark;
	}	 

	public static WChannel toWChannel(Channel channel) {
		final WChannel wChannel = new WChannel();
		wChannel.setDescription(channel.getDescription());
		wChannel.setName(channel.getName());
		wChannel.setUrl(channel.getLink());
		wChannel.setLanguage(channel.getLanguage());
		wChannel.setPubDate(channel.getPubDate());
		wChannel.setTitle(channel.getTitle());

		return wChannel;
	}

}
