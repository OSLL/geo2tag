/*
 * Copyright 2012 OSLL
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

package ru.spb.osll.web.server;

import org.apache.log4j.Logger;

public class WebLogger implements ru.spb.osll.log.Logger {

	@Override
	public void println(int level, String tag, int v) {
		toLog(level, tag + ":" + v);
	}

	@Override
	public void println(int level, String tag, double v) {
		toLog(level, tag + ":" + v);
	}

	@Override
	public void println(int level, String tag, float v) {
		toLog(level, tag + ":" + v);
	}

	@Override
	public void println(int level, String tag, byte v) {
		toLog(level, tag + ":" + v);
	}

	@Override
	public void println(int level, String tag, boolean v) {
		toLog(level, tag + ":" + v);
	}

	@Override
	public void println(int level, String tag, String v) {
		toLog(level, tag + ":" + v);
	}

	@Override
	public void println(int level, String tag, Throwable t) {
		toLog(level, tag, t);
	}
	
	private void toLog(int level, Object message) {
		if (level == ru.spb.osll.log.Logger.DEBUG) {
			Logger.getLogger(getClass()).debug(message);
		} else {
			Logger.getLogger(getClass()).error(message);
		}
	}
	
	private void toLog(int level, String tag, Throwable t) {
		if (level == ru.spb.osll.log.Logger.DEBUG) {
			Logger.getLogger(getClass()).debug(tag, t);
		} else {
			Logger.getLogger(getClass()).error(tag, t);
		}
	}
}
