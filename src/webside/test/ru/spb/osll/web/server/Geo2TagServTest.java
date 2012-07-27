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

import junit.framework.TestCase;

import ru.spb.osll.log.Log;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.server.services.GTServiceImpl;

public class Geo2TagServTest extends TestCase {

	public static final String LOG = "TEST";
	public static String m_authToken = "MMMMMMMMMM";
	
	public void testLogin() {
		WUser user;
		GTServiceImpl service = new GTServiceImpl();
		user = service.login(new WUser("Paul", "test"));
		assertTrue(user != null);
		assertTrue(user.getLogin() == "Paul");
		assertTrue(user.getPassword() == "test");
		m_authToken = user.getToken();
	}
	
	public void testAddUser() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setEmail("sam@test.org");
		user = service.addUser(user);
		assertTrue(user == null);
	}
	
	public void testUnsubscribe() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setToken(m_authToken);
		WChannel channel = new WChannel("Paul", "Paul's channel", GTServiceImpl.serverUrl);
		Boolean result = service.unsubscribe(channel, user);
		assertTrue(result == true);
	}
	
	public void testSubscribe() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setToken(m_authToken);
		WChannel channel = new WChannel("Paul", "Paul's channel", GTServiceImpl.serverUrl);
		Boolean result = service.subscribe(channel, user);
		assertTrue(result == true);
	}
	
	public void testAvailableChannels() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setToken(m_authToken);
		List<WChannel> availableChannels= service.availableChannels(user);
		assertTrue(availableChannels.isEmpty() == false);
	}
	
	public void testSubscribedChannels() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setToken(m_authToken);
		List<WChannel> subscribedChannels= service.subscribedChannels(user);
		assertTrue(subscribedChannels.isEmpty() == false);
	}
	
	public void testGetTagsByFilterChannel() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setToken(m_authToken);
		WChannel channel = new WChannel("Fuel prices", "...", GTServiceImpl.serverUrl);
		int amount = 100;
		List<WMark> tags = service.getTags(user, channel, amount);
		assertTrue(tags.isEmpty() == true);
	}
	
	public void testGetTagsByLoadTags() {
		GTServiceImpl service = new GTServiceImpl();
		WUser user = new WUser("Paul", "test");
		user.setToken(m_authToken);
		double latitude = 60.166504;
		double longitude = 24.841204;
		double radius = 30.0;
		List<WMark> tags = service.getTags(user, latitude, longitude, radius);
		assertTrue(tags.isEmpty() == true);
	}
}
