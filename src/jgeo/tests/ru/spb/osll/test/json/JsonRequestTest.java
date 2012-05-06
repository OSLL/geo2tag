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

import junit.framework.TestCase;

import org.json.JSONObject;

import ru.spb.osll.json.JsonAddUserRequest;
import ru.spb.osll.json.JsonAddUserResponse;
import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyChannelResponse;
import ru.spb.osll.json.JsonAvailableChannelRequest;
import ru.spb.osll.json.JsonAvailableChannelResponse;
import ru.spb.osll.json.JsonFilterBoxRequest;
import ru.spb.osll.json.JsonFilterCircleRequest;
import ru.spb.osll.json.JsonFilterCylinderRequest;
import ru.spb.osll.json.JsonFilterFenceRequest;
import ru.spb.osll.json.JsonFilterPolygonRequest;
import ru.spb.osll.json.JsonFilterRectangleRequest;
import ru.spb.osll.json.JsonFilterResponse;
import ru.spb.osll.json.JsonLoginRequest;
import ru.spb.osll.json.JsonLoginResponse;
import ru.spb.osll.json.JsonSubscribeRequest;
import ru.spb.osll.json.JsonSubscribeResponse;
import ru.spb.osll.json.JsonUnsubscribeRequest;
import ru.spb.osll.json.JsonUnsubscribeResponse;
import ru.spb.osll.log.Log;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;

public class JsonRequestTest extends TestCase {
	public static final String LOG = "TEST";

	// GEO2TAG INSTANCE
	public final String url = "http://tracks.osll.spb.ru:81/service";

	public void testLogin() {
		JSONObject JSONResponse = new JsonLoginRequest("Mark", "test", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonLoginResponse r = new JsonLoginResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertEquals(r.getAuthString(), "MMMMMMMMMM");
		}
	}
	
	public void testSubscribe() {
		JSONObject JSONResponse = new JsonSubscribeRequest("KKKKKKKKKK", "Public announcements", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonSubscribeResponse r = new JsonSubscribeResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
		}
	}
	
	public void testUnsubscribe() {
		JSONObject JSONResponse = new JsonUnsubscribeRequest("KKKKKKKKKK", "Public announcements", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonUnsubscribeResponse r = new JsonUnsubscribeResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
		}
	}

	public void testAvailableChannels() {
		JSONObject JSONResponse = new JsonAvailableChannelRequest("KKKKKKKKKK", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonAvailableChannelResponse r = new JsonAvailableChannelResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
		}
	}

	public void testAddUser() {
		final JSONObject JSONResponse = new JsonAddUserRequest("sergpetrov@gmail.com", "sergpetrov",
				"sergAP", url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonAddUserResponse r = new JsonAddUserResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
		}
	}
	
	public void testApplyChannel() {
		final JSONObject JSONResponse = new JsonApplyChannelRequest(
				"MMMMMMMMMM", "Test channel", "My test channel",
				"http://osll.spb.ru", 3000, url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonApplyChannelResponse r = new JsonApplyChannelResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
		}
	}

	public void testCircleFilter() {
		final JSONObject JSONResponse = new JsonFilterCircleRequest(
				"MMMMMMMMMM", 60.1632, 24.8593, 30.0,
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testCylinderFilter() {
		final JSONObject JSONResponse = new JsonFilterCylinderRequest(
				"MMMMMMMMMM", 60.1632, 24.8593, 30.0, 1.0, -1.0,
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testRectangleFilter() {
		final JSONObject JSONResponse = new JsonFilterRectangleRequest(
				"MMMMMMMMMM", 0.0, 100.0, 0.0, 100.0,
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testBoxFilter() {
		final JSONObject JSONResponse = new JsonFilterBoxRequest("MMMMMMMMMM",
				0.0, 100.0, 0.0, 100.0, -0.1, 0.1, "04 03 2011 15:33:47.630",
				"31 12 2011 15:33:47.630", url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testPolygonFilter() {
		JsonFilterPolygonRequest filter = new JsonFilterPolygonRequest(
				"MMMMMMMMMM", "04 03 2011 15:33:47.630",
				"31 12 2011 15:33:47.630", url);
		filter.addPoint(0.0, 0.0);
		filter.addPoint(70.0, 0.0);
		filter.addPoint(70.0, 100.0);
		final JSONObject JSONResponse = filter.doRequest();

		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testFenceFilter() {
		JsonFilterPolygonRequest filter = new JsonFilterFenceRequest(
				"MMMMMMMMMM", -0.1, 0.1, "04 03 2011 15:33:47.630",
				"31 12 2011 15:33:47.630", url);
		filter.addPoint(0.0, 0.0);
		filter.addPoint(70.0, 0.0);
		filter.addPoint(70.0, 100.0);
		final JSONObject JSONResponse = filter.doRequest();

		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

}
