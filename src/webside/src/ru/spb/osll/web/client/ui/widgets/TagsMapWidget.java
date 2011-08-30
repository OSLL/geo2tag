package ru.spb.osll.web.client.ui.widgets;

import com.google.gwt.user.client.ui.Widget;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.UnderConstructionWidget;

public class TagsMapWidget extends SimpleComposite {

	@Override
	protected String getName() {
		return Localizer.res().TagsMapWidget();
	}

	@Override
	protected Widget onInitialize() {
		final String mess = Localizer.res().disabledTagsOnMap();
		return new UnderConstructionWidget(mess);
	}
	
	public static TagsMapWidget Instance(){
		if(instance == null){
			instance = new TagsMapWidget();
		}
		instance.resume();
		return instance;
	}
	private static TagsMapWidget instance;
	private TagsMapWidget(){
		super();
	};
}
