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
import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.ui.core.FieldsWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
import ru.spb.osll.web.client.ui.core.UnderConstructionWidget;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.PasswordTextBox;
import com.google.gwt.user.client.ui.TextBox;
import com.google.gwt.user.client.ui.Widget;

public class RegistrationWidget extends FieldsWidget {
	private TextBox m_login;
	private PasswordTextBox m_pass;
	private PasswordTextBox m_passConfirm;

	@Override
	protected String getName() {
		return LOC.registration();
	}

	// TODO delete later
	@Override
	protected Widget onInitialize() {
		// super.onInitialize();
		final String mess = LOC.disabledRegistration();
		return new UnderConstructionWidget(mess);
	}

	@Override
	protected List<TField> getFields() {
		m_login = UIUtil.getTextBox(65, 200);
		m_pass = UIUtil.getPasswordTextBox(65, 200);
		m_passConfirm = UIUtil.getPasswordTextBox(65, 200);

		List<TField> fields = new ArrayList<TField>();
		fields.add(new TField(Localizer.res().login(), m_login));
		fields.add(new TField(Localizer.res().password(), m_pass));
		fields.add(new TField(Localizer.res().confirm(), m_passConfirm));
		return fields;
	}

	@Override
	protected List<Button> getButtons() {
		Button btn = new Button(Localizer.res().btnSignin());
		btn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				hideMessage();
				if (isValid()) {
					final User user = new User(m_login.getText(), m_pass.getText());
					final AsyncCallback<User> callback = new AsyncCallback<User>() {
						@Override
						public void onFailure(Throwable caught) {
							showMessage(caught.getMessage());
						}
						@Override
						public void onSuccess(User user) {
							if (user != null) {
								if (user.getStatus() == Response.STATUS_SUCCES) {
									final String title = Localizer.res().registration();
									UIUtil.getSimpleDialog(title,user.getMessage()).center();
									GTShell.Instance.setDefaultContent();
								} else {
									showMessage(user.getMessage());
								}
							}
						}
					};
					LoginService.Util.getInstance().addUser(user, callback);
				}
			}
		});

		List<Button> buttons = new ArrayList<Button>();
		buttons.add(btn);
		return buttons;
	}

	private boolean isValid() {
		final String login = m_login.getText();
		final String pass = m_pass.getText();
		final String conf = m_passConfirm.getText();

		if (login.equals("")) {
			showMessage(Localizer.res().wrngLoginNull());
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
