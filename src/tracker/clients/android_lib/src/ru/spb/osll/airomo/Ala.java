package ru.spb.osll.airomo;

import ru.spb.osll.airomo.BaseAlaService.InternalReceiver;
import ru.spb.osll.utils.TrackerUtil;
import android.content.Context;
import android.content.ContextWrapper;
import android.content.Intent;

public class Ala implements IsAla {
	public static final String ALA_LOG = "AlaLog";
	
	@Override
	public void startTrack(Context c) {
		c.startService(new Intent(c, AlaService.class));
	}

	@Override
	public void stopTrack(Context c) {
		c.stopService(new Intent(c, AlaService.class));
	}

	@Override
	public boolean isTracking(Context c) {
		return AlaService.isTracking();
	}

	@Override
	public void sendHistory(ContextWrapper c) {
		Intent intent = new Intent(InternalReceiver.ACTION);
		intent.putExtra(InternalReceiver.TYPE_SIGNAL, InternalReceiver.SIGNAL_SEND_HISTORY);
		c.sendBroadcast(intent);
	}

	@Override
	public void sendLastCoordinate(ContextWrapper c) {
		Intent intent = new Intent(InternalReceiver.ACTION);
		intent.putExtra(InternalReceiver.TYPE_SIGNAL, InternalReceiver.SIGNAL_SEND_COORDINATE);
		c.sendBroadcast(intent);
	}

	@Override
	public IsAlaSettings getAlaSettings(Context c){
		return new AlaSettings(c);
	}
	
	@Override
	public boolean isOnline(Context c) {
		return TrackerUtil.isOnline(c);
	}
	
}
