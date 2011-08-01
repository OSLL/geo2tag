package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.core.FieldsWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.user.client.ui.Button;

public class LoginWidget extends FieldsWidget {

	@Override
	protected String getName() {
		return Localizer.res().login();
	}

	@Override
	protected List<Button> getButtons() {
		List<Button> buttons = new ArrayList<Button>();
		buttons.add(new Button(Localizer.res().btnSignin()));
		return buttons;
	}

	@Override
	protected List<TField> getFields() {
		List<TField> fields = new ArrayList<TField>();
		fields.add(new TField(Localizer.res().login(), UIUtil.getTextBox(65, 200)));
		fields.add(new TField(Localizer.res().password(), UIUtil.getPasswordTextBox(65, 200)));
		return fields;
	}
	
//	private final LoginServiceAsync service = GWT.create(LoginService.class);
//
//	class MyHandler implements ClickHandler {
//		public void onClick(ClickEvent event) {
//			sendNameToServer();
//		}
//
//		private void sendNameToServer() {
//			User user = new User("bac1ca", "bac1ca");
//			
//			service.login(user, new AsyncCallback<User>() {
//				@Override
//				public void onFailure(Throwable caught) {
//					Window.alert(caught.getMessage());
//				}
//
//				@Override
//				public void onSuccess(User result) {
//					Window.alert("id = " + result.getId() + "login = " + result.getLogin());
//				}
//			});
//		}
//	}

}
