package ru.spb.osll.web.client.ui.widgets;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.services.users.UserState;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

public class HomePage extends SimpleComposite {

	@Override
	protected String getName() {
		return Localizer.res().pageHome(); 
	}

	@Override
	protected Widget onInitialize() {
		Button login = new Button(Localizer.res().btnSignout());
		login.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				logout();
			}
		});
		
		VerticalPanel layout = UIUtil.getVerticalPanel(true);
		layout.add(login);
		return layout;
	}
	
	private void logout(){
		final AsyncCallback<Boolean> callback = new AsyncCallback<Boolean>() {
			@Override
			public void onFailure(Throwable caught) {
				// TODO log
			}
			@Override
			public void onSuccess(Boolean result) {
				UserState.Instanse().setCurUser(null);
			}		
		};
		LoginService.Util.getInstance().logout(callback);
	}
	
	public static HomePage Instance(){
		if (instance == null){
			instance = new HomePage();
		}
		return instance;
	}
	private static HomePage instance;
	private HomePage(){
		super();
	}
}
