package ru.spb.osll.web.client.ui.widgets;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.CheckBox;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.GTState.ChannelStateListener;
import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.tags.TagService;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
import ru.spb.osll.web.client.ui.core.TableWidget.IsTableAccessor;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;

public class TagsTableWidget extends SimpleComposite
	implements ChannelStateListener {
	
	private TableWidget<Tag> m_tagsTable;
	
	@Override
	protected String getName() {
		return "Tags table";
	}

	@Override
	protected Widget onInitialize() {
		GTState.Instanse().addOnChannelStateListerer(this);

		VerticalPanel contaier = UIUtil.getVerticalPanel();
		contaier.setSpacing(10);
		contaier.setWidth("100%");
		initFiltersPanel(contaier);
		
		List<TableField<Tag>> fields = new ArrayList<TableField<Tag>>();
		fields.add(FIELD_LABEL);
		fields.add(FIELD_DESC);
		fields.add(FIELD_URL);
		fields.add(FIELD_TIME);
		fields.add(FIELD_LAT);
		fields.add(FIELD_LON);
		m_tagsTable = new TableWidget<Tag>(fields);
		
		contaier.add(m_tagsTable);
		loadTags();
		return contaier;
	}

	private void initFiltersPanel(VerticalPanel container){
//		CheckBox 
		//TODO
		Date date = new Date();
		Timestamp timestamp = new Timestamp(date.getTime());
	}
	
	
	@Override
	public void onChannelChange(Channel ch){
		m_tagsTable.erase();
		loadTags(ch);
	}

	private void loadTags(){
		final Channel ch = GTState.Instanse().getCurChannel();
		if (ch == null){
			return;
		}
		loadTags(ch);
	}
	
	private void loadTags(Channel ch){
		TagService.Util.getInstance().getTags(ch, new AsyncCallback<List<Tag>>() {
			@Override
			public void onSuccess(List<Tag> result) {
				m_tagsTable.addRows(result);
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("loadTags", caught);
			}
		});
	}
	
	private final static IsTableAccessor<Tag> ACC_LABEL = new IsTableAccessor<Tag>() {
		public String toCell(Tag t) {
			return t.getLabel();
		}
	};

	private final static IsTableAccessor<Tag> ACC_DESC = new IsTableAccessor<Tag>() {
		public String toCell(Tag t) {
			return t.getDescription();
		}
	};
	
	private final static IsTableAccessor<Tag> ACC_URL = new IsTableAccessor<Tag>() {
		public String toCell(Tag t) {
			return t.getUrl();
		}
	};
	
	private final static IsTableAccessor<Tag> ACC_TIME = new IsTableAccessor<Tag>() {
		public String toCell(Tag t) {
			return t.getTime().toGMTString(); 	// FIXME
		}
	};
	
	private final static IsTableAccessor<Tag> ACC_LAT = new IsTableAccessor<Tag>() {
		public String toCell(Tag t) {
			return t.getLatitude() + ""; 		// FIXME
		}
	};

	private final static IsTableAccessor<Tag> ACC_LON = new IsTableAccessor<Tag>() {
		public String toCell(Tag t) {
			return t.getLongitude() + ""; 		// FIXME
		}
	};
	
	// TODO Localize
	private final static TableField<Tag> FIELD_LABEL = 
			new TableField<Tag>("field.label", "Label", ACC_LABEL);

	private final static TableField<Tag> FIELD_DESC = 
			new TableField<Tag>("field.desc", "Description", ACC_DESC);

	private final static TableField<Tag> FIELD_URL = 
			new TableField<Tag>("field.url", "Url", ACC_URL);

	private final static TableField<Tag> FIELD_TIME = 
			new TableField<Tag>("field.time", "Time", ACC_TIME);

	private final static TableField<Tag> FIELD_LAT = 
			new TableField<Tag>("field.lat", "Latitude", ACC_LAT);

	private final static TableField<Tag> FIELD_LON = 
			new TableField<Tag>("field.lon", "Longitude", ACC_LON);
}
