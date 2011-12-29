/*
 * Copyright 2010-2011  Vasily Romanikhin  bac1ca89@gmail.com
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

package ru.spb.osll.json;

import org.json.JSONException;
import org.json.JSONObject;

import android.util.Log;

public class JsonOldResponse extends JsonBaseResponse {

	// status
	public static String STATUS_OK 			= "Ok";
	public static String STATUS_ERR 		= "Error";
	
	// status messages
	public static String SM_USER_EXTSTS 	= "Username already exists!";
	public static String SM_CHANNEL_EXTSTS 	= "Channel exists!";
	public static String SM_CHANNEL_ADDED 	= "Channel added";
	
	
	private String m_status;
	private String m_statusMessage;
	
	@Override
	public void parseJson(JSONObject obj) {
    	try {
    		m_status = obj.getString("status");
    		m_statusMessage = obj.getString("status_description");
		} catch (JSONException e) {
			Log.e(LOG, e.getMessage());
		}
	}

	public String getStatus() {
		return m_status;
	}

	public String getStatusMessage() {
		return m_statusMessage;
	}
	
}
