package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.ui.common.Fields;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;

import com.google.gwt.user.client.ui.Composite;

public class TagsTableWidget extends BaseTagsWidget {
	@Override
	protected String getName() {
		return LOC.TagsTableWidget();
	}

	@Override
	protected TagsView getTagsView() {
		return new TagsTablePanel();
	};
	
	public static TagsTableWidget Instance(){
		if(instance == null){
			instance = new TagsTableWidget();
		}
		instance.resume();
		return instance;
	}
	private static TagsTableWidget instance;
	private TagsTableWidget(){
		super();
	}

	private class TagsTablePanel extends Composite implements TagsView {
		private TableWidget<Tag> m_tagsTable;
		
		public TagsTablePanel(){
			List<TableField<Tag>> fields = new ArrayList<TableField<Tag>>();
			fields.add(Fields.TAG_FIELD_LABEL);
			fields.add(Fields.TAG_FIELD_DESC);
			fields.add(Fields.TAG_FIELD_URL);
			fields.add(Fields.TAG_FIELD_TIME);
			fields.add(Fields.TAG_FIELD_LAT);
			fields.add(Fields.TAG_FIELD_LON);
			m_tagsTable = new TableWidget<Tag>(fields);
			
			initWidget(m_tagsTable);
		}
		
		@Override
		public void setTags(List<Tag> tags) {
			m_tagsTable.erase();
			m_tagsTable.addRows(tags);
		}

		@Override
		public void refresh(){
			m_tagsTable.erase();
		}
	}
}
