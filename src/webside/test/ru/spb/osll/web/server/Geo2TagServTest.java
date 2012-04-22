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

package ru.spb.osll.web.server;

import java.util.List;

import org.json.JSONObject;
import junit.framework.TestCase;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.Channels;
import ru.spb.osll.web.server.db.Tags;
import ru.spb.osll.web.server.db.Users;
import ru.spb.osll.web.server.json.JsonBase;
import ru.spb.osll.web.server.json.JsonFilterCircleRequest;
import ru.spb.osll.web.server.json.JsonFilterRectangleRequest;
import ru.spb.osll.web.server.json.JsonLoginRequest;

public class Geo2TagServTest extends TestCase {

	public void testLogin() {
		String m_authToken;
		JSONObject JSONResponse = null;
		JSONResponse = new JsonLoginRequest("Paul", "test", JsonBase
				.getServerUrl()).doRequest();
		m_authToken = JsonBase.getString(JSONResponse, "auth_token");
		assertEquals(m_authToken, "PPPPPPPPPP");
		String errno;
		errno = JsonBase.getString(JSONResponse, "errno");
		int error = Integer.parseInt(errno);
		assertTrue(error == 0);
	}

	public void testCircleFilter() {
		String channels;
		JSONObject JSONResponse = null;
		JSONResponse = new JsonFilterCircleRequest("MMMMMMMMMM",
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", 60.1632,
				24.8593, 30.0, JsonBase.getServerUrl()).doRequest();
		channels = JsonBase.getString(JSONResponse, "channels");
		assertEquals(channels, "[]");
	}

	public void testRectangleFilter() {
		String channels;
		JSONObject JSONResponse = null;
		JSONResponse = new JsonFilterRectangleRequest("MMMMMMMMMM",
				"04 03 2011 15:33:47.630", "31 12 2011 15:33:47.630", 0.0,
				100.0, 0.0, 100.0, JsonBase.getServerUrl()).doRequest();
		channels = JsonBase.getString(JSONResponse, "channels");
		assertEquals(channels, "[]");
	}

	public void testUsers() {
		User testUser = new User("Dima", "pass");
		Users.Instance().delete(testUser);

		testUser = Users.Instance().insert(testUser);
		testUser = testUser != null ? testUser : Users.Instance()
				.select("Dima");
		assertTrue(Users.Instance().select("Dima").getPassword().equals("pass"));

		testUser.setToken("blablabla");
		Users.Instance().update(testUser);
		String testToken = Users.Instance().select("Dima").getToken();
		assertTrue(testToken.equals("blablabla"));

		assertTrue(Users.Instance().delete(testUser));
		assertTrue(Users.Instance().select("Dima") == null);
	}

	public void testChannels() {
		Channel testChannel = new Channel("Channel1", "TestChannel",
				"www.bac1ca.com/channel");

		Channels.Instance().delete(testChannel);
		assertTrue(Channels.Instance().delete(testChannel)); // FIXME

		assertTrue(null != Channels.Instance().insert(testChannel));
		assertTrue(null != Channels.Instance().insert(testChannel));

		testChannel.setDescription("blablabla");
		assertTrue(Channels.Instance().update(testChannel));

		List<Channel> channels = Channels.Instance().selectAll();
		assertTrue(channels.get(channels.size() - 1).getDescription().equals(
				"blablabla"));

		assertTrue(Channels.Instance().delete(testChannel));
	}

	public void testSubscribe() {
		User testUser = new User("Dima", "pass");
		Users.Instance().delete(testUser);
		testUser = Users.Instance().insert(testUser);

		Channel testChannel1 = new Channel("Channel1", "TestChannel",
				"www.bac1ca.com/channel");
		Channel testChannel2 = new Channel("Channel2", "TestChannel",
				"www.bac1ca.com/channel");
		Channels.Instance().delete(testChannel1);
		Channels.Instance().delete(testChannel2);
		testChannel1 = Channels.Instance().insert(testChannel1);
		testChannel2 = Channels.Instance().insert(testChannel2);

		assertTrue(Users.Instance().subscribeToChannel(testChannel1, testUser));
		assertTrue(Users.Instance().subscribeToChannel(testChannel2, testUser));
		// assertFalse(Users.Instance().subscribeToChannel(testChannel1,
		// testUser)); // FIXME
		// assertFalse(Users.Instance().subscribeToChannel(testChannel2,
		// testUser)); // FIXME

		List<Channel> channels = Channels.Instance().selectByUser(testUser);
		assertTrue(2 == channels.size());

		assertTrue(Users.Instance().unsubscribeFromChannel(testChannel1,
				testUser));
		// assertFalse(Users.Instance().unsubscribeFromChannel(testChannel1,
		// testUser)); //FIXME

		channels = Channels.Instance().selectByUser(testUser);
		assertTrue(1 == channels.size());

		Users.Instance().delete(testUser);
		Channels.Instance().delete(testChannel1);
		Channels.Instance().delete(testChannel2);
	}

	public void testTags() {
		User testUser = new User("Dima", "pass");
		Users.Instance().delete(testUser);
		testUser = Users.Instance().insert(testUser);

		Tag testTag1 = new Tag(1f, 1f, "testlabel1", "testTag1", "testurl",
				testUser.getId());
		testTag1 = Tags.Instance().insert(testTag1);
		assertTrue(Tags.Instance().selectByUser(testUser).get(0).getLabel()
				.equals("testlabel1"));

		Tag testTag2 = new Tag(2f, 2f, "testlabel2", "testTag2", "testurl",
				testUser.getId());
		testTag2 = Tags.Instance().insert(testTag2);

		assertTrue(2 == Tags.Instance().selectByUser(testUser).size());
		Tags.Instance().delete(testTag2);
		assertTrue(1 == Tags.Instance().selectByUser(testUser).size());

		testTag1.setDescription("blablabla");
		assertTrue(Tags.Instance().update(testTag1));
		assertTrue(Tags.Instance().selectByUser(testUser).get(0)
				.getDescription().equals("blablabla"));

		// CHANNELS
		Channel testChannel1 = new Channel("Channel1", "TestChannel",
				"www.bac1ca.com/channel");
		Channel testChannel2 = new Channel("Channel2", "TestChannel",
				"www.bac1ca.com/channel");
		Channels.Instance().delete(testChannel1);
		Channels.Instance().delete(testChannel2);
		testChannel1 = Channels.Instance().insert(testChannel1);
		testChannel2 = Channels.Instance().insert(testChannel2);

		Tags.Instance().addTagToChannel(testChannel1, testTag1);
		Tags.Instance().addTagToChannel(testChannel2, testTag1);

		assertTrue(1 == Tags.Instance().selectByChannel(testChannel1).size());
		Tags.Instance().removeTagFromChannel(testChannel1, testTag1);
		assertTrue(0 == Tags.Instance().selectByChannel(testChannel1).size());

		Users.Instance().delete(testUser);
		Channels.Instance().delete(testChannel1);
		Channels.Instance().delete(testChannel2);
		Tags.Instance().delete(testTag1);
		Tags.Instance().delete(testTag2);
	}

}
