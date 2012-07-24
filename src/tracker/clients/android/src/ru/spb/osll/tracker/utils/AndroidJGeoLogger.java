/*
 * Copyright 2012  Vasily Romanikhin  vasily.romanikhin@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.tracker.utils;

import android.util.Log;
import ru.spb.osll.log.Logger;

public class AndroidJGeoLogger implements Logger {

	@Override
	public void println(int level, String tag, int v) {
	    log(level, tag, v);
	}

	@Override
	public void println(int level, String tag, double v) {
	    log(level, tag, v);
	}


	@Override
	public void println(int level, String tag, float v) {
	    log(level, tag, v);
	}


	@Override
	public void println(int level, String tag, byte v) {
	    log(level, tag, v);
	}


	@Override
	public void println(int level, String tag, boolean v) {
	    log(level, tag, v);
	}


	@Override
	public void println(int level, String tag, String v) {
	    log(level, tag, v);
	}


	@Override
	public void println(int level, String tag, Throwable t) {
		t.printStackTrace();
		Log.e(tag, "Throwable:", t);
	}
	
	private void log(int level, String tag, Object msg){
		if (level == Logger.DEBUG) {
		    Log.d(tag, msg.toString());
		} else {
		    Log.e(tag, msg.toString());
		}
	}

}