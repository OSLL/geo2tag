package ru.spb.osll.web.server.db;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;

import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.common.DBUtil;

public class Users {
	public static final String TABLE 	= "users";
	public static final String ID 		= "id";
	public static final String LOGIN 	= "login";
	public static final String PASSWORD	= "password";
	public static final String TOKEN 	= "token";
	
	public static boolean insertUser(User user){
		// TODO
		return true;
	}

	public static boolean updateUser(User user){
		// TODO
		return true;
	}

	
	private static final String SELECT_USER = "SELECT * FROM users WHERE login='%s';";
	
	public static User selectUser(String login){
		User user = null;
		try {
			final Connection c = DBUtil.getConnction();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(SELECT_USER, login);
			final ResultSet result = Ex1Stmt.executeQuery(query);
		    while (result.next()) {
		    	user = new User();	
		    	//user.setId(result.getInt(ID)); FIXME
		    	user.setLogin(result.getString(LOGIN));
		    	user.setPassword(result.getString(PASSWORD));
		    	user.setToken(result.getString(TOKEN));
		    }
		} catch (Exception e) {
			// TODO
		}
	    return user;
	}
	
}
