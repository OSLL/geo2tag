package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.TableWidget.IsTableAccessor;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

public class Channels extends SimpleComposite {

	TableWidget<Channel> m_table;
	
	@Override
	protected String getName() {
		return "Channels"; // TODO localize
	}

	@Override
	protected Widget onInitialize() {
		VerticalPanel layout = UIUtil.getVerticalPanel(true);
		
		List<TableField<Channel>> fields = new ArrayList<TableField<Channel>>();
		fields.add(FIELD_NAME);
		fields.add(FIELD_DESC);
		fields.add(FIELD_URL);
		
		m_table = new TableWidget<Channel>(fields);
		showChannels();
		
		layout.add(m_table);
		return layout;
	}
	
	private void showChannels(){
		// TODO channel service to get channels
	}
	
	private final static IsTableAccessor<Channel> ACC_NAME = new IsTableAccessor<Channel>() {
		public String toCell(Channel ch) {
			return ch.getName();
		}
	};

	private final static IsTableAccessor<Channel> ACC_DESC = new IsTableAccessor<Channel>() {
		public String toCell(Channel ch) {
			return ch.getDescription();
		}
	};

	private final static IsTableAccessor<Channel> ACC_URL = new IsTableAccessor<Channel>() {
		public String toCell(Channel ch) {
			return ch.getUrl();
		}
	};
	
	private final static TableField<Channel> FIELD_NAME = 
			new TableField<Channel>("field.name", "Name", ACC_NAME);

	private final static TableField<Channel> FIELD_DESC = 
			new TableField<Channel>("field.name", "Desc", ACC_DESC);

	private final static TableField<Channel> FIELD_URL = 
			new TableField<Channel>("field.name", "Url", ACC_URL);

}
