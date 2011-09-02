package ru.spb.osll.web.client.ui.widgets;

import ru.spb.osll.web.client.localization.Localizer;

public class TagsMapWidget extends BaseTagsWidget {

	@Override
	protected String getName() {
		return Localizer.res().TagsMapWidget();
	}

	@Override
	protected TagsView getTagsView() {
		return new OSMWidget();
	};
	
	
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
	}
}
