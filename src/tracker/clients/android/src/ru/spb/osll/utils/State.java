package ru.spb.osll.utils;

import java.util.HashMap;

public class State {
	private HashMap<String, Object> myState = new HashMap<String, Object>();
	private static State Instance;

	public static State instance(){
		if (Instance == null)
			Instance = new State();
		return Instance;
	}
	
	public synchronized void putToState(String key, Object object){
		myState.put(key, object);
	}

	public synchronized Object getFromState(String key){
		return myState.get(key);
	}

	public synchronized Object popFromState(String key){
		Object object = myState.get(key);
		myState.remove(key);
		return object;
	}

	public synchronized void clearState(String key){
		myState.clear();
	}
}
