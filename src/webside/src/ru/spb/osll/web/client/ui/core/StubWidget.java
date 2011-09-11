package ru.spb.osll.web.client.ui.core;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;

public abstract class StubWidget extends Composite {
	
	protected abstract ImageResource getImage();
	
	protected abstract List<Label> getLabels();
	
	protected List<Anchor> getLinks(){
		return new ArrayList<Anchor>();
	}
	
	public StubWidget(){
		VerticalPanel container = UIUtil.getVerticalPanel(true);
		container.setWidth("300px");
		container.setSpacing(10);
		
		ImageResource im = getImage();
		if (null != im){
			container.add(new Image(im));
		}
		for (Label label : getLabels()){
			container.add(label);
		}
		for (Anchor link : getLinks()){
			container.add(link);
		}
		
		DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidget(container);

		VerticalPanel main = UIUtil.getVerticalPanel(true);
		main.add(decPanel);
		initWidget(main);
	}

}
