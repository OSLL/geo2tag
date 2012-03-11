package ru.spb.osll.GDS.events;

import ru.spb.osll.objects.Mark;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Parcelable;

public abstract class EventsReceiver extends BroadcastReceiver {
	
	public static final String 	ACTION_EVENTS 	= "osll.gds.events";

	public static final String TYPE_OPERATION 	= "gds.type";
	public static final int TYPE_ERROR 			= 0;
	public static final int TYPE_EVENTS			= 1;

	public static final String 	ERROR 			= "type.error";
	public static final String  EVENTS 			= "type.events";
	public static final String 	LONLAT 			= "type.lonlat";

	@Override
	public void onReceive(Context context, Intent intent) {
		int type = intent.getIntExtra(TYPE_OPERATION, -1);
		switch (type) {
		case TYPE_ERROR:
			onErrorOccured(intent.getStringExtra(ERROR));
			break;
		case TYPE_EVENTS:
			Parcelable[] result = intent.getParcelableArrayExtra(EVENTS);
			Mark[] marks = new Mark[result.length];
			for (int i = 0; i < result.length; ++i) {
				marks[i] = (Mark) result[i];
			}
			onEvents(marks);
			break;
		}
	}

	public abstract void onErrorOccured(String error);
	
	public abstract void onEvents(Mark[] mark);

}
