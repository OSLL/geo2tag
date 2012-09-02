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

package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.ui.core.FieldsWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.Timer;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.PasswordTextBox;
import com.google.gwt.user.client.ui.TextBox;

public class RegistrationWidget extends FieldsWidget {
	private TextBox m_login;
	private TextBox m_email;
	private PasswordTextBox m_pass;
	private PasswordTextBox m_passConfirm;

	@Override
	protected String getName() {
		return LOC.registration();
	}
	
	@Override
	protected List<Anchor> getLinks() {
		Anchor loginLink = new Anchor(Localizer.res().pageLogin());
		loginLink.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				dropData();
				GTShell.Instance.setContent(LoginWidget.Instance());
			}
		});
		
		Anchor forgotPasswordLink = new Anchor(Localizer.res().forgotPassword());
		forgotPasswordLink.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				dropData();
				GTShell.Instance.setContent(RestorePasswordWidget.Instance());
			}
		});
		
		List<Anchor> links = new ArrayList<Anchor>();
		links.add(loginLink);
		links.add(forgotPasswordLink);
		return links;
	}

	@Override
	protected List<TField> getFields() {
		m_login = UIUtil.getTextBox(65, 200);
		m_email = UIUtil.getTextBox(65, 200);
		m_pass = UIUtil.getPasswordTextBox(65, 200);
		m_passConfirm = UIUtil.getPasswordTextBox(65, 200);

		List<TField> fields = new ArrayList<TField>();
		fields.add(new TField(Localizer.res().login(), m_login));
		fields.add(new TField(Localizer.res().email(), m_email));
		fields.add(new TField(Localizer.res().password(), m_pass));
		fields.add(new TField(Localizer.res().confirm(), m_passConfirm));
		return fields;
	}

	@Override
	protected List<Button> getButtons() {
		Button btn = new Button(Localizer.res().btnRegister());
		btn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				hideMessage();
				registerUser();
			}
		});

		List<Button> buttons = new ArrayList<Button>();
		buttons.add(btn);
		return buttons;
	}

	private void registerUser(){
		hideMessage();
		if (isValid()) {
			final WUser user = new WUser(m_login.getText(), m_pass.getText());
			user.setEmail(m_email.getText());
			AsyncCallback<Boolean> callback = new AsyncCallback<Boolean>() {
				@Override
				public void onSuccess(Boolean result) {
					if (result == true) {
						showMessage(Localizer.res().waitForMessage());
						LoginWidget.Instance().dropData();
						Timer timer = new Timer()
				        {
				            @Override
				            public void run()
				            {
				            	dropData();
				            	GTShell.Instance.setContent(LoginWidget.Instance());
				            }
				        };
				        timer.schedule(2500);
					} else {
						showMessage(Localizer.res().userExists());
					}
				}
				@Override
				public void onFailure(Throwable caught) {
					showMessage(caught.getMessage());
				}
			};
			GTService.Util.getInstance().registerUser(user, callback);
		}
	}
	
	private boolean isValid() {
		final String login = m_login.getText();
		final String email = m_email.getText();
		final String pass = m_pass.getText();
		final String conf = m_passConfirm.getText();

		if (login.equals("")) {
			showMessage(Localizer.res().wrngLoginNull());
			return false;
		}
		if (email.equals("")) {
			showMessage(Localizer.res().wrngEmailNull());
			return false;
		}		
		if (pass.equals("")) {
			showMessage(Localizer.res().wrngPasswordNull());
			return false;
		}
		if (conf.equals("")) {
			showMessage(Localizer.res().wrngConfirmNull());
			return false;
		}
		if (!pass.equals(conf)) {
			showMessage(Localizer.res().wrngPassAndConf());
			return false;
		}
		return true;
	}
	
	public void dropData() {
	    m_login.setText("");
	    m_email.setText("");
	    m_pass.setText("");
	    m_passConfirm.setText("");
	    hideMessage();
	}

	public static RegistrationWidget Instance(){
		if (instance == null){
			instance = new RegistrationWidget();
		}
		return instance;
	}
	private static RegistrationWidget instance;
	private RegistrationWidget(){
		super();
	}
}
