package ru.spb.osll.objects;

import java.util.ArrayList;
import java.util.List;

public class Channel {
	private String name;
	private List<Mark> makrs = new ArrayList<Mark>();
	
	public Channel(String name) {
		this.name = name;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public void addMark(Mark mark) {
		makrs.add(mark); 
	}
	
	public void setMarks(List<Mark> marks) {
		this.makrs = marks;
	}
	
	public List<Mark> getMarks() {
		return makrs;
	}

}
