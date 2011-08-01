package ru.spb.osll.web.client.ui.widgets;

import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.image.ResImages;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.uibinder.client.UiHandler;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.Widget;

public class WidgetByDesigner extends SimpleComposite {

	private static W1UiBinder uiBinder = GWT.create(W1UiBinder.class);

	interface W1UiBinder extends UiBinder<Widget, WidgetByDesigner> {
	}

	interface Images extends ResImages{
	}
	
	@UiField Button button;

	public WidgetByDesigner() {
		super();
		button.setText("Push me!!!");
	}

	@UiHandler("button")
	void onClick(ClickEvent e) {
		Window.alert("Hello!");
	}

	@Override
	protected Widget onInitialize() {
		return uiBinder.createAndBindUi(this);
	}

	@Override
	public String getName() {
		return "Widget by GWT Designer";
	}
}
