package ru.spb.osll.web.server.db;

import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;

public class Channels {
	public static final String TABLE 		= "channel";
	public static final String ID 			= "id";
	public static final String NAME 		= "name";
	public static final String DESCRIPTION 	= "description";
	public static final String URL 			= "url";

	public static Channel insert(Channel channel){
		// TODO
		return null;
	}

	public static Channel update(Channel channel){
		// TODO
		return null;
	}

	public static boolean delete(Channel channel){
		// TODO
		return true;
	}

	public static List<Channel> selectAll(){
		// TODO
		return null;
	}

	public static List<Channel> selectByUser(User user){
		// TODO
		return null;
	}

	public static List<Channel> selectUnuse(User user){
		// TODO
		return null;
	}
	
}
