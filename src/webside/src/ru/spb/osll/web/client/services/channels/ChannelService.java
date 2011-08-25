package ru.spb.osll.web.client.services.channels;

import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("ChannelService")
public interface ChannelService extends RemoteService {

	List<Channel> getAllChannels() throws IllegalArgumentException; 
	
	List<Channel> getUserChannels(User u) throws IllegalArgumentException;
	
	Boolean subscribe(Channel ch, User u) throws IllegalArgumentException;
	
	Boolean unsubscribe(Channel ch, User u) throws IllegalArgumentException;
	
	public static class Util {
		private static ChannelServiceAsync instance;
		public static ChannelServiceAsync getInstance(){
			if (instance == null) {
				instance = GWT.create(ChannelService.class);
			}
			return instance;
		}
	}
}
