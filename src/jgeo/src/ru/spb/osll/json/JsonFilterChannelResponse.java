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

package ru.spb.osll.json;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.log.Log;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;

public class JsonFilterChannelResponse extends JsonBaseResponse {

	private List<Channel> m_channels = new ArrayList<Channel>();

	@Override
	public void parseJson(JSONObject obj) {
		super.parseJson(obj);
		try {
				JSONObject jchannel = obj.getJSONObject("channel");
				final String channelName = jchannel.getString("name");
				Channel channel = new Channel(channelName);

				JSONArray jmarks = jchannel.getJSONArray("items");
				for (int j = 0; j < jmarks.length(); j++) {
					JSONObject jmark = jmarks.getJSONObject(j);

					Mark mark = new Mark();
					mark.setChannel(channelName);
					mark.setId(jmark.getLong("id"));
					mark.setUser(jmark.getString("user"));
					mark.setTitle(jmark.getString("title"));
					mark.setLink(jmark.getString("link"));
					mark.setDescription(jmark.getString("description"));
					mark.setLatitude(jmark.getDouble("latitude"));
					mark.setLongitude(jmark.getDouble("longitude"));
					mark.setAltitude(jmark.getDouble("altitude"));
					mark.setTime(jmark.getString("pubDate"));

					channel.addMark(mark);
				}
				m_channels.add(channel);
		} catch (JSONException e) {
			Log.out.println(LOG, e);
		}
	}

	public List<Channel> getChannels() {
		return m_channels;
	}
}
