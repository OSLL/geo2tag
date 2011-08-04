package ru.spb.osll.web.client.ui.core;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HasHorizontalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.client.ui.FlexTable.FlexCellFormatter;

public abstract class FieldsWidget extends SimpleComposite {
	private Label m_messageLabel;
	
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
	
	protected List<Anchor> getLinks(){
		return new ArrayList<Anchor>();
	}
	
	
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

		// FIELDS
		final List<TField> fields = getFields();
		for (int i = 0; i < fields.size(); i++){
			final TField field = fields.get(i);
			fieldsContainer.setHTML(i + 1, 0, field.name);
			fieldsContainer.setWidget(i + 1, 1, field.widget);
		}

		// MESSAGE
		HorizontalPanel messageContainer = UIUtil.getHorizontalPanel(true);
		m_messageLabel = new Label();
		m_messageLabel.setVisible(false);
		m_messageLabel.setStyleName("warning-label-style");
		messageContainer.add(m_messageLabel);

		// LINKS
		HorizontalPanel linksContainer = UIUtil.getHorizontalPanel(true);
		for (Anchor a : getLinks()) {
			linksContainer.add(a);
		}
		
		// BUTTONS
		HorizontalPanel buttonsContainer = UIUtil.getHorizontalPanel(true);
		for (Button button : getButtons()){
			buttonsContainer.add(button);
		}
		
		VerticalPanel verticalPanel = UIUtil.getVerticalPanel();
		verticalPanel.add(fieldsContainer);
		verticalPanel.add(messageContainer);
		verticalPanel.add(linksContainer);
		verticalPanel.add(buttonsContainer);

		DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidget(verticalPanel);

		VerticalPanel layout = UIUtil.getVerticalPanel(alignOnCenter());
		layout.add(decPanel);
		return layout;
	}
	
	public void showMessage(String message){
		m_messageLabel.setText(message);
		m_messageLabel.setVisible(true);
	}

	public void hideMessage(){
		m_messageLabel.setVisible(false);
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
