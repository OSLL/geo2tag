package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import org.apache.log4j.Logger;

import ru.spb.osll.web.server.db.common.DBUtil;

abstract class AbstractBase <T> {
	
	protected long baseInsert(final String query){
		long id = -1;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			statement.execute(query, Statement.RETURN_GENERATED_KEYS); 
			final ResultSet rs = statement.getGeneratedKeys();
			while(rs.next()){
				id = rs.getLong(1); 
			}
		} catch (Exception e) {
			Logger.getLogger(Users.class).error("insertUser", e.getCause());
		}
		return id;
	}

	
	abstract protected T constructObject(ResultSet result) throws SQLException;
	
	protected T baseSelect(final String query){
		T object = null;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	object = constructObject(result);
		    }
		} catch (Exception e) {
			Logger.getLogger(AbstractBase.class).error("select", e.getCause());
		}
	    return object;
	}
	
	
	protected boolean baseBoolQuery(final String query){
		boolean succes = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			statement.execute(query);
			succes = true;
		} catch (Exception e) {
			Logger.getLogger(AbstractBase.class).error("boolQuery", e.getCause());
		}
		return succes;
	}	
	
}
