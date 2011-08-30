package ru.spb.osll.web.client.services.tags;

import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.user.client.rpc.AsyncCallback;

public interface TagServiceAsync {

	void getTags(User u, AsyncCallback<List<Tag>> callback);

	void getTags(User u, Date dateFrom, Date dateTo,
			AsyncCallback<List<Tag>> callback);
	
	void getTags(Channel ch, AsyncCallback<List<Tag>> callback);

	void getTags(Channel ch, Date dateFrom, Date dateTo,
			AsyncCallback<List<Tag>> callback);

}
