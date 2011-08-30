package ru.spb.osll.web.server.db;

import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

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
			Logger.getLogger(Users.class).error("baseInsert", e.getCause());
		}
		return id;
	}

	
	abstract protected T constructObject(ResultSet result) throws SQLException;
	
	protected T baseSingleSelect(final String query){
		T object = null;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	object = constructObject(result);
		    }
		} catch (Exception e) {
			Logger.getLogger(AbstractBase.class).error("baseSingleSelect", e.getCause());
		}
	    return object;
	}

	protected List<T> baseMultiSelect(final String query){
		List<T> list = new ArrayList<T>();
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	list.add(constructObject(result));
		    }
		} catch (Exception e) {
			Logger.getLogger(AbstractBase.class).error("baseMultiSelect", e.getCause());
		}
	    return list;
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

	protected boolean containQuery(final String query){
		boolean succes = false;
		try {
			final Statement statement = DBUtil.getConnection().createStatement();
			final ResultSet result = statement.executeQuery(query);
		    while (result.next()) {
		    	succes = true;
		    }
		} catch (Exception e) {
			Logger.getLogger(AbstractBase.class).error("containQuery", e.getCause());
		}
		return succes;
	}	

}
