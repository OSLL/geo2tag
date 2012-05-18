/*
 * Copyright 2010-2011  Edward Ryabikov  edward.ryabikov@gmail.com
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

package ru.spb.osll.json;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import ru.spb.osll.log.Log;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;

public class JsonLoadTagsResponse extends JsonBaseResponse {

	private List<Channel> m_channels = new ArrayList<Channel>();

	@Override
	public void parseJson(JSONObject obj) {
		super.parseJson(obj);
		try {
				JSONObject jrss = obj.getJSONObject("rss");
				JSONObject jchannels = jrss.getJSONObject("channels");
				JSONArray jchannelsItems = jchannels.getJSONArray("items");
				for (int i = 0; i < jchannelsItems.length(); i++) {
					JSONObject jchannel = jchannelsItems.getJSONObject(i);
					String name = jchannel.getString("name");
					JSONArray jmarks = jchannel.getJSONArray("items");
					Channel channel = new Channel();
					channel.setName(name);
					for (int j = 0; j < jmarks.length(); j++) {
						Mark mark = new Mark();
						mark.setUser(jmarks.getJSONObject(j).getString("user"));
						mark.setTitle(jmarks.getJSONObject(j).getString("title"));
						mark.setLink(jmarks.getJSONObject(j).getString("link"));
						mark.setDescription(jmarks.getJSONObject(j).getString("description"));
						mark.setLatitude(jmarks.getJSONObject(j).getDouble("latitude"));
						mark.setLongitude(jmarks.getJSONObject(j).getDouble("longitude"));
						mark.setAltitude(jmarks.getJSONObject(j).getDouble("altitude"));
						mark.setTime(jmarks.getJSONObject(j).getString("pubDate"));

						channel.addMark(mark);
					}
					m_channels.add(channel);
				}
		} catch (JSONException e) {
			Log.out.println(LOG, e);
		}
	}

	public List<Channel> getChannels() {
		return m_channels;
	}
	
}
