/*
 * Copyright 2011-2012 OSLL
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

package ru.spb.osll.web.client.ui.common;

import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.ui.core.IsDataAccessor;

public class Accessors {

	// CHANNEL'S ACCESSORS
	public final static IsDataAccessor<WChannel> CHANNEL_ACC_NAME = new IsDataAccessor<WChannel>(){
		public String extractData(WChannel ch) {
			return ch.getName();
		}
	};

	public final static IsDataAccessor<WChannel> CHANNEL_ACC_DESC = new IsDataAccessor<WChannel>(){
		public String extractData(WChannel ch) {
			return ch.getDescription();
		}
	};

	public final static IsDataAccessor<WChannel> CHANNEL_ACC_URL = new IsDataAccessor<WChannel>() {
		public String extractData(WChannel ch) {
			return ch.getUrl();
		}
	};

	// CHANNEL'S ACCESSORS
	public final static IsDataAccessor<WMark> TAG_ACC_LABEL = new IsDataAccessor<WMark>() {
		public String extractData(WMark t) {
			return t.getLabel();
		}
	};

	public final static IsDataAccessor<WMark> TAG_ACC_DESC = new IsDataAccessor<WMark>() {
		public String extractData(WMark t) {
			return t.getDescription();
		}
	};
	
	public final static IsDataAccessor<WMark> TAG_ACC_URL = new IsDataAccessor<WMark>() {
		public String extractData(WMark t) {
			return t.getUrl();
		}
	};
	
	public final static IsDataAccessor<WMark> TAG_ACC_TIME = new IsDataAccessor<WMark>() {
		public String extractData(WMark t) {
			return t.getTime().toGMTString(); 	// FIXME
		}
	};
	
	public final static IsDataAccessor<WMark> TAG_ACC_LAT = new IsDataAccessor<WMark>() {
		public String extractData(WMark t) {
			return t.getLatitude() + ""; 		// FIXME
		}
	};

	public final static IsDataAccessor<WMark> TAG_ACC_LON = new IsDataAccessor<WMark>() {
		public String extractData(WMark t) {
			return t.getLongitude() + ""; 		// FIXME
		}
	};
	
    public final static IsDataAccessor<Integer> INTEGRE_ACC = new IsDataAccessor<Integer>() {
        public String extractData(Integer value) {
            return value.toString();
        }
    };

}
