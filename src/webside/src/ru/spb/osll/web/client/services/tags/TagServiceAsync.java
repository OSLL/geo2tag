package ru.spb.osll.web.client.services.tags;

import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TagServiceAsync {

	void getTags(Channel ch, AsyncCallback<List<Tag>> callback);

}
