package ru.spb.osll.web.server.db;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.List;
import java.util.ArrayList;
import org.apache.log4j.Logger;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.objects.Subscribe;
import ru.spb.osll.web.server.db.common.DBUtil;

public class Channels {
	public static final String TABLE 		= "channel";
	public static final String ID 			= "id";
	public static final String NAME 		= "name";
	public static final String DESCRIPTION 	= "description";
	public static final String URL 			= "url";

	private static final String INSERT_CHANNEL = "INSERT INTO channels VALUES (%li, '%s', '%s', '%s');";
	public static boolean insert(Channel channel){ 
		boolean success = true; 
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(INSERT_CHANNEL, channel.getId(), channel.getName(), channel.getDescription(), channel.getUrl());
			final int result = Ex1Stmt.executeUpdate(query);     
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("insertChannel", e.getCause());
		    success = false;
		}
		return success;
	}

	private static final String DELETE_CHANNEL = "DELETE FROM channels WHERE name='%s';";	
	public static boolean delete(Channel channel){
		boolean success = true; 
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(DELETE_CHANNEL, channel.getName());
			final int result = Ex1Stmt.executeUpdate(query);     
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("deleteChannel", e.getCause());
			success = false;
		}
		return success;
	}	
	
	public static boolean update(Channel channel){
		return delete(channel) && insert(channel);
	}
	
	private static final String SELECT_ALL_CHANNELS = "SELECT * FROM channels;";
	public static List<Channel> selectAll(){
		Channel channel = null;
		List<Channel> listChannels = new ArrayList<Channel>();
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = SELECT_ALL_CHANNELS;
			final ResultSet result = Ex1Stmt.executeQuery(query);
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

	private static final String SELECT_BY_USER = "SELECT * FROM subscribe WHERE user_id='%s';";
	private static final String SELECT_CHANNELS = "SELECT * FROM channels WHERE id='%s';";
	public static final String CHANNEL_ID 		= "channel_id";
	public static final String USER_ID 			= "user_id";
	public static List<Channel> selectByUser(User user){
		Channel channel = null;
		Subscribe subscribe = null;
		List<Channel> listChannels = new ArrayList<Channel>();
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(SELECT_BY_USER, user.getId());
			final ResultSet result = Ex1Stmt.executeQuery(query);
			long temp_channel_id;
			String temp_query;
			ResultSet temp_result;
		    while (result.next()) {
		    	subscribe = new Subscribe();	
		    	temp_channel_id = result.getInt(CHANNEL_ID); 
		    	temp_query = String.format(SELECT_CHANNELS, temp_channel_id);
		    	temp_result = Ex1Stmt.executeQuery(temp_query);
		    	channel = new Channel();
		    	channel.setId(temp_result.getInt(ID)); 
		    	channel.setName(temp_result.getString(NAME));
		    	channel.setDescription(temp_result.getString(DESCRIPTION));
		    	channel.setUrl(temp_result.getString(URL));
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
	
}
