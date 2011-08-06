package ru.spb.osll.web.server.db;

import java.sql.Connection;
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
	private static long id = 0;
	
	private static final String INSERT_USER = "INSERT INTO users VALUES (%s, '%s', '%s', '%s');";
	
	public static User insert(User user){
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final ResultSet res = Ex1Stmt.executeQuery("SELECT MAX(id) FROM users;");
			long id = 0;
			while (res.next()) {
			id = res.getInt("max");
			};
			id = id + 1;
			user.setId(id);
			final String query = String.format(INSERT_USER, id + "", user.getLogin(), user.getPassword(), user.getToken());
			final int result = Ex1Stmt.executeUpdate(query);
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("insertUser", e.getCause());
		}
		return user;
	}
	
	private static final String DELETE_USER = "DELETE FROM users WHERE login='%s';";

	public static boolean delete(User user){
		boolean success = true; 
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(DELETE_USER, user.getLogin());
			final int result = Ex1Stmt.executeUpdate(query);     
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("deleteUser", e.getCause());
			success = false;
		}
		return success;
	}	

	public static User update(User user){
		boolean success = delete(user);
		return insert(user);
	}

	
	private static final String SELECT_USER = "SELECT * FROM users WHERE login='%s';";
	
	public static User select(String login){
		User user = null;
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(SELECT_USER, login);
			final ResultSet result = Ex1Stmt.executeQuery(query);
		    while (result.next()) {
		    	user = new User();	
		    	user.setId(result.getInt(ID)); 
		    	user.setLogin(result.getString(LOGIN));
		    	user.setPassword(result.getString(PASSWORD));
		    	user.setToken(result.getString(TOKEN));
		    }
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("selectUser", e.getCause());
		}
	    return user;
	}
	
}
