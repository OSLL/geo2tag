package ru.spb.osll.web.client.services.channels;

import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface ChannelServiceAsync {

	void getAllChannels(AsyncCallback<List<Channel>> callback);

	void getUserChannels(User u, AsyncCallback<List<Channel>> callback);

	void subscribe(Channel ch, User u, AsyncCallback<Boolean> callback);

	void unsubscribe(Channel ch, User u, AsyncCallback<Boolean> callback);

}
