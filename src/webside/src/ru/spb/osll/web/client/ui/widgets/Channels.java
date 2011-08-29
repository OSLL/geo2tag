package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.GTState.UserStateListener;
import ru.spb.osll.web.client.services.channels.ChannelService;
import ru.spb.osll.web.client.services.channels.ChannelServiceAsync;
import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
import ru.spb.osll.web.client.ui.core.TableWidget.IsTableAccessor;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.DOM;
import com.google.gwt.user.client.Window;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.ScrollPanel;
import com.google.gwt.user.client.ui.SimplePanel;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

public class Channels extends SimpleComposite 
	implements UserStateListener {

	private TableWidget<Channel> m_userChannels;
	private TableWidget<Channel> m_avalChannels;
	
	@Override
	protected String getName() {
		return "Channels"; // TODO localize
	}

	@Override
	protected Widget onInitialize() {
		GTState.Instanse().addOnUserStateListerer(this);
		
		List<TableField<Channel>> fields = new ArrayList<TableField<Channel>>();
		fields.add(FIELD_NAME);
		fields.add(FIELD_DESC);
		fields.add(FIELD_URL);

		m_userChannels = new TableWidget<Channel>(fields);
		m_avalChannels = new TableWidget<Channel>(fields);
		loadUserChannels();
		loadAvaiChannels();
		
		VerticalPanel vp = UIUtil.getVerticalPanel();
		vp.setSpacing(10);

		vp.add(constructTitle("Your channels", 20));
		vp.add(borderOnTable(m_userChannels));
		vp.add(initButtons());
		vp.add(constructTitle("Available channels", 20));
		vp.add(borderOnTable(m_avalChannels));
		vp.setWidth("100%");
		return vp;
	}
	
	@Override
	public void onUserChange(User u) {
		String logMess = (u != null) ? u.toString() : "null";
		GWT.log(logMess);
		
		m_userChannels.erase();
		m_avalChannels.erase();
		loadUserChannels();
		loadAvaiChannels();
	}
	
	private HorizontalPanel initButtons(){
		Button subscribeBtn = new Button("sub", new ClickHandler() {		// TODO
			@Override
			public void onClick(ClickEvent event) {
				subscribe();
			}
		});
		
		Button unsubscribeBtn = new Button("unsub", new ClickHandler() {	// TODO
			@Override
			public void onClick(ClickEvent event) {
				unsubscribe();
			}
		});
		
		Button showTagsBtn = new Button("Show tags", new ClickHandler() {	// TODO
			@Override
			public void onClick(ClickEvent event) {
				GTState.Instanse().setCurChannel(m_userChannels.getSelectedObject());
				GTShell.Instance.setContent(new TagsTableWidget());
			}
		});
		
		HorizontalPanel buttons = UIUtil.getHorizontalPanel();
		buttons.setSpacing(8);
		buttons.add(subscribeBtn);
		buttons.add(unsubscribeBtn);
		buttons.add(showTagsBtn);
		return buttons;
	}
	
	private void subscribe(){
		final User u = GTState.Instanse().getCurUser();
		final Channel ch = m_avalChannels.getSelectedObject();
		if (null == u || ch == null){
			return;
		}
		
		ChannelService.Util.getInstance().subscribe(ch, u, new AsyncCallback<Boolean>() {
			@Override
			public void onSuccess(Boolean result) {
				if (result){
					m_userChannels.addRow(ch);
				}
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("onFailure : " + caught.getMessage());
			}
		});
		
	}
	
	private void unsubscribe(){
		final User u = GTState.Instanse().getCurUser();
		final Channel ch = m_userChannels.getSelectedObject();
		if (null == u || ch == null){
			return;
		}
		
		ChannelService.Util.getInstance().unsubscribe(ch, u, new AsyncCallback<Boolean>() {
			@Override
			public void onSuccess(Boolean result) {
				if (result){
					m_userChannels.deleteRow(m_userChannels.getSelectedRow());
				}
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("onFailure : " + caught.getMessage());
			}
		});
	}

	private Label constructTitle(String title, int px){
		return UIUtil.constructLabel(title, px);
	}
	
	private SimplePanel borderOnTable(TableWidget<?> table){
		ScrollPanel scrollPanel = new ScrollPanel();
		scrollPanel.add(table);
		scrollPanel.setHeight(Window.getClientHeight()/3 + "px");
        DOM.setStyleAttribute(scrollPanel.getElement(), "border", "1px solid dodgerblue");
		return scrollPanel;
	}
	
	private void loadUserChannels(){
		final User u = GTState.Instanse().getCurUser();
		if (null == u){
			return;
		}
		
		final ChannelServiceAsync service = ChannelService.Util.getInstance();
		service.getUserChannels(u, new AsyncCallback<List<Channel>>() {
			@Override
			public void onSuccess(List<Channel> result) {
				m_userChannels.addRows(result);
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("onFailure : " + caught.getMessage());
			}
		});
	}
	
	private void loadAvaiChannels(){
		final ChannelServiceAsync service = ChannelService.Util.getInstance();
		service.getAllChannels(new AsyncCallback<List<Channel>>() {
			@Override
			public void onSuccess(List<Channel> result) {
				m_avalChannels.addRows(result);
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("onFailure : " + caught.getMessage());
			}
		});
	}

	private final static IsTableAccessor<Channel> ACC_NAME = new IsTableAccessor<Channel>() {
		public String toCell(Channel ch) {
			return ch.getName();
		}
	};

	private final static IsTableAccessor<Channel> ACC_DESC = new IsTableAccessor<Channel>() {
		public String toCell(Channel ch) {
			return ch.getDescription();
		}
	};

	private final static IsTableAccessor<Channel> ACC_URL = new IsTableAccessor<Channel>() {
		public String toCell(Channel ch) {
			return ch.getUrl();
		}
	};
	
	private final static TableField<Channel> FIELD_NAME = 
			new TableField<Channel>("field.name", "Name", ACC_NAME);

	private final static TableField<Channel> FIELD_DESC = 
			new TableField<Channel>("field.name", "Desc", ACC_DESC);

	private final static TableField<Channel> FIELD_URL = 
			new TableField<Channel>("field.name", "Url", ACC_URL);
}
