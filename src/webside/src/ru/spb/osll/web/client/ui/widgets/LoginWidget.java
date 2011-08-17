package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.services.objects.Response;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.services.users.UserState;
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
	
	@Override
	protected List<Anchor> getLinks(){
		Anchor regLink = new Anchor(Localizer.res().registration());
		regLink.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				GTShell.Instance.setContent(RegistrationWidget.Instance());
			}
		});
		
		final List<Anchor> links = new ArrayList<Anchor>();
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
		final User user = new User(m_loginField.getText(), m_passField.getText());
		
		AsyncCallback<User> callback = new AsyncCallback<User>() {
			@Override
			public void onFailure(Throwable caught) {
				showMessage(caught.getMessage());
			}
			
			@Override
			public void onSuccess(User user) {
				if (user != null){
					if (user.getStatus() == Response.STATUS_SUCCES){
						UserState.Instanse().setCurUser(user);
						
						final String title = Localizer.res().login();
						UIUtil.getSimpleDialog(title, user.getMessage()).center();
						GTShell.Instance.setDefaultContent();
					} else if (user.getStatus() == Response.STATUS_FAIL){
						showMessage(user.getMessage());
					}
				}
			}
		};
		LoginService.Util.getInstance().login(user, callback);		
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
