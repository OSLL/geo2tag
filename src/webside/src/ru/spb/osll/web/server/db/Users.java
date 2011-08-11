package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.Statement;

import org.apache.log4j.Logger;

import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.common.DBUtil;

public class Users {
	public static final String TABLE 	= "users";
	
	public static final String ID 		= "id";
	public static final String LOGIN 	= "login";
	public static final String PASSWORD	= "password";
	public static final String TOKEN 	= "token";
	
	
	private static final String INSERT_USER = "INSERT INTO users (login, password, token) VALUES ('%s', '%s', '%s');";
	public static User insert(User user){
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(INSERT_USER, user.getLogin(), user.getPassword(), user.getToken());
			statement.execute(query, Statement.RETURN_GENERATED_KEYS); 
			final ResultSet rs = statement.getGeneratedKeys();
			rs.next();
			final long id = rs.getLong(1); 
			user.setId(id);
			return user;
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("insertUser", e.getCause());
		}
		return null;
	}

	
	private static final String DELETE_USER = "DELETE FROM users WHERE login='%s';";
	public static boolean delete(User user){
		boolean success = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(DELETE_USER, user.getLogin());
			statement.execute(query);
			success = true;
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("delete", e.getCause());
		}
		return success;
	}	

	
	private static final String UPDATE_USER = "UPDATE users SET password='%s', token='%s' WHERE login='%s';";
	public static boolean update(User user){
		boolean success = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(UPDATE_USER, user.getPassword(), user.getToken(), user.getLogin());
			statement.execute(query);
			success = true;
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("update", e.getCause());
		}
		return success;
	}

	
	private static final String SELECT_USER = "SELECT * FROM users WHERE login='%s';";
	public static User select(String login){
		User user = null;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final String query = String.format(SELECT_USER, login);
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	user = new User();	
		    	user.setId(result.getInt(ID)); 
		    	user.setLogin(result.getString(LOGIN));
		    	user.setPassword(result.getString(PASSWORD));
		    	user.setToken(result.getString(TOKEN));
		    }
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("select", e.getCause());
		}
	    return user;
	}
	
}
