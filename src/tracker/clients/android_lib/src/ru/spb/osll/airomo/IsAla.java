package ru.spb.osll.airomo;

import android.content.Context;
import android.content.ContextWrapper;

public interface IsAla {
	
	void startTrack(Context c);

	void stopTrack(Context c);

	boolean isTracking(Context c);

	void sendHistory(ContextWrapper c);

	void sendLastCoordinate(ContextWrapper c);

	IsAlaSettings getAlaSettings(Context c);

	//List<Mark> getAllMarks();	// TODO  

	//String getLastError();	// TODO

	boolean isOnline(Context c);
	// TODO GPS is’t ready
}

