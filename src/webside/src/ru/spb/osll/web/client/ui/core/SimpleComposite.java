package ru.spb.osll.web.client.ui.core;

import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleComposite extends Composite {
		
	public SimpleComposite(){
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
