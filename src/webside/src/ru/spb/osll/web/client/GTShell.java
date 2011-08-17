package ru.spb.osll.web.client;

import java.util.HashMap;
import java.util.Map;

import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.services.users.LoginServiceAsync;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.widgets.HomePage;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;

import com.google.gwt.core.client.GWT;
import com.google.gwt.dom.client.TableCellElement;
import com.google.gwt.dom.client.TableElement;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.uibinder.client.UiBinder;
import com.google.gwt.uibinder.client.UiField;
import com.google.gwt.user.client.History;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.ListBox;
import com.google.gwt.user.client.ui.SimplePanel;
import com.google.gwt.user.client.ui.Widget;

public class GTShell extends Composite {

	private static GTShellUiBinder uiBinder = GWT.create(GTShellUiBinder.class);

	interface GTShellUiBinder extends UiBinder<Widget, GTShell> {
	}

	@UiField SimplePanel contentPanel;
	@UiField TableElement linkCell;
	@UiField ListBox localeBox;
	@UiField TableCellElement localeSelectionCell;
	@UiField SimplePanel mainMenuContainer;

	private GTMenu m_mainMenu;
	private Map<String, Widget> m_widgetsCache = new HashMap<String, Widget>();
	
	public static GTShell Instance;
	
	public GTShell() {
		initWidget(uiBinder.createAndBindUi(this));
		m_mainMenu = new GTMenu();
		mainMenuContainer.add(m_mainMenu);
		Instance = this;
		
		initDefaultWidget();
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
			setDefaultContent();
			return;
		}
		
		final String token = getTokenByWidget(content);
		m_widgetsCache.put(token, content);
		if (useHistory) {
			History.newItem(token, false);
		}
		if (content instanceof SimpleComposite){
			m_mainMenu.setSecectedGroup((SimpleComposite)content);
		}
		contentPanel.setWidget(content);   
	}
	
	private void initHistoryListener(){
		History.addValueChangeHandler(new ValueChangeHandler<String>() {
			public void onValueChange(ValueChangeEvent<String> event) {
				final String t = event.getValue();
				final Widget w = getWidgetByToken(t);
				setContent(w);
			}
		});
	}
	
	protected void initDefaultWidget(){
		LoginServiceAsync service = LoginService.Util.getInstance();
		service.isAuthorized(new AsyncCallback<User>() {
			@Override
			public void onSuccess(User user) {
				if (user == null){
					setContent(LoginWidget.Instance(), false);
				} else {
					setContent(HomePage.Instance(), false);
				}				
			}
			@Override
			public void onFailure(Throwable caught) {
				//TODO
				//Logger.getLogger(GTShell.class).warn(caught.getMessage()); 
			}
		});
	}
	
	private String getTokenByWidget(Widget w){
		if (w == null){
			return "";
		}
		final String group = null;
		if (w instanceof SimpleComposite) {
			m_mainMenu.getGroup((SimpleComposite) w);
		}
		final String token = getTokenByClass(w.getClass());
		return group == null ? token : group + "_" + token;
	}
	
	private Widget getWidgetByToken(String t){
		final Widget w = m_widgetsCache.get(t);
		m_widgetsCache.remove(t);
		return w;
	}

	public static String getTokenByClass(Class<?> cwClass) {
		String className = cwClass.getName();
		className = className.substring(className.lastIndexOf('.') + 1);
		return className;
	}
}
