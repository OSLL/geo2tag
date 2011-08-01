package ru.spb.osll.web.client.ui.core;

import java.util.List;

import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HasHorizontalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.client.ui.FlexTable.FlexCellFormatter;

public abstract class FieldsWidget extends SimpleComposite {

	public FieldsWidget(){
		super();
	}
	
	protected boolean alignOnCenter(){
		return true;
	}
	
	protected String getTitleString(){
		return null;
	}
	
	protected abstract List<TField> getFields();
	protected abstract List<Button> getButtons();
	
	@Override
	protected Widget onInitialize() {
		FlexTable fieldsContainer = new FlexTable();
		fieldsContainer.setCellSpacing(8);
		FlexCellFormatter cellFormatter = fieldsContainer.getFlexCellFormatter();

		// Add a title to the form
		final String title = getTitleString();
		if (title != null){
			fieldsContainer.setHTML(0, 0, title);
		}
		cellFormatter.setColSpan(0, 0, 2);
		cellFormatter.setHorizontalAlignment(0, 0, HasHorizontalAlignment.ALIGN_CENTER);

		final List<TField> fields = getFields();
		for (int i = 0; i < fields.size(); i++){
			final TField field = fields.get(i);
			fieldsContainer.setHTML(i + 1, 0, field.name);
			fieldsContainer.setWidget(i + 1, 1, field.widget);
		}

		HorizontalPanel buttonsContainer = UIUtil.getHorizontalPanel(true);
		for (Button button : getButtons()){
			buttonsContainer.add(button);
		}

		VerticalPanel verticalPanel = UIUtil.getVerticalPanel();
		verticalPanel.add(fieldsContainer);
		verticalPanel.add(buttonsContainer);

		DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidget(verticalPanel);

		VerticalPanel layout = UIUtil.getVerticalPanel(alignOnCenter());
		layout.add(decPanel);
		return layout;
	}
	
	public static class TField {
		public final String name;
		public final Widget widget;
		public TField(String name, Widget widget){
			this.name = name;
			this.widget = widget;
		}
	}
}
