package ru.spb.osll.web.server.db;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.List;
import java.util.ArrayList;
import org.apache.log4j.Logger;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.common.DBUtil;

public class Tags {
	public static final String TABLE 		= "tag";
	public static final String ID 			= "id";
	public static final String TIME 		= "time";
	public static final String LATITUDE 	= "latitude";
	public static final String LONGITUDE 	= "longitude";
	public static final String LABEL 		= "label";
	public static final String DESCRIPTION 	= "description";
	public static final String URL 			= "url";
	public static final String USERID   	= "user_id";

	private static final String INSERT_TAG = "INSERT INTO tag VALUES (%s, current_timestamp, %s, %s, '%s', '%s', '%s', %s);";
	public static Tag insert(Tag tag){  
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final ResultSet res = Ex1Stmt.executeQuery("SELECT MAX(id) FROM tag;");
			long id = 0;
			while (res.next()) {
			id = res.getInt("max");
			};
			id = id + 1;
			tag.setId(id);
			final String query = String.format(INSERT_TAG, id + "", tag.getLatitude() + "",
					                           tag.getLongitude() + "", tag.getLabel(), tag.getDescription(), 
					                           tag.getUrl(), tag.getUserId() + "");
			final int result = Ex1Stmt.executeUpdate(query);
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("insertTag", e.getCause());
		}
		return tag;
	}

	private static final String DELETE_TAG = "DELETE FROM tag WHERE id=%s;";	
	public static boolean delete(Tag tag){
		boolean success = true; 
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(DELETE_TAG, tag.getId());
			final int result = Ex1Stmt.executeUpdate(query);     
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("deleteTag", e.getCause());
			success = false;
		}
		return success;
	}	
	
	public static Tag update(Tag tag){
		boolean success = delete(tag);
		return insert(tag);
	}
	
	private static final String SELECT_TAG_BY_USER = "SELECT * FROM tag WHERE user_id=%s;";
	public static List<Tag> selectByUser(User user){
		Tag tag = null;
		List<Tag> listTags = new ArrayList<Tag>();
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(SELECT_TAG_BY_USER, user.getId());
			final ResultSet result = Ex1Stmt.executeQuery(query);
		    while (result.next()) {
		    	tag = new Tag();
		    	tag.setId(result.getInt(ID)); 
		    	tag.setTime(result.getTimestamp(TIME));
		    	tag.setLatitude(result.getFloat(LATITUDE));
		    	tag.setLongitude(result.getFloat(LONGITUDE));
		    	tag.setLabel(result.getString(LABEL));
		    	tag.setDescription(result.getString(DESCRIPTION));
		    	tag.setUrl(result.getString(URL));
		    	tag.setUserId(result.getInt(USERID));
		    	listTags.add(tag);
		    }
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("selectTagByUser", e.getCause());
		}
	    return listTags;
	}

	private static final String SELECT_TAG_BY_CHANNEL = "SELECT * FROM tags WHERE channel_id=%s;";
	private static final String SELECT_TAGS           = "SELECT * FROM tag WHERE id=%s;";
	private static final String TAG_ID                = "tag_id";
	public static List<Tag> selectByChannel(Channel channel){
		Tag tag = null;
		List<Tag> listTags = new ArrayList<Tag>();
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(SELECT_TAG_BY_CHANNEL, channel.getId());
			final ResultSet result = Ex1Stmt.executeQuery(query);
			long temp_tag_id;
			String temp_query;
			ResultSet temp_result = null;
		    while (result.next()) {
		    	try {
		    	    temp_tag_id = result.getInt(TAG_ID); 
		    	    temp_query = String.format(SELECT_TAGS, temp_tag_id);
		    	    temp_result = Ex1Stmt.executeQuery(temp_query);
		    	} catch (Exception e) {
					Logger.getLogger(Channels.class).error("selectChannelTags", e.getCause()); 
		    	}
		    	while (temp_result.next()) {
		    	    tag = new Tag();
		    	    tag.setId(temp_result.getInt(ID)); 
		    	    tag.setTime(temp_result.getTimestamp(TIME));
		    	    tag.setLatitude(temp_result.getFloat(LATITUDE));
		    	    tag.setLongitude(temp_result.getFloat(LONGITUDE));
		    	    tag.setLabel(temp_result.getString(LABEL));
		         	tag.setDescription(temp_result.getString(DESCRIPTION));
		        	tag.setUrl(temp_result.getString(URL));
		    	    tag.setUserId(temp_result.getInt(USERID));
		    	}
		    	listTags.add(tag);
		    }
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("selectTagsByChannel", e.getCause());
		}
	    return listTags;
	}	
}

