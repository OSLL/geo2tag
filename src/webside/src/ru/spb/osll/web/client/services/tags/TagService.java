package ru.spb.osll.web.client.services.tags;

import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.RemoteService;
import com.google.gwt.user.client.rpc.RemoteServiceRelativePath;

@RemoteServiceRelativePath("TagService")
public interface TagService extends RemoteService {

	List<Tag> getTags(Channel ch) throws IllegalArgumentException;

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
