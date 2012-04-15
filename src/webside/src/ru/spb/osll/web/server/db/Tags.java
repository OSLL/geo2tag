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
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;

public class Tags extends AbstractBase<Tag> {
	public static final String TABLE 		= "tag";
	
	public static final String ID 			= "id";
	public static final String TIME 		= "time";
	public static final String LATITUDE 	= "latitude";
	public static final String LONGITUDE 	= "longitude";
	public static final String LABEL 		= "label";
	public static final String DESCRIPTION 	= "description";
	public static final String URL 			= "url";
	public static final String USERID   	= "user_id";

	public static Tags Instance(){
		if (instance == null){
			instance = new Tags();
		}
		return instance;
	}
	private static Tags instance;
	private Tags(){}
	
	
	public Tag insert(Tag tag){  
		final String insertTag = "INSERT INTO tag(latitude, longitude, label, description, user_id, url) " +
				"VALUES (%s, %s, '%s', '%s', '%s', '%s');";
		final String query = String.format(insertTag, tag.getLatitude(), tag.getLongitude(), 
				tag.getLabel(), tag.getDescription(), tag.getUserId(), tag.getUrl());
		final long id = baseInsert(query);
		if (-1 != id){
			tag.setId(id);
		} else {
			tag = null;
		}
		return tag;
	}
		
	public boolean delete(Tag tag){
		final String deleteTag = "DELETE FROM tag WHERE id=%s;"; 
		final String query = String.format(deleteTag, tag.getId());
		return baseBoolQuery(query);
	}	
	
	public boolean update(Tag tag){
		final String updateTag = "UPDATE tag SET latitude='%s', longitude='%s', " +
				"label='%s', description='%s', url='%s', user_id='%s' WHERE id='%s';";
		final String query = String.format(updateTag, tag.getLatitude(), tag.getLongitude(), 
				tag.getLabel(),	tag.getDescription(), tag.getUrl(), tag.getUserId(), tag.getId());
		return baseBoolQuery(query);
	}
	
	public List<Tag> selectByUser(User user){
		final String selectTagsByUser = "SELECT * FROM tag WHERE user_id=%s;";
		final String query = String.format(selectTagsByUser, user.getId());
	    return baseMultiSelect(query);
	}

	public List<Tag> selectByUser(User user, Date dateFrom, Date dateTo){
		final String timeFromCondition = getTimeFromCondition(dateFrom);
		final String timeToCondition = getTimeToCondition(dateTo);
		final String selectTagsByUser = "SELECT * FROM tag WHERE user_id=%s " + 
			timeFromCondition + timeToCondition + ";";
		final String query = String.format(selectTagsByUser, user.getId());
	    return baseMultiSelect(query);
	}
	
	public List<Tag> selectByChannel(Channel channel){
		final String selectTagsByChannel = "SELECT time, id, latitude, longitude, label, description, url, user_id" +
			" FROM tag INNER JOIN tags ON tag.id = tags.tag_id WHERE tags.channel_id = '%s';";
		final String query = String.format(selectTagsByChannel, channel.getId());
	    return baseMultiSelect(query);
	}	

	public List<Tag> selectByChannel(Channel channel, Date dateFrom, Date dateTo){
		final String timeFromCondition = getTimeFromCondition(dateFrom);
		final String timeToCondition = getTimeToCondition(dateTo);
		final String selectTagsByChannel = "SELECT time, id, latitude, longitude, label, description, url, user_id" +
			" FROM tag INNER JOIN tags ON tag.id = tags.tag_id WHERE tags.channel_id = '%s'" + 
			timeFromCondition + timeToCondition + ";";
		final String query = String.format(selectTagsByChannel, channel.getId());
	    return baseMultiSelect(query);
	}	

	// FIXME later
	public List<Tag> selectByChannels(List<Channel> channels, Date dateFrom, Date dateTo){
		List<Tag> result = new ArrayList<Tag>();
		for (Channel ch : channels){
			result.addAll(selectByChannel(ch, dateFrom, dateTo));
		}
		return result;
	}
	
	private String getTimeFromCondition(Date timeFrom){
		if (timeFrom == null){
			return "";
		}
		final String condition = " AND time >= '%s' ";
		return String.format(condition, new Timestamp(timeFrom.getTime()));
	}

	private String getTimeToCondition(Date timeTo){
		if (timeTo == null){
			return "";
		}
		final String condition = " AND time <= '%s' ";
		return String.format(condition, new Timestamp(timeTo.getTime()));
	}
	
	public boolean addTagToChannel(Channel ch, Tag tag){
		final String insertTagChannel = "INSERT INTO tags(channel_id, tag_id) VALUES ('%s', '%s');";
		final String query = String.format(insertTagChannel, ch.getId() , tag.getId());
		return baseBoolQuery(query);
	}

	public boolean removeTagFromChannel(Channel ch, Tag tag){
		final String deleteTagChannel = "DELETE FROM tags WHERE channel_id='%s' AND tag_id='%s';";	
		final String query = String.format(deleteTagChannel, ch.getId(), tag.getId());
		return baseBoolQuery(query);
	}

	@Override
	protected Tag constructObject(ResultSet result) throws SQLException {
		Tag tag = new Tag();
    	tag.setId(result.getInt(ID)); 
    	tag.setTime(result.getTimestamp(TIME));
    	tag.setLatitude(result.getFloat(LATITUDE));
    	tag.setLongitude(result.getFloat(LONGITUDE));
    	tag.setLabel(result.getString(LABEL));
    	tag.setDescription(result.getString(DESCRIPTION));
    	tag.setUrl(result.getString(URL));
    	tag.setUserId(result.getInt(USERID));
		return tag;
	}
	
}

