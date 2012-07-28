package ru.spb.osll.web.client;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.GTServiceAsync;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.ui.core.SimpleComposite;

import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.TableElement;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.History;
import com.google.gwt.user.client.rpc.AsyncCallback;

import com.google.gwt.user.client.ui.*;
import ru.spb.osll.web.client.ui.widgets.*;

public class GTShell extends Composite {

	private static GTShellUiBinder uiBinder = GWT.create(GTShellUiBinder.class);

	interface GTShellUiBinder extends UiBinder<Widget, GTShell> {
	}

	@UiField SimplePanel contentPanel;
	@UiField TableElement linkCell;
//	@UiField ListBox localeBox;
//	@UiField TableCellElement localeSelectionCell;
	@UiField SimplePanel mainMenuContainer;
	@UiField HorizontalPanel autentificationBox;
	
	private Anchor m_authLink;
	private Anchor m_regLink;
	private GTSiteMap m_siteMap;
	
	public static GTShell Instance;
	
	public GTShell() {
		initWidget(uiBinder.createAndBindUi(this));
		Instance = this;
		
		m_siteMap = new GTSiteMap();
		mainMenuContainer.add(m_siteMap.getMenu());

		m_regLink = new Anchor(Localizer.res().registration());
		m_regLink.addClickHandler(m_regHandler);
		
		m_authLink = new Anchor("");
		m_authLink.addClickHandler(m_authHandler);
		
		autentificationBox.setSpacing(8);
		autentificationBox.add(m_regLink);
		autentificationBox.add(m_authLink);

		initStartWidget();
		initHistoryListener();
	}

	public void setDefaultContent(){
		initDefaultWidget();
	}

	public void setContent(Widget content) {
		setContent(content, true);
	}

	private void setContent(Widget content, boolean useHistory) {
		if (content == null){
			return;
		}
		
		final String token = m_siteMap.getToken(content);
		if (useHistory) {
			History.newItem(token, false);
		}
		m_siteMap.getMenu().setSecectedGroup(content);
		contentPanel.setWidget(content);   
	}

	public void refreshAutorizedStatus(){
		boolean autorized = GTState.Instanse().getCurUser() != null;
		if (autorized){
			m_authLink.setText(Localizer.res().btnSignout());
			m_regLink.setVisible(false);
		} else {
			m_authLink.setText(Localizer.res().btnSignin());
			m_regLink.setVisible(true);
		}
	}
	
	protected void initStartWidget(){
		setContent(HomePage.Instance(), false);
		
		GTServiceAsync service = GTService.Util.getInstance();
		service.isAuthorized(new AsyncCallback<WUser>() {
			@Override
			public void onSuccess(WUser user) {
				GTState.Instanse().setCurUser(user); 
				refreshAutorizedStatus();
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("initStartWidget", caught);
			}
		});
	}
	
	private void initHistoryListener(){
		History.addValueChangeHandler(new ValueChangeHandler<String>() {
			public void onValueChange(ValueChangeEvent<String> event) {
				setContentByToken(event.getValue(), false);
			}
		});
		setContentByToken(History.getToken(), false);
	}

	private void setContentByToken(String t, boolean useHistory){
		final Widget w = m_siteMap.getWidget(t);
		if (w instanceof SimpleComposite){
			((SimpleComposite) w).resume();
		}
		setContent(w, false);
	}
	
	protected void initDefaultWidget(){
		GTServiceAsync service = GTService.Util.getInstance();
		service.isAuthorized(new AsyncCallback<WUser>() {
			@Override
			public void onSuccess(WUser user) {
				if (user == null){
					setContent(LoginWidget.Instance(), false);
				} else {
					GTState.Instanse().setCurUser(user); 
					setContent(Channels.Instance(), false);
				}
				refreshAutorizedStatus();
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("initDefaultWidget", caught);
			}
		});
	}

	private ClickHandler m_authHandler = new ClickHandler() {
		@Override
		public void onClick(ClickEvent event) {
			if (GTState.Instanse().getCurUser() != null){
				logout();
				m_authLink.setText(Localizer.res().btnSignin());
				m_regLink.setVisible(true);
			}
			setContent(LoginWidget.Instance());
			// TODO refreshAutorizedStatus()
		}
	};

	private ClickHandler m_regHandler = new ClickHandler() {
		@Override
		public void onClick(ClickEvent event) {
			setContent(RegistrationWidget.Instance());
		}
	};
	
	private void logout(){
		final AsyncCallback<Boolean> callback = new AsyncCallback<Boolean>() {
			@Override
			public void onSuccess(Boolean result) {
				GTState.Instanse().setCurUser(null);
			}		
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("logout", caught);
			}
		};
		GTService.Util.getInstance().logout(callback);
	}

}
