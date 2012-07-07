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

package ru.spb.osll.web.client;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;

import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.GTServiceAsync;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WUser;

public class GTState {
	private WUser m_currenUser;
	private WChannel m_currenChannel;

	private List<UserStateListener> m_userHandlers;
	private List<ChannelStateListener> m_channelHandlers;

	public void addOnUserStateListerer(UserStateListener l){
		m_userHandlers.add(l);
	}

	public void addOnChannelStateListerer(ChannelStateListener l){
		m_channelHandlers.add(l);
	}
	
	public void setCurUser(WUser user){
		m_currenUser = user;
		for(UserStateListener l : m_userHandlers){
			l.onUserChange(user);
		}
	}

	public WUser getCurUser(){
		return m_currenUser;
	}	
	
	public void setCurChannel(WChannel ch){
		m_currenChannel = ch;
		for(ChannelStateListener l : m_channelHandlers){
			l.onChannelChange(ch);
		}
	}

	public WChannel getCurChannel(){
		return m_currenChannel;
	}	

	
	// GT STATE LISTENERS 
	public static interface UserStateListener{
		public void onUserChange(WUser u);
	}

	public static interface ChannelStateListener{
		public void onChannelChange(WChannel ch);
	}

	// GT ACTIONS
	public void checkAuth(){
		GTServiceAsync service = GTService.Util.getInstance();
		service.isAuthorized(new AsyncCallback<WUser>() {
			@Override
			public void onSuccess(WUser user) {
				GTState.Instanse().setCurUser(user); 
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("checkAuth", caught);
			}
		});
	}
	
	
	
	private GTState(){
		m_userHandlers = new ArrayList<UserStateListener>();
		m_channelHandlers = new ArrayList<ChannelStateListener>();
	};
	private static GTState instance;
	public static GTState Instanse(){
		if (instance == null){
			instance = new GTState();
		}
		return instance;
	}
}
