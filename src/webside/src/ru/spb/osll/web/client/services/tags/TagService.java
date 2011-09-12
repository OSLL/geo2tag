package ru.spb.osll.web.client.services.tags;

import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("TagService")
public interface TagService extends RemoteService {

	List<Tag> getTags(User u) 
			throws IllegalArgumentException;

	List<Tag> getTags(User u, Date dateFrom, Date dateTo)
			throws IllegalArgumentException;
	
	List<Tag> getTags(Channel ch) 
			throws IllegalArgumentException;

	List<Tag> getTags(Channel ch, Date dateFrom, Date dateTo)
			throws IllegalArgumentException;

	List<Tag> getTags(List<Channel> channels, Date dateFrom, Date dateTo)
			throws IllegalArgumentException;

	
	public static class Util {
		private static TagServiceAsync instance;
		public static TagServiceAsync getInstance(){
			if (instance == null) {
				instance = GWT.create(TagService.class);
			}
			return instance;
		}
	}
}
