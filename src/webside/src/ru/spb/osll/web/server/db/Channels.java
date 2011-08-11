package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.Statement;
import java.util.List;
import java.util.ArrayList;
import org.apache.log4j.Logger;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.common.DBUtil;

public class Channels {
	public static final String TABLE 		= "channel";
	
	public static final String ID 			= "id";
	public static final String NAME 		= "name";
	public static final String DESCRIPTION 	= "description";
	public static final String URL 			= "url";

	
	
	private static final String INSERT_CHANNEL 
		= "INSERT INTO (name, description, url) channel VALUES ('%s', '%s', '%s');";
	public static Channel insert(Channel ch){  
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(INSERT_CHANNEL, ch.getName(), ch.getDescription(), ch.getUrl());
			statement.execute(query, Statement.RETURN_GENERATED_KEYS); 
			final ResultSet rs = statement.getGeneratedKeys();
			rs.next();
			final long id = rs.getLong(1); 
			ch.setId(id);
			return ch;
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("insertChannel", e.getCause());
		    return null;
		}
	}

	private static final String DELETE_CHANNEL = "DELETE FROM channel WHERE name='%s';";	
	public static boolean delete(Channel channel){
		boolean success = false; 
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(DELETE_CHANNEL, channel.getName());
			statement.execute(query);     
			success = true;
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("deleteChannel", e.getCause());
		}
		return success;
	}	
	
	private static final String UPDATE_CHANNEL = 
		"UPDATE channel SET name='%s', description='%s', url='%s' WHERE id='%s';";
	public static boolean update(Channel ch){
		boolean success = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(UPDATE_CHANNEL, ch.getName(), ch.getDescription(), ch.getUrl(), ch.getId());
			statement.execute(query);
			success = true;
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("update", e.getCause());
		}
		return success;
	}
	
	private static final String SELECT_ALL_CHANNELS = "SELECT * FROM channel;";
	public static List<Channel> selectAll(){
		Channel channel = null;
		List<Channel> listChannels = new ArrayList<Channel>();
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = SELECT_ALL_CHANNELS;
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	channel = new Channel();	
		    	channel.setId(result.getInt(ID)); 
		    	channel.setName(result.getString(NAME));
		    	channel.setDescription(result.getString(DESCRIPTION));
		    	channel.setUrl(result.getString(URL));
		    	listChannels.add(channel);
		    }
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("selectChannels", e.getCause());
		}
	    return listChannels;
	}


	private static final String SELECT_BY_USER =
			"SELECT * FROM channel INNER JOIN subscribe ON subscribe.channel_id = channel.id" +
			" WHERE subscribe.user_id = %s;";
	public static List<Channel> selectByUser(User user){
		Channel channel = null;
		List<Channel> listChannels = new ArrayList<Channel>();
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(SELECT_BY_USER, user.getId());
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	channel = new Channel();
		    	channel.setId(result.getLong(ID)); 
		    	channel.setName(result.getString(NAME));
		    	channel.setDescription(result.getString(DESCRIPTION));
		    	channel.setUrl(result.getString(URL));
		    	listChannels.add(channel);
		    }
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("selectChannelsByUser", e.getCause());
		}
	    return listChannels;
	}

	public static List<Channel> selectUnuse(User user){
		// TODO 
		return null;
	}	

	public static boolean subscribeToChannel(Channel ch, User user){
		// TODO
		return true;
	}	

	public static boolean unsubscribeFromChannel(Channel ch, User user){
		// TODO
		return true;
	}	

}
