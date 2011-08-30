package ru.spb.osll.web.client.ui.common;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.ui.core.IsDataAccessor;

public class Accessors {

	// CHANNEL'S ACCESSORS
	public final static IsDataAccessor<Channel> CHANNEL_ACC_NAME = new IsDataAccessor<Channel>(){
		public String extractData(Channel ch) {
			return ch.getName();
		}
	};

	public final static IsDataAccessor<Channel> CHANNEL_ACC_DESC = new IsDataAccessor<Channel>(){
		public String extractData(Channel ch) {
			return ch.getDescription();
		}
	};

	public final static IsDataAccessor<Channel> CHANNEL_ACC_URL = new IsDataAccessor<Channel>() {
		public String extractData(Channel ch) {
			return ch.getUrl();
		}
	};

	// CHANNEL'S ACCESSORS
	public final static IsDataAccessor<Tag> TAG_ACC_LABEL = new IsDataAccessor<Tag>() {
		public String extractData(Tag t) {
			return t.getLabel();
		}
	};

	public final static IsDataAccessor<Tag> TAG_ACC_DESC = new IsDataAccessor<Tag>() {
		public String extractData(Tag t) {
			return t.getDescription();
		}
	};
	
	public final static IsDataAccessor<Tag> TAG_ACC_URL = new IsDataAccessor<Tag>() {
		public String extractData(Tag t) {
			return t.getUrl();
		}
	};
	
	public final static IsDataAccessor<Tag> TAG_ACC_TIME = new IsDataAccessor<Tag>() {
		public String extractData(Tag t) {
			return t.getTime().toGMTString(); 	// FIXME
		}
	};
	
	public final static IsDataAccessor<Tag> TAG_ACC_LAT = new IsDataAccessor<Tag>() {
		public String extractData(Tag t) {
			return t.getLatitude() + ""; 		// FIXME
		}
	};

	public final static IsDataAccessor<Tag> TAG_ACC_LON = new IsDataAccessor<Tag>() {
		public String extractData(Tag t) {
			return t.getLongitude() + ""; 		// FIXME
		}
	};

}
