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

import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;

public class Fields {

	// CHANNEL'S FIELDS
	// TODO Localize
	public final static TableField<WChannel> CHANNEL_FIELD_NAME = 
			new TableField<WChannel>("field.name", "Name", Accessors.CHANNEL_ACC_NAME);

	public final static TableField<WChannel> CHANNEL_FIELD_DESC = 
			new TableField<WChannel>("field.name", "Desc", Accessors.CHANNEL_ACC_DESC);

	public final static TableField<WChannel> CHANNEL_FIELD_URL = 
			new TableField<WChannel>("field.name", "Url", Accessors.CHANNEL_ACC_URL);

	
	// TAG'S FIELDS
	// TODO Localize
	public final static TableField<WMark> TAG_FIELD_LABEL = 
			new TableField<WMark>("field.label", "Label", Accessors.TAG_ACC_LABEL);

	public final static TableField<WMark> TAG_FIELD_DESC = 
			new TableField<WMark>("field.desc", "Description", Accessors.TAG_ACC_DESC);

	public final static TableField<WMark> TAG_FIELD_URL = 
			new TableField<WMark>("field.url", "Url", Accessors.TAG_ACC_URL);

	public final static TableField<WMark> TAG_FIELD_TIME = 
			new TableField<WMark>("field.time", "Time", Accessors.TAG_ACC_TIME);

	public final static TableField<WMark> TAG_FIELD_LAT = 
			new TableField<WMark>("field.lat", "Latitude", Accessors.TAG_ACC_LAT);

	public final static TableField<WMark> TAG_FIELD_LON = 
			new TableField<WMark>("field.lon", "Longitude", Accessors.TAG_ACC_LON);

}
