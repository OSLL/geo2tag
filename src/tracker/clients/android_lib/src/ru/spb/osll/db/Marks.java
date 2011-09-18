package ru.spb.osll.db;

import java.util.List;

import ru.spb.osll.objects.Mark;

public abstract class Marks {
	public static Marks Instance() {
		return ourInstance;
	}

	private static Marks ourInstance;
	
	protected Marks() {
		ourInstance = this;
	}
	
	public abstract void addMark(Mark mark);

	public abstract List<Mark> getMarks();
	
	public abstract void removeAllMarks();

	public abstract void removeSomeMarks(long startId, long lastId);

}
