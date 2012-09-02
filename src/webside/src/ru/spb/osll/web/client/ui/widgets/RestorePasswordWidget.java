/*
 * Copyright 2010-2012 OSLL osll@osll.spb.ru
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

package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.ui.core.FieldsWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.TextBox;

public class RestorePasswordWidget extends FieldsWidget {
	private TextBox m_email;

	@Override
	protected String getName() {
		return LOC.passwRestoring();
	}
	
	@Override
	protected List<TField> getFields() {
		m_email = UIUtil.getTextBox(65, 200);

		List<TField> fields = new ArrayList<TField>();
		fields.add(new TField(Localizer.res().email(), m_email));
		return fields;
	}

	@Override
	protected List<Button> getButtons() {
		Button btn = new Button(Localizer.res().btnSubmit());
		btn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				hideMessage();
				restorePassword();
			}
		});

		List<Button> buttons = new ArrayList<Button>();
		buttons.add(btn);
		return buttons;
	}

	private void restorePassword(){
		hideMessage();
		String email = m_email.getText();
		AsyncCallback<Boolean> callback = new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				showMessage(caught.getMessage());
			}
			
			@Override
			public void onSuccess(Boolean result) {
				showMessage(Localizer.res().waitForPwdRestoring());
			}
		};
		GTService.Util.getInstance().restorePassword(email, callback);	
	}
	
	public void dropData() {
	    m_email.setText("");
	    hideMessage();
	}

	public static RestorePasswordWidget Instance(){
		if (instance == null){
			instance = new RestorePasswordWidget();
		}
		return instance;
	}
	private static RestorePasswordWidget instance;
	private RestorePasswordWidget(){
		super();
	}
}