package ru.spb.osll.web.client;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.services.users.LoginServiceAsync;

public class GTState {
	private User m_currenUser;
	private Channel m_currenChannel;

	private List<UserStateListener> m_userHandlers;
	private List<ChannelStateListener> m_channelHandlers;

	public void addOnUserStateListerer(UserStateListener l){
		m_userHandlers.add(l);
	}

	public void addOnChannelStateListerer(ChannelStateListener l){
		m_channelHandlers.add(l);
	}
	
	public void setCurUser(User user){
		m_currenUser = user;
		for(UserStateListener l : m_userHandlers){
			l.onUserChange(user);
		}
	}

	public User getCurUser(){
		return m_currenUser;
	}	
	
	public void setCurChannel(Channel ch){
		m_currenChannel = ch;
		for(ChannelStateListener l : m_channelHandlers){
			l.onChannelChange(ch);
		}
	}

	public Channel getCurChannel(){
		return m_currenChannel;
	}	

	
	// GT STATE LISTENERS 
	public static interface UserStateListener{
		public void onUserChange(User u);
	}

	public static interface ChannelStateListener{
		public void onChannelChange(Channel ch);
	}

	// GT ACTIONS
	public void checkAuth(){
		LoginServiceAsync service = LoginService.Util.getInstance();
		service.isAuthorized(new AsyncCallback<User>() {
			@Override
			public void onSuccess(User user) {
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
