package ru.spb.osll.web.server.db;

import java.sql.Connection;
import java.sql.Statement;

import org.apache.log4j.Logger;

import ru.spb.osll.web.client.services.objects.CTag;
import ru.spb.osll.web.server.db.common.DBUtil;

public class CTags {
	private static final String INSERT_TAGS = "INSERT INTO tags VALUES (%s, %s);";
	public static CTag insert(CTag tag){  
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(INSERT_TAGS, tag.getChannelId() + "", tag.getTagId() + "");
			final int result = Ex1Stmt.executeUpdate(query);
		} catch (Exception e) {
		    Logger.getLogger(Channels.class).error("insertChannelTag", e.getCause());
		}
		return tag;
	}

	private static final String DELETE_TAG = "DELETE FROM tags WHERE channel_id=%s;";	
	public static boolean delete(CTag tag){
		boolean success = true; 
		try {
			final Connection c = DBUtil.getConnection();
			final Statement Ex1Stmt = c.createStatement();
			final String query = String.format(DELETE_TAG, tag.getChannelId());
			final int result = Ex1Stmt.executeUpdate(query);     
		} catch (Exception e) {
			Logger.getLogger(Channels.class).error("deleteChannelTag", e.getCause());
			success = false;
		}
		return success;
	}
}