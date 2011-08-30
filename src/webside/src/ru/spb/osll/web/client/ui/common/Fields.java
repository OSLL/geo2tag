package ru.spb.osll.web.client.ui.common;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;

public class Fields {

	// CHANNEL'S FIELDS
	// TODO Localize
	public final static TableField<Channel> CHANNEL_FIELD_NAME = 
			new TableField<Channel>("field.name", "Name", Accessors.CHANNEL_ACC_NAME);

	public final static TableField<Channel> CHANNEL_FIELD_DESC = 
			new TableField<Channel>("field.name", "Desc", Accessors.CHANNEL_ACC_DESC);

	public final static TableField<Channel> CHANNEL_FIELD_URL = 
			new TableField<Channel>("field.name", "Url", Accessors.CHANNEL_ACC_URL);

	
	// TAG'S FIELDS
	// TODO Localize
	public final static TableField<Tag> TAG_FIELD_LABEL = 
			new TableField<Tag>("field.label", "Label", Accessors.TAG_ACC_LABEL);

	public final static TableField<Tag> TAG_FIELD_DESC = 
			new TableField<Tag>("field.desc", "Description", Accessors.TAG_ACC_DESC);

	public final static TableField<Tag> TAG_FIELD_URL = 
			new TableField<Tag>("field.url", "Url", Accessors.TAG_ACC_URL);

	public final static TableField<Tag> TAG_FIELD_TIME = 
			new TableField<Tag>("field.time", "Time", Accessors.TAG_ACC_TIME);

	public final static TableField<Tag> TAG_FIELD_LAT = 
			new TableField<Tag>("field.lat", "Latitude", Accessors.TAG_ACC_LAT);

	public final static TableField<Tag> TAG_FIELD_LON = 
			new TableField<Tag>("field.lon", "Longitude", Accessors.TAG_ACC_LON);

}
