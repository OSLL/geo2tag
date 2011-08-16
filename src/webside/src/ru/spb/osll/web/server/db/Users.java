package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.SQLException;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;

public class Users extends AbstractBase<User> {
	public static final String TABLE 	= "users";
	
	public static final String ID 		= "id";
	public static final String LOGIN 	= "login";
	public static final String PASSWORD	= "password";
	public static final String TOKEN 	= "token";
	
	public static Users Instance(){
		if (instance == null){
			instance = new Users();
		}
		return instance;
	}
	private static Users instance;
	private Users(){}


	public User select(String login){
		final String selectUser = "SELECT * FROM users WHERE login='%s';";
		final String query = String.format(selectUser, login);
		return baseSingleSelect(query);
	}

	public User select(long id){
		final String selectUser = "SELECT * FROM users WHERE id=%s;";
		final String query = String.format(selectUser, id);
		return baseSingleSelect(query);
	}
	
	public User insert(User user){
		final String insertUser = "INSERT INTO users (login, password, token) VALUES ('%s', '%s', '%s');";
		final String query = String.format(insertUser, user.getLogin(), user.getPassword(), user.getToken());
		final long id = baseInsert(query); 
		if (id != -1){
			user.setId(id);
		}else {
			user = null;
		}
		return user;
	}
	
	public boolean delete(User user){
		final String deleteUser = "DELETE FROM users WHERE login='%s';";
		final String query = String.format(deleteUser, user.getLogin());
		return baseBoolQuery(query);
	}	
	
	public boolean update(User u){
		final String updateUser = "UPDATE users SET password='%s', token='%s' WHERE login='%s';";
		final String query = String.format(updateUser, u.getPassword(), u.getToken(), u.getLogin());
		return baseBoolQuery(query);
	}
	
	public boolean subscribeToChannel(Channel ch, User user){
		return subscribeToChannel(ch.getId(), user.getId());
	}	
	
	public boolean unsubscribeFromChannel(Channel ch, User user){
		return unsubscribeFromChannel(ch.getId(), user.getId());
	}	
	
	public boolean subscribeToChannel(long channelId, long userId){
		final String subscribe = "INSERT INTO subscribe(channel_id, user_id) values('%s', '%s');";
		final String query = String.format(subscribe, channelId, userId);
		return baseBoolQuery(query);
	}	

	public boolean unsubscribeFromChannel(long channelId, long userId){
		final String unsubscribe = "DELETE FROM subscribe WHERE channel_id='%s' AND user_id='%s';";
		final String query = String.format(unsubscribe, channelId, userId);
		return baseBoolQuery(query);
	}
	
	@Override
	protected User constructObject(ResultSet result) throws SQLException {
		User user = new User();	
    	user.setId(result.getInt(ID)); 
    	user.setLogin(result.getString(LOGIN));
    	user.setPassword(result.getString(PASSWORD));
    	user.setToken(result.getString(TOKEN));
		return user;
	}	

}
