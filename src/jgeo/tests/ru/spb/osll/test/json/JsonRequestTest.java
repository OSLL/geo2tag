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

import ru.spb.osll.json.*;
import ru.spb.osll.log.Log;
import ru.spb.osll.objects.Channel;
import ru.spb.osll.objects.Mark;

public class JsonRequestTest extends TestCase {
	public static final String LOG = "TEST";

	// GEO2TAG INSTANCE
	public final String m_url = "http://tracks.osll.spb.ru:81/service";
	public static String m_authToken = "MMMMMMMMMM";
	
	public void testLogin() {
		Log.out.println(LOG + " Login", "Start");
		JSONObject JSONResponse = new JsonLoginRequest("Mark", "test", m_url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonLoginResponse r = new JsonLoginResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), Errno.SUCCESS);
			Log.out.println(LOG, r.getAuthString());
			m_authToken = r.getAuthString();
		}
		Log.out.println(LOG + " Login", "Finish");
	}
	
    public void testVersion() {
    	JSONObject JSONResponse = new JsonVersionRequest(m_url).doRequest();
        assertNotNull(JSONResponse);
        if (JSONResponse != null) {
            Log.out.println(LOG, JSONResponse.toString());
            JsonVersionResponse r = new JsonVersionResponse();
            r.parseJson(JSONResponse);
            assertEquals(r.getErrno(), 0);
            assertEquals(r.getVersionStr(), "0.19.1");
        }
    }

    public void testApplySubscribeUnsubscribe() {
    	final JSONObject JSONResponse = new JsonApplyChannelRequest(
                m_authToken, "Test channel2", "My test channel",
                "http://osll.spb.ru", 3000, m_url).doRequest();
        assertNotNull(JSONResponse);

        final JSONObject JSONResponse1 = new JsonSubscribeRequest(m_authToken,
                "Test channel2", m_url).doRequest();
        assertNotNull(JSONResponse1);

        final JSONObject JSONResponse2 = new JsonUnsubscribeRequest(
        		m_authToken, "Test channel2", m_url).doRequest();
        assertNotNull(JSONResponse2);

        if (JSONResponse != null) {
            Log.out.println(LOG, JSONResponse.toString());
            JsonApplyChannelResponse r = new JsonApplyChannelResponse();
            r.parseJson(JSONResponse);
            Log.out.println("Apply channel" + LOG, r.getErrno());
            boolean successApply = (r.getErrno() == Errno.SUCCESS)
                    || (r.getErrno() == Errno.CHANNEL_ALREADY_EXIST_ERROR);
            assertTrue(successApply);

            Log.out.println(LOG, JSONResponse1.toString());
            JsonSubscribeResponse r1 = new JsonSubscribeResponse();
            r1.parseJson(JSONResponse1);

            boolean successSubscribe = (r1.getErrno() == Errno.SUCCESS)
                    || (r1.getErrno() == Errno.CHANNEL_ALREADY_SUBSCRIBED_ERROR);
            assertTrue(successSubscribe);

            Log.out.println(LOG, JSONResponse2.toString());
            JsonUnsubscribeResponse r2 = new JsonUnsubscribeResponse();
            r2.parseJson(JSONResponse2);
            assertEquals(r2.getErrno(), Errno.SUCCESS);
        }
    }

	public void testAvailableChannels() {
		JSONObject JSONResponse = new JsonAvailableChannelRequest(m_authToken,
				m_url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonAvailableChannelResponse r = new JsonAvailableChannelResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannels().size() > 0);
			List<Channel> channels = r.getChannels();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
			}
		}
	}

	public void testAddUser() {
		final JSONObject JSONResponse = new JsonAddUserRequest(
				"sergpetrov@gmail.com", "sergpetrov", "sergAP", m_url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonAddUserResponse r = new JsonAddUserResponse();
			r.parseJson(JSONResponse);
			boolean successResult = (r.getErrno()==Errno.SUCCESS)||
					(r.getErrno()==Errno.USER_ALREADY_EXIST_ERROR);
			assertTrue(successResult);
		}
	}

	public void testApplyChannel() {
		final JSONObject JSONResponse = new JsonApplyChannelRequest(
				m_authToken, "Test channel", "My test channel",
				"http://osll.spb.ru", 3000, m_url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonApplyChannelResponse r = new JsonApplyChannelResponse();
			r.parseJson(JSONResponse);
			boolean successResult = (r.getErrno() == Errno.SUCCESS)
					|| (r.getErrno() == Errno.CHANNEL_ALREADY_EXIST_ERROR);
			assertTrue(successResult);
		}
	}

	public void testCircleFilter() {
		final JSONObject JSONResponse = new JsonFilterCircleRequest(
				m_authToken, 60.1632, 24.8593, 30.0,
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", m_url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			// assertTrue(r.getChannelsData().size() > 0);
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
				m_authToken, 60.1632, 24.8593, 30.0, 1.0, -1.0,
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", m_url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			// assertTrue(r.getChannelsData().size() > 0);
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
				m_authToken, 0.0, 100.0, 0.0, 100.0,
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", m_url)
				.doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			// assertTrue(r.getChannelsData().size() > 0);
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
		final JSONObject JSONResponse = new JsonFilterBoxRequest(m_authToken,
				0.0, 100.0, 0.0, 100.0, -0.1, 0.1, "04 03 2011 15:33:47.630",
				"31 12 2011 15:33:47.630", m_url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonFilterResponse r = new JsonFilterResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			// assertTrue(r.getChannelsData().size() > 0);
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
				m_authToken, "04 03 2011 15:33:47.630",
				"31 12 2011 15:33:47.630", m_url);
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
			// assertTrue(r.getChannelsData().size() > 0);
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
				m_authToken, -0.1, 0.1, "04 03 2011 15:33:47.630",
				"31 12 2011 15:33:47.630", m_url);
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
			// assertTrue(r.getChannelsData().size() > 0);
			List<Channel> channels = r.getChannelsData();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testFilterChannel() {
		final JSONObject JSONResponse = new JsonSubscribeRequest(m_authToken,
				"Fuel Prices", m_url).doRequest();
		assertNotNull(JSONResponse);

		final JSONObject JSONResponse1 = new JsonFilterChannelRequest(m_authToken,
				"Fuel Prices", 10, m_url).doRequest();
		assertNotNull(JSONResponse1);
		if (JSONResponse1 != null) {
			Log.out.println(LOG, JSONResponse1.toString());
			JsonFilterChannelResponse r = new JsonFilterChannelResponse();
			r.parseJson(JSONResponse1);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannels().size() > 0);
			List<Channel> channels = r.getChannels();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}

	public void testSubscribedChannels() {
		final JSONObject JSONResponse = new JsonSubscribedChannelsRequest(
				m_authToken, m_url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonSubscribedChannelsResponse r = new JsonSubscribedChannelsResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannels().size() > 0);
			List<Channel> channels = r.getChannels();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}
		}
	}
	
	public void testLoadTags() {
		double latitude = 60.166504; 
		double longitude = 24.841204;
		double radius = 30.0;
		final JSONObject JSONResponse = new JsonLoadTagsRequest(m_authToken, 
				latitude, longitude, radius, m_url).doRequest();
		assertNotNull(JSONResponse);
		if (JSONResponse != null) {
			Log.out.println(LOG, JSONResponse.toString());
			JsonLoadTagsResponse r = new JsonLoadTagsResponse();
			r.parseJson(JSONResponse);
			assertEquals(r.getErrno(), 0);
			assertTrue(r.getChannels().size() >= 0);
			List<Channel> channels = r.getChannels();
			for (Channel c : channels) {
				Log.out.println(LOG, "" + c.getName());
				for (Mark m : c.getMarks()) {
					Log.out.println(LOG, "" + m);
				}
			}	
		}
	}
	
    public void testRegisterUser() {
        final JSONObject JSONResponse = new JsonRegisterUserReguest(
                "sergpetrov@gmail.com", "sergpetrov", "sergAP", m_url)
                .doRequest();
        assertNotNull(JSONResponse);
        if (JSONResponse != null) {
            Log.out.println(LOG, JSONResponse.toString());
            JsonRegisterUserResponse r = new JsonRegisterUserResponse();
            r.parseJson(JSONResponse);
            boolean successResult = (r.getErrno() == Errno.SUCCESS)
                    || (r.getErrno() == Errno.USER_ALREADY_EXIST_ERROR);
            assertTrue(successResult);
        }
    }

}