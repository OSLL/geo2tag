package ru.spb.osll.airomo;

import java.util.List;

import ru.spb.osll.error.AlaError;
import ru.spb.osll.objects.Mark;


public interface IsAla {
	void setUserData(String user, String pass);

	String auth(String user, String pass);
	
	void startTrack();

	void stopTrack();

	boolean isTracking();

	void setTrackInterval(int sec);

	int getTrackInterval();

	// default 50, maximum 200;
	void setHistoryLimit(int maxMarks);
	
	int getHistoryLimit();

	void sendHistory();

	void sendLastCoordinate();

	List<Mark> getAllMarks();  
	
	void addGooffListener(GooffListener l);	
	public interface GooffListener {
		void gooff();
	}

	void addOnNetworkListener(NetworkListener l);	
	public interface NetworkListener {
		void networkChanged(boolean isOnline); 
	}

	void addErrorListener(ErrorListener l);	
	public interface ErrorListener {
		void onError(AlaError error); 
	}
	
	void onErrorOccured(String error);

	String getLastError();

	boolean isOnline();
	// TODO GPS is’t ready
}

