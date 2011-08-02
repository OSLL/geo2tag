package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.rpc.services.login.LoginService;
import ru.spb.osll.web.client.rpc.services.login.LoginServiceAsync;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.ui.core.FieldsWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.PasswordTextBox;
import com.google.gwt.user.client.ui.TextBox;

public class LoginWidget extends FieldsWidget {
	
	TextBox m_loginField;
	PasswordTextBox m_passField;
	
	@Override
	protected String getName() {
		return Localizer.res().login();
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
	
	private final LoginServiceAsync service = GWT.create(LoginService.class);

	@Override
	protected List<Button> getButtons() {
		Button btn = new Button(Localizer.res().btnSignin());
		btn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				final User user = new User(m_loginField.getText(), m_passField.getText());
				service.login(user, new AsyncCallback<User>() {
					@Override
					public void onFailure(Throwable caught) {
						Window.alert(caught.getMessage());
					}

					@Override
					public void onSuccess(User result) {
						if (result == null){
							Window.alert(" user == null ");
						} else {
							Window.alert("id = " + result.getId() + " login = " + result.getLogin());
						}
					}
				});
			}
		});
		
		List<Button> buttons = new ArrayList<Button>();
		buttons.add(btn);
		return buttons;
	}
	
}
