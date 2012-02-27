/*
 * Copyright 2010-2011  Vasily Romanikhin  bac1ca89@gmail.com
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

package ru.spb.osll.test.json;

import java.util.List;

import org.json.JSONObject;

import ru.spb.osll.json.*;
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
	public final String url = "http://109.124.92.188:81/service";
	
	public void testLogin(){
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			JSONObject JSONResponse = new JsonLoginRequest("Mark", "test", url).doRequest();
			assertNotNull(JSONResponse);
			if (JSONResponse != null) {
				Log.d(LOG, JSONResponse.toString());
				JsonLoginResponse r = new JsonLoginResponse();
				r.parseJson(JSONResponse);
				assertEquals(r.getStatus(), JsonOldResponse.STATUS_OK);
				assertEquals(r.getAuthString(), "MMMMMMMMMM");
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
				JsonAddUserResponse r = new JsonAddUserResponse();
				r.parseJson(JSONResponse);
				String status = r.getStatus();
				String statusMess = r.getStatusMessage();
				boolean success = status.equals(JsonOldResponse.STATUS_OK) || statusMess.equals(JsonOldResponse.SM_USER_EXTSTS);
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
			final JSONObject JSONResponse = new JsonApplyChannelRequest("MMMMMMMMMM", 
				"My channel", "tracker channel", "http://osll.spb.ru", 3000, url).doRequest();
			assertNotNull(JSONResponse);
			if (JSONResponse != null) {
				Log.d(LOG, JSONResponse.toString());
				JsonApplyChannelResponse r = new JsonApplyChannelResponse();
				r.parseJson(JSONResponse);
				String status = r.getStatus();
				String statusMess = r.getStatusMessage();
				boolean success = status.equals(JsonOldResponse.STATUS_OK) || statusMess.equals(JsonOldResponse.SM_CHANNEL_EXTSTS); 
				assertTrue(success);
			} 
		} else {
			Log.v(LOG, "client offline");
		}
	}
	
	public void testCircleFilter() {
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
	
	public void testCylinderFilter() {
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			final JSONObject JSONResponse = new JsonFilterCylinderRequest("MMMMMMMMMM", 60.1632,
				24.8593, 30.0, 1.0, -1.0, "04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url).doRequest();
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

	public void testRectangleFilter() {
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			final JSONObject JSONResponse = new JsonFilterRectangleRequest("MMMMMMMMMM", 0.0,
				100.0, 0.0, 100.0, "04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url).doRequest();
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

	public void testBoxFilter() {
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			final JSONObject JSONResponse = new JsonFilterBoxRequest("MMMMMMMMMM", 0.0,
				100.0, 0.0, 100.0, -0.1, 0.1, "04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url).doRequest();
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

	public void testPolygonFilter() {
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			JsonFilterPolygonRequest filter = new JsonFilterPolygonRequest("MMMMMMMMMM", 
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url);
			filter.addPoint(0.0, 0.0);
			filter.addPoint(70.0, 0.0);
			filter.addPoint(70.0, 100.0);
			final JSONObject JSONResponse = filter.doRequest();
			
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

	public void testFenceFilter() {
		final boolean isOnline = TrackerUtil.isOnline(getContext());
		assertTrue(isOnline);
		if (isOnline){
			JsonFilterPolygonRequest filter = new JsonFilterFenceRequest("MMMMMMMMMM", 
				-0.1, 0.1, "04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url);
			filter.addPoint(0.0, 0.0);
			filter.addPoint(70.0, 0.0);
			filter.addPoint(70.0, 100.0);
			final JSONObject JSONResponse = filter.doRequest();
			
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
