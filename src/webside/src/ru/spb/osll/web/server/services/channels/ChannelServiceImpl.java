package ru.spb.osll.web.server.services.channels;

import java.util.List;

import ru.spb.osll.web.client.services.channels.ChannelService;
import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.Channels;
import ru.spb.osll.web.server.db.Users;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class ChannelServiceImpl extends RemoteServiceServlet implements ChannelService {

	@Override
	public List<Channel> getAllChannels() throws IllegalArgumentException {
		return Channels.Instance().selectAll();
	}

	@Override
	public List<Channel> getUserChannels(User u) throws IllegalArgumentException {
		return Channels.Instance().selectByUser(u);
	}

	@Override
	public Boolean subscribe(Channel ch, User u) throws IllegalArgumentException {
		if (Users.Instance().isSubscribed(ch.getId(), u.getId())){
			return false;
		}
		return Users.Instance().subscribeToChannel(ch, u);
	}

	@Override
	public Boolean unsubscribe(Channel ch, User u) throws IllegalArgumentException {
		return Users.Instance().unsubscribeFromChannel(ch, u);
	}
}
