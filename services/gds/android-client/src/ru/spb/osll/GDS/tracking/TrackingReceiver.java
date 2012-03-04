package ru.spb.osll.GDS.tracking;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public abstract class TrackingReceiver extends BroadcastReceiver {
	
	public static final String 	ACTION_TRACKING 	= "osll.gds.tracking";

	public static final String TYPE_OPERATION 	= "gds.type";
	public static final int TYPE_ERROR 			= 0;
	public static final int TYPE_MARK_SENT		= 1;

	public static final String 	ERROR 			= "type.error";
	public static final String 	LONLAT 			= "type.lonlat";

	@Override
	public void onReceive(Context context, Intent intent) {
		int type = intent.getIntExtra(TYPE_OPERATION, -1);
		switch (type) {
		case TYPE_ERROR:
			onErrorOccured(intent.getStringExtra(ERROR));
			break;
		case TYPE_MARK_SENT:
			onMarkSent(intent.getStringExtra(LONLAT));
			break;
		}
	}

	public abstract void onErrorOccured(String error);
	
	public abstract void onMarkSent(String lonlat);

}
