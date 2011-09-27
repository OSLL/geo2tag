package ru.spb.osll.ala;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public abstract class AlaReceiver extends BroadcastReceiver {
	public static final String 	ACTION_ALA 	= "airomo.ala.action";

	public static final String TYPE_OPERATION 	= "airomo.type";
	public static final int TYPE_ERROR 			= 0;
	public static final int TYPE_MARK 			= 1;
	
	public static final String 	ERROR 			= "type.error";
	public static final String 	LONLAT 			= "type.lonlat";
	
	@Override
	public void onReceive(Context context, Intent intent) {
		int type = intent.getIntExtra(TYPE_OPERATION, -1);
		switch (type) {
		case TYPE_ERROR:
			final String error = intent.getStringExtra(ERROR);
			onErrorOccured(error);
			break;
		case TYPE_MARK:
			final String lonlat = intent.getStringExtra(LONLAT);
			onNewMark(lonlat);
			break;
		}
	}

	public abstract void onErrorOccured(String error);
	
	public abstract void onNewMark(String lonlat);
	
}
