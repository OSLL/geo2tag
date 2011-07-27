package ru.spb.osll.web.client.ui.core;

import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleComposite extends Composite {
		
	public SimpleComposite(){
		initWidget(onInitialize());
	}
	
	abstract protected Widget onInitialize();
	abstract protected String getName();
	abstract protected ImageResource getImage() ;
}
