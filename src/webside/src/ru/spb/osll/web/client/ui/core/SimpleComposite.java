package ru.spb.osll.web.client.ui.core;

import ru.spb.osll.web.client.localization.GTLocalization;
import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleComposite extends Composite {
	protected GTLocalization LOC;
	private Label m_specMess;
	
	public SimpleComposite(){
		LOC = Localizer.res();
		m_specMess = UIUtil.constructLabel(16);
		m_specMess.setStyleName("warning-label-style");
		m_specMess.setVisible(false);
		
		VerticalPanel cotainer = UIUtil.getVerticalPanel(true);
		cotainer.add(m_specMess);
		cotainer.add(onInitialize());
		initWidget(cotainer);
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
	
	public void showWarningMessage(String mess){
		m_specMess.setText(mess);
		m_specMess.setVisible(true);
	}

	public void hideWarningMessage(){
		m_specMess.setVisible(false);
	}
	
}
