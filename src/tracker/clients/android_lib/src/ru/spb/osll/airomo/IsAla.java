package ru.spb.osll.airomo;

import java.util.List;

import ru.spb.osll.error.AlaError;
import ru.spb.osll.objects.Mark;


public interface IsAla {
	String auth(String user, String pass);
	
	void startTrack();

	void stopTrack();

	boolean isTracking();

	// preferences
	void setUser(String user);
	String getUser();
	
	void  setPass(String pass);
	String getPass();

	void setTrackInterval(int sec);
	int getTrackInterval();

	void setHistoryLimit(int maxMarks);
	int getHistoryLimit();
	
	void setServerUrl(String severUrl);
	String getServerUrl();

	void setChannel(String channel);
	String getChannel();
	
	void sendHistory();

	void sendLastCoordinate();

	List<Mark> getAllMarks();  

	void addTrackListener(TrackListener l);	
	public interface TrackListener {
		void onNewMark(Mark mark);
	}
	
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

