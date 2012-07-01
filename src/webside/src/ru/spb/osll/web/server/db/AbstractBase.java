/*
 * Copyright 2011-2012 OSLL
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

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
