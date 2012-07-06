package ru.spb.osll.web.client;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;

import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.services.users.LoginServiceAsync;

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
		LoginServiceAsync service = LoginService.Util.getInstance();
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
