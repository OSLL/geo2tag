/*
 * Copyright 2011-2012 OSLL
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.web.client.ui.core;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.FlexTable;
import com.google.gwt.user.client.ui.HasHorizontalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.client.ui.FlexTable.FlexCellFormatter;

public abstract class FieldsWidget extends SimpleComposite {
	private Label m_messageLabel;
	
	public FieldsWidget(){
		super();
	}
	
	protected boolean alignOnCenter(){
		return true;
	}
	
	protected String getTitleString(){
		return null;
	}
	
	protected abstract List<TField> getFields();
	protected abstract List<Button> getButtons();
	
	protected List<Anchor> getLinks(){
		return new ArrayList<Anchor>();
	}
	
	
	@Override
	protected Widget onInitialize() {
		FlexTable fieldsContainer = new FlexTable();
		fieldsContainer.setCellSpacing(8);
		FlexCellFormatter cellFormatter = fieldsContainer.getFlexCellFormatter();

		// Add a title to the form
		final String title = getTitleString();
		if (title != null){
			fieldsContainer.setHTML(0, 0, title);
		}
		cellFormatter.setColSpan(0, 0, 2);
		cellFormatter.setHorizontalAlignment(0, 0, HasHorizontalAlignment.ALIGN_CENTER);

		// FIELDS
		final List<TField> fields = getFields();
		for (int i = 0; i < fields.size(); i++){
			final TField field = fields.get(i);
			fieldsContainer.setHTML(i + 1, 0, field.name);
			fieldsContainer.setWidget(i + 1, 1, field.widget);
		}

		// MESSAGE
		HorizontalPanel messageContainer = UIUtil.getHorizontalPanel(true);
		m_messageLabel = new Label();
		m_messageLabel.setVisible(false);
		m_messageLabel.setStyleName("warning-label-style");
		messageContainer.add(m_messageLabel);

		// LINKS
		HorizontalPanel linksContainer = UIUtil.getHorizontalPanel(true);
		for (Anchor a : getLinks()) {
			linksContainer.add(a);
		}
		
		// BUTTONS
		HorizontalPanel buttonsContainer = UIUtil.getHorizontalPanel(true, 8);
		for (Button button : getButtons()){
			buttonsContainer.add(button);
		}
		
		VerticalPanel verticalPanel = UIUtil.getVerticalPanel();
		verticalPanel.add(fieldsContainer);
		verticalPanel.add(messageContainer);
		verticalPanel.add(linksContainer);
		verticalPanel.add(buttonsContainer);

		DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidget(verticalPanel);

		VerticalPanel layout = UIUtil.getVerticalPanel(alignOnCenter());
		layout.add(decPanel);
		return layout;
	}
	
	public void showMessage(String message){
		m_messageLabel.setText(message);
		m_messageLabel.setVisible(true);
	}

	public void hideMessage(){
		m_messageLabel.setVisible(false);
	}
	
	public static class TField {
		public final String name;
		public final Widget widget;
		public TField(String name, Widget widget){
			this.name = name;
			this.widget = widget;
		}
	}
}
