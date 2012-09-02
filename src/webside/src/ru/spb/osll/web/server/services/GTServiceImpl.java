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

package ru.spb.osll.web.server.services;

import java.util.ArrayList;
import java.util.List;

import javax.servlet.http.HttpSession;

import org.apache.log4j.Logger;

import ru.spb.osll.log.Log;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.server.DefaultValues;
import ru.spb.osll.web.server.JGeoConnector;
import ru.spb.osll.web.server.Session;
import ru.spb.osll.web.server.SettingsStorage;
import ru.spb.osll.web.server.WebLogger;
import ru.spb.osll.json.*;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class GTServiceImpl extends RemoteServiceServlet implements 
		GTService, Session.HasSession {
	
	public static String serverUrl = 
			SettingsStorage.getInstance().getValue(DefaultValues.SERVER_URL).toString() + "service";
	
	
	static {
		Log.setLogger(new WebLogger());
	}

	@Override
	public WUser login(WUser user) throws IllegalArgumentException {
		Logger.getLogger(getClass()).debug("called login()");
		JsonLoginRequest request = new JsonLoginRequest(user.getLogin(), user.getPassword(), serverUrl);
		JsonLoginResponse response = new JsonLoginResponse();
		response.parseJson(request.doRequest());
		if (response.getAuthString() == null) {
			return null;
		} else {
			WUser newUser = new WUser(user.getLogin(), user.getPassword());
			newUser.setToken(response.getAuthString());
			newUser.setStatus(Response.STATUS_SUCCES);
			Session.Instance().addValue(this, USER_TOKEN, newUser.getToken());
			return newUser;
		}
	}

	@Override
	public boolean logout() throws IllegalArgumentException {
		Session.Instance().removeValue(this, USER_TOKEN);
		return true;
	}

	@Override
	public WUser isAuthorized() throws IllegalArgumentException {
	    Logger.getLogger(getClass()).debug("called isAuthorized()");
		if (Session.Instance().getValue(this, USER_TOKEN) != null) {
	        final String authToken = Session.Instance().getValue(this, USER_TOKEN).toString();
	        WUser user = new WUser();
	        user.setToken(authToken);
	        return user; 
		}	    
		return null;
	}

	@Override
	public WUser addUser(WUser user) throws IllegalArgumentException {
		JsonAddUserRequest request = new JsonAddUserRequest(user.getEmail(),
													        user.getLogin(), 
														    user.getPassword(), 
														    serverUrl);
		JsonAddUserResponse response = new JsonAddUserResponse();
		response.parseJson(request.doRequest());
		if (response.getAuthToken() == null) {
			return null;
		} else {
			WUser newUser = new WUser(user.getLogin(), user.getPassword());
			newUser.setEmail(user.getEmail());
			newUser.setPassword(user.getPassword());
			newUser.setToken(response.getAuthToken());
			return newUser;
		}
	}
	
	public Boolean registerUser(WUser user) throws IllegalArgumentException {
		JsonRegisterUserReguest request = new JsonRegisterUserReguest(user.getEmail(),
														     	      user.getLogin(), 
														     	      user.getPassword(), 
														     	      serverUrl);
		JsonRegisterUserResponse response = new JsonRegisterUserResponse();
		response.parseJson(request.doRequest());
		if (response.getErrno() == Errno.SUCCESS) {
			return true;
		} else {
			return false;
		}
	}

	@Override
	public Boolean subscribe(WChannel ch, WUser u) throws IllegalArgumentException {
		JsonSubscribeRequest request = new JsonSubscribeRequest(u.getToken(), ch.getName(), serverUrl);
		JsonSubscribeResponse response = new JsonSubscribeResponse();
		response.parseJson(request.doRequest());
		if (response.getErrno() == Errno.SUCCESS) {
			return true;
		} else {
			return false;
		}
	}

	@Override
	public Boolean unsubscribe(WChannel ch, WUser u)
			throws IllegalArgumentException {
		JsonUnsubscribeRequest request = new JsonUnsubscribeRequest(u.getToken(), ch.getName(), serverUrl);
		JsonUnsubscribeResponse response = new JsonUnsubscribeResponse();
		response.parseJson(request.doRequest());
		if (response.getErrno() == Errno.SUCCESS) {
			return true;
		} else {
			return false;
		}
	}

	@Override
	public List<WChannel> availableChannels(WUser u) throws IllegalArgumentException {
		JsonAvailableChannelRequest request = new JsonAvailableChannelRequest(u.getToken(), serverUrl);
		JsonAvailableChannelResponse response = new JsonAvailableChannelResponse();
		response.parseJson(request.doRequest());
		List<WChannel> channels = new ArrayList<WChannel>();
		for (int i = 0; i < response.getChannels().size(); i++) {
			WChannel channel = JGeoConnector.toWChannel(response.getChannels().get(i));
			channels.add(channel);
		}
		return channels;
	}

	@Override
	public List<WChannel> subscribedChannels(WUser u) throws IllegalArgumentException {
		JsonSubscribedChannelsRequest request = new JsonSubscribedChannelsRequest(u.getToken(), serverUrl);
		JsonSubscribedChannelsResponse response = new JsonSubscribedChannelsResponse();
		response.parseJson(request.doRequest());
		List<WChannel> channels = new ArrayList<WChannel>();
		for (int i = 0; i < response.getChannels().size(); i++) {
			WChannel channel = JGeoConnector.toWChannel(response.getChannels().get(i));
			channels.add(channel);
		}
		return channels;
	}

	@Override
	public List<WChannel> ownedChannels(WUser u) throws IllegalArgumentException {
		// TODO Later
		return null;
	}

	@Override
	public List<WMark> getTags(WUser u, WChannel ch, int amount) 
			throws IllegalArgumentException {
		JsonFilterChannelRequest request = new JsonFilterChannelRequest(u.getToken(), ch.getName(), amount, serverUrl);
		JsonFilterChannelResponse response = new JsonFilterChannelResponse();
		response.parseJson(request.doRequest());
		List<WMark> marks = new ArrayList<WMark>();
		for (int i = 0; i < response.getChannels().size(); i++) {
			for (int j = 0; j < response.getChannels().get(i).getMarks().size(); j++) {
				WMark mark = JGeoConnector.toWTag(response.getChannels().get(i).getMarks().get(j));
				marks.add(mark);
			}
		}
		return marks;
	}

    @Override
    public List<WMark> getTags(WUser u, List<WChannel> channels, int amount)
            throws IllegalArgumentException {
        Logger.getLogger(getClass()).debug("called getTags(WUser u, List<WChannel> channels, int amount)");
        final List<WMark> marks = new ArrayList<WMark>();
        for (WChannel ch : channels) {
            marks.addAll(getTags(u, ch, amount));
        }        
        return marks;
    }
	
	@Override
	public List<WMark> getTags(WUser u, double latitude, double longitude, double radius) 
			throws IllegalArgumentException {
		JsonLoadTagsRequest request = new JsonLoadTagsRequest(u.getToken(), latitude, longitude, radius, serverUrl);
		JsonLoadTagsResponse response = new JsonLoadTagsResponse();
		response.parseJson(request.doRequest());
		List<WMark> marks = new ArrayList<WMark>();
		for (int i = 0; i < response.getChannels().size(); i++) {
			for (int j = 0; j < response.getChannels().get(i).getMarks().size(); j++) {
				WMark mark = JGeoConnector.toWTag(response.getChannels().get(i).getMarks().get(j));
				marks.add(mark);
			}
		}
		return marks;
	}
	
	@Override
	public Boolean restorePassword(String email) 
			throws IllegalArgumentException {
		JsonRestorePasswordRequest request = new JsonRestorePasswordRequest(email, serverUrl);
		JsonRestorePasswordResponse response = new JsonRestorePasswordResponse();
		response.parseJson(request.doRequest());
		if (response.getErrno() == Errno.SUCCESS) {
			return true;
		} else {
			return false;
		}
	}
	
	@Override
	public HttpSession getSession() {
		return getThreadLocalRequest().getSession();
	}
	
	private static final String USER_TOKEN = "user.token";

}