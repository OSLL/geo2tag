package ru.spb.osll.web.client.ui.core;

import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;

public class UnderConstructionWidget extends Composite {
	
	private Label m_message;
	
	public UnderConstructionWidget(){
		this(null);
	}

	public UnderConstructionWidget(String message){
		m_message = UIUtil.constructLabel(17, "100%");
		if (null != message){
			m_message.setText(message);
		}
		Image image = new Image(Images.res().underConstruction());
		
		VerticalPanel container = UIUtil.getVerticalPanel(true);
		container.setWidth("300px");
		container.setSpacing(10);
		container.add(image);
		container.add(m_message);
		
		DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidget(container);

		VerticalPanel main = UIUtil.getVerticalPanel(true);
		main.add(decPanel);
		initWidget(main);
	}
	
	public void setText(String message){
		m_message.setText(message);
	}
	
}
