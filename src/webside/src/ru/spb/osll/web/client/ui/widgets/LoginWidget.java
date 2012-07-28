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
import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.ui.core.FieldsWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.PasswordTextBox;
import com.google.gwt.user.client.ui.TextBox;

public class LoginWidget extends FieldsWidget {
	private TextBox m_loginField;
	private PasswordTextBox m_passField;
	
	@Override
	protected String getName() {
		return LOC.login();
	}

	@Override
	protected List<TField> getFields() {
		m_loginField = UIUtil.getTextBox(65, 200);
		m_passField = UIUtil.getPasswordTextBox(65, 200);
		
		List<TField> fields = new ArrayList<TField>();
		fields.add(new TField(Localizer.res().login(), m_loginField));
		fields.add(new TField(Localizer.res().password(), m_passField));
		return fields;
	}
	
	@Override
	protected List<Anchor> getLinks(){
		Anchor testAccLink = new Anchor(LOC.TestAccountWidget());
		testAccLink.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				GTShell.Instance.setContent(TestAccountWidget.Instance());
			}
		});
		
		Anchor regLink = new Anchor(Localizer.res().registration());
		regLink.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				GTShell.Instance.setContent(RegistrationWidget.Instance());
			}
		});
		
		final List<Anchor> links = new ArrayList<Anchor>();
		links.add(testAccLink);
		links.add(regLink);
		return links;
	}
	
	@Override
	protected List<Button> getButtons() {
		Button btn = new Button(Localizer.res().btnSignin());
		btn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				login();
			}
		});
		
		List<Button> buttons = new ArrayList<Button>();
		buttons.add(btn);
		return buttons;
	}
	
	private void login(){
		hideMessage();
		final WUser user = new WUser(m_loginField.getText(), m_passField.getText());
		AsyncCallback<WUser> callback = new AsyncCallback<WUser>() {
			@Override
			public void onFailure(Throwable caught) {
				showMessage(caught.getMessage());
			}
			
			@Override
			public void onSuccess(WUser user) {
				if (user != null){
					if (user.getStatus() == Response.STATUS_SUCCES){
						GTState.Instanse().setCurUser(user);
						GTShell.Instance.setDefaultContent();
					} else if (user.getStatus() == Response.STATUS_FAIL){
						showMessage(Localizer.res().loginFailed());
					}
				} else {
					showMessage(Localizer.res().invalidCredentials());
				}
			}
		};
		GTService.Util.getInstance().login(user, callback);		
	}
	
	public void dropData() {
	    m_loginField.setText("");
	    m_passField.setText("");
	}
	
	public static LoginWidget Instance(){
		if(instance == null){
			instance = new LoginWidget();
		}
		return instance;
	}
	private static LoginWidget instance;
	private LoginWidget(){
		super();
	};
}
