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

package ru.spb.osll.web.client.services;

import java.util.List;

import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WUser;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface GTServiceAsync {

	void login(WUser user, AsyncCallback<WUser> callback);

	void logout(AsyncCallback<Boolean> callback);

	void isAuthorized(AsyncCallback<WUser> callback);

	void addUser(WUser user, AsyncCallback<WUser> callback);
	
	void registerUser(WUser user, AsyncCallback<Boolean> callback);

	void subscribe(WChannel ch, WUser u, AsyncCallback<Boolean> callback);

	void unsubscribe(WChannel ch, WUser u, AsyncCallback<Boolean> callback);

	void availableChannels(WUser u, AsyncCallback<List<WChannel>> callback);

	void subscribedChannels(WUser u, AsyncCallback<List<WChannel>> callback);

	void ownedChannels(WUser u, AsyncCallback<List<WChannel>> callback);

    void getTags(WUser u, WChannel ch, int amount, AsyncCallback<List<WMark>> callback);

	void getTags(WUser u, List<WChannel> channels, int amount, AsyncCallback<List<WMark>> callback);
	
	void getTags(WUser u, double latitude, double longitude, double radius, AsyncCallback<List<WMark>> callback);

}
