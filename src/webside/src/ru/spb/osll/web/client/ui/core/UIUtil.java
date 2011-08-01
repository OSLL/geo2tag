package ru.spb.osll.web.client.ui.core;

import com.google.gwt.user.client.ui.HasHorizontalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.PasswordTextBox;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.VerticalPanel;

public class UIUtil {
	public static String FILL_PARENT = "100%";
	
	public static TextBox getTextBox(int  visibleLength, int width){
		TextBox textBox = new TextBox();
		textBox.setVisibleLength(visibleLength);
		textBox.setWidth(width + "px");
		return textBox;
	}

	public static TextBox getPasswordTextBox(int visibleLength, int width){
		PasswordTextBox passwordTextBox = new PasswordTextBox();
		passwordTextBox.setVisibleLength(visibleLength);
		passwordTextBox.setWidth(width + "px");
		return passwordTextBox;
	}

	public static VerticalPanel getVerticalPanel(){
		return new VerticalPanel();
	}
	
	public static VerticalPanel getVerticalPanel(boolean alignCenter){
		VerticalPanel verticalPanel = new VerticalPanel();
		if (alignCenter){
			verticalPanel.setWidth("100%");
			verticalPanel.setHorizontalAlignment(HasHorizontalAlignment.ALIGN_CENTER);
		}
		return verticalPanel;
	}

	public static HorizontalPanel getHorizontalPanel(){
		return new HorizontalPanel();
	}
	
	public static HorizontalPanel getHorizontalPanel(boolean alignCenter){
		HorizontalPanel horizontalPanel = new HorizontalPanel();
		if (alignCenter){
			horizontalPanel.setWidth("100%");
			horizontalPanel.setHorizontalAlignment(HasHorizontalAlignment.ALIGN_CENTER);
		}
		return horizontalPanel;
	}

}
