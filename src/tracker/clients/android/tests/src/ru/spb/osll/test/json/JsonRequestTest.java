package ru.spb.osll.test.json;

import java.util.List;

import org.json.JSONObject;

import ru.spb.osll.json.IRequest.IResponse;
import ru.spb.osll.json.JsonAddUserRequest;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonFilterCircleRequest;
import ru.spb.osll.json.JsonFilterResponse;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;
import ru.spb.osll.utils.TrackerUtil;
import android.test.AndroidTestCase;
import android.util.Log;

public class JsonRequestTest extends AndroidTestCase {
	public static final String LOG = "TEST";
	
	// GEO2TAG INSTANCE
	//public final String url = "http://tracks.osll.spb.ru:81/service"; 
	
	// GEO2TAG INSTANCE
	//public final String url = "http://192.168.0.105:81/service";
	public final String url = "http://109.124.93.108:81/service";
	
	public void testLogin(){
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			JSONObject JSONResponse = new JsonLoginRequest("Mark", "test", url).doRequest();
			assertNotNull(JSONResponse);
			if (JSONResponse != null) {
				assertEquals("MMMMMMMMMM", JsonBase.getString(JSONResponse, IResponse.AUTH_TOKEN));
			} 
		} else {
			Log.v(LOG, "client offline");
		}
	}

	public void testAddUser(){
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			final JSONObject JSONResponse = new JsonAddUserRequest("bac1ca", "test", url).doRequest();
			assertNotNull(JSONResponse);
			if (JSONResponse != null) {
				Log.d(LOG, JSONResponse.toString());
				
				String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
				String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
				boolean success = status.equals(IResponse.OK_STATUS) || statusDescription.equals(IResponse.USER_EXTSTS); 
				assertTrue(success);
			} 
		} else {
			Log.v(LOG, "client offline");
		}
	}
	
	public void testApplyChannel(){
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			final JSONObject JSONResponse = new JsonApplyChannelRequest("MMMMMMMMMM", "My channel",
							"tracker channel", "http://osll.spb.ru", 3000, url).doRequest();
			assertNotNull(JSONResponse);
			if (JSONResponse != null) {
				Log.d(LOG, JSONResponse.toString()); 
				String status = JsonBase.getString(JSONResponse, IResponse.STATUS);
				String statusDescription = JsonBase.getString(JSONResponse, IResponse.STATUS_DESCRIPTION);
				boolean success = status.equals(IResponse.OK_STATUS) || statusDescription.equals(IResponse.CHANNEL_EXTSTS); 
				assertTrue(success);
			} 
		} else {
			Log.v(LOG, "client offline");
		}
	}

	
	public void testCircleFilters(){
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			final JSONObject JSONResponse = new JsonFilterCircleRequest("MMMMMMMMMM", 60.1632,
				24.8593, 30.0, "04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url).doRequest();
			assertNotNull(JSONResponse);
			if (JSONResponse != null) {
				Log.d(LOG, JSONResponse.toString()); 
				JsonFilterResponse r = new JsonFilterResponse();
				r.parseJson(JSONResponse);
				assertEquals(r.getErrno(), 0);
				assertTrue(r.getChannelsData().size() > 0);
				List<Channel> channels = r.getChannelsData();				
				for (Channel c : channels) {
					Log.d(LOG, "" + c.getName());
					for (Mark m : c.getMarks()) {
						Log.d(LOG, "" + m);
					}
				}
			} 
		} else {
			Log.v(LOG, "client offline");
		}
	}

}
