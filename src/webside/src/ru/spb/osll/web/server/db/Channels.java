package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;

public class Channels extends AbstractBase<Channel> {
	public static final String TABLE 		= "channel";
	
	public static final String ID 			= "id";
	public static final String NAME 		= "name";
	public static final String DESCRIPTION 	= "description";
	public static final String URL 			= "url";
	
	public static Channels Instance(){
		if (instance == null){
			instance = new Channels();
		}
		return instance;
	}
	private static Channels instance;
	private Channels() {};
	
	
	public Channel insert(Channel ch){  
		final String insertChannel = "INSERT INTO channel(name, description, url) VALUES('%s', '%s', '%s');";
		final String query = String.format(insertChannel, ch.getName(), ch.getDescription(), ch.getUrl());
		final long id = baseInsert(query); 
		if (id != -1){
			ch.setId(id);
		} else {
			ch = null;
		}
		return ch;
	}

	public boolean delete(Channel channel){
		final String deleteChannel = "DELETE FROM channel WHERE name='%s';";	
		final String query = String.format(deleteChannel, channel.getName());
		return baseBoolQuery(query);
	}	
	
	public boolean update(Channel ch){
		final String updateChannel = "UPDATE channel SET name='%s', description='%s', url='%s' WHERE id='%s';";
		final String query = String.format(updateChannel, ch.getName(), ch.getDescription(), ch.getUrl(), ch.getId());
		return baseBoolQuery(query);
	}
	
	public List<Channel> selectAll(){
		final String query = "SELECT * FROM channel;";
	    return baseMultiSelect(query);
	}

	public List<Channel> selectByUser(User user){
		final String selectByUser = "SELECT * FROM channel INNER JOIN subscribe " +
				" ON subscribe.channel_id = channel.id WHERE subscribe.user_id = %s;";
		final String query = String.format(selectByUser, user.getId());
	    return baseMultiSelect(query);
	}

	@Deprecated
	public List<Channel> selectUnuse(User user){
		// TODO 
		return null;
	}

	@Override
	protected Channel constructObject(ResultSet result) throws SQLException {
    	Channel channel = new Channel();
    	channel.setId(result.getLong(ID)); 
    	channel.setName(result.getString(NAME));
    	channel.setDescription(result.getString(DESCRIPTION));
    	channel.setUrl(result.getString(URL));
		return channel;
	}	

}
