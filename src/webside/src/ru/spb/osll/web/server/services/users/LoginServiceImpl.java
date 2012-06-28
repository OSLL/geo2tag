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

package ru.spb.osll.web.server.services.users;

import javax.servlet.http.HttpSession;

import org.json.JSONObject;

import ru.spb.osll.json.JsonLoginResponse;
import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.server.db.Users;
import ru.spb.osll.web.server.Session;
import ru.spb.osll.json.*;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class LoginServiceImpl extends RemoteServiceServlet implements
		LoginService, Session.HasSession {
	public static String serverUrl = "http://tracks.osll.spb.ru:81/service";

	@Override
	public WUser login(WUser user) throws IllegalArgumentException {
		JSONObject JSONResponse = null;
		JSONResponse = new JsonLoginRequest(user.getLogin(), 
			user.getPassword(), serverUrl).doRequest();
		if (JSONResponse != null) {
			JsonLoginResponse r = new JsonLoginResponse();
			r.parseJson(JSONResponse);
			user.setToken(r.getAuthString());
			user.setStatus(r.getErrno());
			Session.Instance().addValue(this, USER_ID, user.getId());
		} else {
			user.setStatus(Response.STATUS_FAIL);
		}
		return user;
	}

	@Override
	public boolean logout() throws IllegalArgumentException {
		Session.Instance().removeValue(this, USER_ID);
		return true;
	}

	@Override
	public WUser isAuthorized() throws IllegalArgumentException {
		final Long id = (Long) Session.Instance().getValue(this, USER_ID);
		return id == null ? null : Users.Instance().select(id);
	}

	@Override
	public WUser addUser(WUser user) throws IllegalArgumentException {
		final Users users = Users.Instance();
		final WUser standart = users.select(user.getLogin());
		if (standart == null) {
			user = users.insert(user);
			user.setStatus(Response.STATUS_SUCCES);
		} else {
			user.setStatus(Response.STATUS_FAIL);
		}
		return user;
	}

	@Override
	public HttpSession getSession() {
		return getThreadLocalRequest().getSession();
	}

	private static final String USER_ID = "user.id";
}
