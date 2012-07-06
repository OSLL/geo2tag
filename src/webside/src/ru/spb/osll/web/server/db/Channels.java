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
import java.util.List;

import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WUser;

public class Channels extends AbstractBase<WChannel> {
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
	
	
	public WChannel insert(WChannel ch){  
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

	public boolean delete(WChannel channel){
		final String deleteChannel = "DELETE FROM channel WHERE name='%s';";	
		final String query = String.format(deleteChannel, channel.getName());
		return baseBoolQuery(query);
	}	
	
	public boolean update(WChannel ch){
		final String updateChannel = "UPDATE channel SET name='%s', description='%s', url='%s' WHERE id='%s';";
		final String query = String.format(updateChannel, ch.getName(), ch.getDescription(), ch.getUrl(), ch.getId());
		return baseBoolQuery(query);
	}
	
	public List<WChannel> selectAll(){
		final String query = "SELECT * FROM channel;";
	    return baseMultiSelect(query);
	}

	public List<WChannel> selectByUser(WUser user){
		final String selectByUser = "SELECT * FROM channel INNER JOIN subscribe " +
				" ON subscribe.channel_id = channel.id WHERE subscribe.user_id = %s;";
		final String query = String.format(selectByUser, user.getId());
	    return baseMultiSelect(query);
	}

	@Deprecated
	public List<WChannel> selectUnuse(WUser user){
		// TODO 
		return null;
	}

	@Override
	protected WChannel constructObject(ResultSet result) throws SQLException {
    	WChannel channel = new WChannel();
    	channel.setId(result.getLong(ID)); 
    	channel.setName(result.getString(NAME));
    	channel.setDescription(result.getString(DESCRIPTION));
    	channel.setUrl(result.getString(URL));
		return channel;
	}	

}
