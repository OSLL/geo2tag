package ru.spb.osll.airomo;

import android.util.Log;

abstract class BaseAla implements IsAla {
	public static final String ALA_LOG = "AlaLog";
	
	public BaseAla() {
		Log.v(ALA_LOG, "BaseAla was created...");
	}

}
