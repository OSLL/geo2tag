package ru.spb.osll.web.client.ui.widgets;

import ru.spb.osll.web.client.ui.widgets.BaseTagsWidget.TagsView;

import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Widget;

public abstract class BaseMapWidget extends Composite implements TagsView {
	public BaseMapWidget() {
		initWidget(onInitializeMap());
	}

	protected abstract Widget onInitializeMap();
}
