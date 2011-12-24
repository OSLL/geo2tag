package ru.spb.osll.json;

import java.util.ArrayList;
import java.util.List;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;

public class JsonFilterResponse extends JsonBaseResponse {

	private List<Channel> m_channels = new ArrayList<Channel>();
	
	@Override
	public void parseJson(JSONObject obj) {
		super.parseJson(obj);
		try {
			JSONArray jchannels = obj.getJSONArray("channels");
			for (int i = 0; i < jchannels.length(); i++) {
				JSONObject jchannel  = jchannels.getJSONObject(i).getJSONObject("channel");
				final String channelName = jchannel.getString("name");
				Channel channel = new Channel(channelName);
				
				JSONArray jmarks = jchannel.getJSONArray("items");
				for (int j = 0; j < jmarks.length(); j++) {
					JSONObject jmark = jmarks.getJSONObject(j); 
					
					Mark mark = new Mark();
					mark.setChannel(channelName);
					mark.setUser(jmark.getString("user"));
					mark.setTitle(jmark.getString("title"));
					mark.setLink(jmark.getString("link"));
					mark.setDescription(jmark.getString("description"));
					mark.setLatitude(jmark.getDouble("latitude"));
					mark.setLongitude(jmark.getDouble("longitude"));
//					mark.setAltitude(jmark.getDouble("altitude")); // TODO
					mark.setAltitude(0.0);
					mark.setTime(jmark.getString("pubDate"));
					
					channel.addMark(mark);
				}
				m_channels.add(channel);
			}
		} catch (JSONException e) {
			Log.e(LOG, e.getMessage());
		}
	}
	
	public List<Channel> getChannelsData() {
		return m_channels;
	}

}
