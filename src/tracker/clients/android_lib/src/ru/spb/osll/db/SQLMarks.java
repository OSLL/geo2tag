package ru.spb.osll.db;

import java.util.LinkedList;
import java.util.List;

import ru.spb.osll.objects.Mark;
import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteStatement;

public class SQLMarks extends Marks {
	
	private String table		= "marks";

	private String id 			= "_id";
	private String title 		= "title";
	private String channel 		= "channel";
	private String link 		= "link";
	private String desc 		= "desc";
	private String time 		= "time";
	private String lat 			= "lat";
	private String lon 			= "lon";
	
	private final SQLiteDatabase m_database;
	private final SQLiteStatement m_eraseMarksStatement;
	private final SQLiteStatement m_removeSomeMarksStatement;
	
	public SQLMarks(Context context) {
		m_database = context.openOrCreateDatabase("geo2tag.db", Context.MODE_PRIVATE, null);
		if (m_database.getVersion() == 0) {
			String sql = "CREATE TABLE %s (" +
					"%s INTEGER PRIMARY KEY AUTOINCREMENT, " +
					"%s TEXT, %s TEXT, %s TEXT, %s TEXT, %s TEXT, " +
					"%s REAL, %s REAL)";
			m_database.execSQL(String.format(sql, table, id, channel, title, link, desc, time, lat, lon));
		}
		m_database.setVersion(1);
		m_eraseMarksStatement = m_database.compileStatement("DELETE FROM marks");
		m_removeSomeMarksStatement = m_database.compileStatement("DELETE FROM marks WHERE _id >= ? AND _id <= ?");
	}

	@Override
	synchronized public void addMark(Mark mark) {
		ContentValues val = new ContentValues();
		val.put(title, mark.getTitle());
		val.put(channel, mark.getChannel());
		val.put(link, mark.getLink());
		val.put(desc, mark.getDescription());
		val.put(time, mark.getTime());
		val.put(lat, mark.getLatitude());
		val.put(lon, mark.getLongitude());
		m_database.insert(table, title, val);
	}

	@Override
	synchronized public List<Mark> getMarks() {
		final List<Mark> list = new LinkedList<Mark>();
		final Cursor cursor = m_database.rawQuery("SELECT * FROM marks", null);
		while (cursor.moveToNext()) {
			Mark mark = new Mark();
			mark.setId(cursor.getLong(cursor.getColumnIndex(id)));
			mark.setTitle(cursor.getString(cursor.getColumnIndex(title)));
			mark.setChannel(cursor.getString(cursor.getColumnIndex(channel)));
			mark.setLink(cursor.getString(cursor.getColumnIndex(link)));
			mark.setDescription(cursor.getString(cursor.getColumnIndex(desc)));
			mark.setTime(cursor.getString(cursor.getColumnIndex(time)));
			mark.setLatitude(cursor.getDouble(cursor.getColumnIndex(lat)));
			mark.setLongitude(cursor.getDouble(cursor.getColumnIndex(lon)));
			list.add(mark);
		}
		cursor.close();
		return list;
	}

	@Override
	synchronized public void removeAllMarks() {
		m_eraseMarksStatement.execute();
	}

	@Override
	public void removeSomeMarks(long initId, long lastId) {
		m_removeSomeMarksStatement.bindLong(1, initId);
		m_removeSomeMarksStatement.bindLong(2, lastId);
		m_removeSomeMarksStatement.execute();
	}
}
