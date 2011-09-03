package ru.spb.osll.web.client.ui.core;

import ru.spb.osll.web.client.localization.GTLocalization;
import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleComposite extends Composite {
	protected GTLocalization LOC;
	
	public SimpleComposite(){
		LOC = Localizer.res();
		initWidget(onInitialize());
	}

	public void resume(){
		onResume();
	}
	
	abstract protected String getName();

	abstract protected Widget onInitialize();

	protected void onResume(){}

	protected ImageResource getImage() {
		return Images.res().peek();
	}
}
