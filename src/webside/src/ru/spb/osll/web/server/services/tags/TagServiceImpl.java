package ru.spb.osll.web.server.services.tags;

import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.tags.TagService;
import ru.spb.osll.web.server.db.Tags;

import com.google.gwt.user.server.rpc.RemoteServiceServlet;

@SuppressWarnings("serial")
public class TagServiceImpl extends RemoteServiceServlet implements TagService {

	@Override
	public List<Tag> getTags(Channel ch) throws IllegalArgumentException {
		return Tags.Instance().selectByChannel(ch);
	}

}
