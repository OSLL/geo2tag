package ru.spb.osll.airomo;


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

//	List<Mark> getAllMarks(); // TODO 
	
	// TODO move listeners 
	// on closed || on low battery
	void addGooffListener(GooffListener l);	
	public interface GooffListener {
		void gooff();
	}

	void addOnNetworkListener(NetworkListener l);	
	public interface NetworkListener {
		void networkChanged(boolean isOnline); 
	}

	//signal:
	void onErrorOccured(String error);

	String getLastError();

	// if NetworkListener is private 
	boolean isOnline();
	// TODO GPS is’t ready
}

