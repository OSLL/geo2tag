package ru.spb.osll.web.client;

import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.TableCellElement;
import com.google.gwt.dom.client.TableElement;
import com.google.gwt.event.shared.HandlerRegistration;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.ListBox;
import com.google.gwt.user.client.ui.SimplePanel;
import com.google.gwt.user.client.ui.Widget;

public class GTShell extends Composite {

	private static GTShellUiBinder uiBinder = GWT.create(GTShellUiBinder.class);

	interface GTShellUiBinder extends UiBinder<Widget, GTShell> {
	}

	@UiField SimplePanel contentPanel;

	@UiField TableElement linkCell;

	@UiField ListBox localeBox;

	@UiField TableCellElement localeSelectionCell;

	@UiField SimplePanel mainMenuContainer;

	private Widget content;

	private HandlerRegistration contentSourceHandler;

	public static GTShell Instance;
	
	public GTShell() {
		initWidget(uiBinder.createAndBindUi(this));
		mainMenuContainer.add(new GTMenu());
		Instance = this;
	}
	
	public void setContent(final Widget content) {
		// Clear the old handler.
		if (contentSourceHandler != null) {
			contentSourceHandler.removeHandler();
			contentSourceHandler = null;
		}

		this.content = content;
		if (content == null) {
			contentPanel.setWidget(null);
			return;
		}

		// Show the widget.
		showWidget();
	}
	
	public Widget getContent(){
		return content;
	}

	public void setDefaultContent(){
		setContent(null);	// TODO
	}

	private void showWidget() {
		if (content == null) {
			return;
		}
		contentPanel.setWidget(content);
	}
	
}
