package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.SQLException;
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

	
	private static final String INSERT_TAG = "INSERT INTO tag(latitude, longitude, label, description, user_id, url)" +
			" VALUES (%s, %s, %s, '%s', '%s', '%s', %s);";
	public static Tag insert(Tag tag){  
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(INSERT_TAG, tag.getLatitude(), tag.getLongitude(), 
					tag.getLabel(), tag.getDescription(), tag.getUserId(), tag.getUrl());
			statement.execute(query, Statement.RETURN_GENERATED_KEYS); 
			final ResultSet rs = statement.getGeneratedKeys();
			rs.next();
			final long id = rs.getLong(1); 
			tag.setId(id);
			return tag;
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("insertTag", e.getCause());
		}
		return tag;
	}

	
	private static final String DELETE_TAG = "DELETE FROM tag WHERE id=%s;";	
	public static boolean delete(Tag tag){
		boolean success = true; 
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(DELETE_TAG, tag.getId());
			statement.execute(query);     
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("deleteTag", e.getCause());
			success = false;
		}
		return success;
	}	

	
	private static final String UPDATE_TAG = "UPDATE tag SET latitude='%s', longitude='%s', label='%s'," +
			" description='%s', url='%s', user_id='%s' WHERE id='%s';";
	public static boolean update(Tag tag){
		boolean success = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(UPDATE_TAG, tag.getLatitude(), tag.getLongitude(), tag.getLabel(),
					tag.getDescription(), tag.getUrl(), tag.getUserId(), tag.getId());
			statement.execute(query);
			success = true;
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("update", e.getCause());
		}
		return success;
	}
	
	
	private static final String SELECT_TAGS_BY_USER = "SELECT * FROM tag WHERE user_id=%s;";
	public static List<Tag> selectByUser(User user){
		List<Tag> tags = new ArrayList<Tag>();
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(SELECT_TAGS_BY_USER, user.getId());
			final ResultSet result = statement.executeQuery(query);
			selectInternal(result, tags);
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("selectTagByUser", e.getCause());
		}
	    return tags;
	}

	
	private static final String SELECT_TAGS_BY_CHANNEL = "SELECT time, id, latitude, longitude, label, description, url, user_id" +
			" FROM tag INNER JOIN tags ON tag.id = tags.tag_id WHERE tags.channel_id = '%s';";
	public static List<Tag> selectByChannel(Channel channel){
		List<Tag> tags = new ArrayList<Tag>();
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(SELECT_TAGS_BY_CHANNEL, channel.getId());
			final ResultSet result = statement.executeQuery(query);
			selectInternal(result, tags);
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("selectTagsByChannel", e.getCause());
		}
	    return tags;
	}	
	
	private static void selectInternal(ResultSet result, List<Tag> tags) throws SQLException{
		if (tags == null){
			return;
		}
		Tag tag = null;
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
	    	tags.add(tag);
	    }		
	}
	
	
	private static final String INSERT_TAG_CHANNEL = "INSERT INTO tags(channel_id, tag_id) VALUES (%s, %s);";
	public static boolean addTagToChannel(Tag tag, Channel ch){
		boolean succes = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(INSERT_TAG_CHANNEL, ch.getId() , tag.getId());
			statement.execute(query);
			succes = true;
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("addTagToChannel", e.getCause());
		}
		return succes;
	}


	private static final String DELETE_TAG_CHANNEL = "DELETE FROM tags WHERE tag_id=%s;";	
	public static boolean removeTagFromChannel(Tag tag){
		boolean succes = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(DELETE_TAG_CHANNEL, tag.getId());
			statement.execute(query);
			succes = true;
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("addTagToChannel", e.getCause());
		}
		return succes;
	}

	
	public static boolean updateTagFromChannel(Tag tag, Channel ch){
		// TODO
		return true;
	}
	
}

