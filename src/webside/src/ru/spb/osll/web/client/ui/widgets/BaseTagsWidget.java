package ru.spb.osll.web.client.ui.widgets;

import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.services.channels.ChannelService;
import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.tags.TagService;
import ru.spb.osll.web.client.ui.common.Accessors;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SmartListBox;
import ru.spb.osll.web.client.ui.core.UIUtil;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Button;
import com.google.gwt.user.client.ui.DecoratorPanel;
import com.google.gwt.user.client.ui.Grid;
import com.google.gwt.user.client.ui.IsWidget;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.PopupPanel;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.datepicker.client.DateBox;

public abstract class BaseTagsWidget extends SimpleComposite {
	private TagsView m_tagsView; 
	
	private SmartListBox<Channel> m_channelBox;
    private DateBox m_dateBoxFrom;
    private DateBox m_dateBoxTo;
    private RadioButton m_radioBtnAll;		
    private RadioButton m_radioBtnMy;
	
    protected abstract TagsView getTagsView();
    
	@Override
	protected Widget onInitialize() {
		m_tagsView = getTagsView();
		VerticalPanel contaier = UIUtil.getVerticalPanel();
		contaier.setSpacing(10);
		contaier.setWidth("100%");
		initFiltersPanel(contaier);
		contaier.add(m_tagsView);
		return contaier;
	}
    
	@Override
	public void onResume() {
		final User user = GTState.Instanse().getCurUser();
		refreshChannelBox(user);
		m_tagsView.clear();
	}
	
	private void initFiltersPanel(VerticalPanel container){
	    m_dateBoxFrom = new GTDateBox();
	    m_dateBoxTo = new GTDateBox();
	    
	    m_dateBoxFrom.setFormat(new DateBox.DefaultFormat());
	    m_dateBoxTo.setFormat(new DateBox.DefaultFormat());

	    m_channelBox = new SmartListBox<Channel>(Accessors.CHANNEL_ACC_NAME, false);
	    m_channelBox.setWidth("200px");
	    
	    final String uniqueId = UIUtil.getUniqueId("channle.type"); 
	    m_radioBtnAll = new RadioButton(uniqueId, LOC.radioBtnInChannel(), false);
	    m_radioBtnAll.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				m_channelBox.setEnabled(event.getValue());
			}
		});
	    m_radioBtnAll.setValue(true);
	    m_radioBtnMy = new RadioButton(uniqueId, LOC.radioBtnMy(), false);
	    m_radioBtnMy.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				m_channelBox.setEnabled(!event.getValue());
			}
		});
	    
	    Button refreshBtn = new Button(LOC.btnRefresh());
	    refreshBtn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				if (m_radioBtnAll.getValue()){
					refreshByChannel();
				} else {
					refreshByUser();
				}
			}
		});

	    Widget widgets[] = {
	    		new Label("Tags"),
	    		m_radioBtnAll,
	    		m_radioBtnMy,
	    		cosntuctPair("Channel", m_channelBox),
	    		cosntuctPair("From", m_dateBoxFrom),
	    		cosntuctPair("To", m_dateBoxTo),
	    		refreshBtn
	    };

	    DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidth("100%");
		decPanel.setWidget(cosntuctLine(widgets));
		container.add(decPanel);
	}
	
	private Widget cosntuctPair(String label, Widget w){
		Grid container = new Grid(1, 2);
		container.setCellSpacing(4);
		container.setWidget(0, 0, new Label(label));
		container.setWidget(0, 1, w);
		return container;
	}

	private Widget cosntuctLine(Widget widgets[]){
		Grid container = new Grid(1, widgets.length);
		container.setCellSpacing(5);
		for (int i = 0; i < widgets.length; i++){
			container.setWidget(0, i, widgets[i]);
		}
		return container;
	}

	private void refreshByUser(){
		final User u = GTState.Instanse().getCurUser();
		final Date dateFrom = m_dateBoxFrom.getValue();
		final Date dateTo = m_dateBoxTo.getValue();
		if(u == null){
			return;
		}
		loadTags(u, dateFrom, dateTo);
		m_tagsView.clear();
	}
	
	private void refreshByChannel(){
		final Channel ch = m_channelBox.getSelectedObject();
		final Date dateFrom = m_dateBoxFrom.getValue();
		final Date dateTo = m_dateBoxTo.getValue();
		if(ch == null){
			return;
		}
		loadTags(ch, dateFrom, dateTo);
		m_tagsView.clear();
	}

	private void loadTags(User u, Date dateFrom, Date dateTo){
		TagService.Util.getInstance().getTags(u, dateFrom, dateTo,
			new AsyncCallback<List<Tag>>() {
				@Override
				public void onSuccess(List<Tag> result) {
					m_tagsView.setTags(result);
				}
				@Override
				public void onFailure(Throwable caught) {
					GWT.log("loadTags", caught);
				}
			}
		);
	}
	
	private void loadTags(Channel ch, Date dateFrom, Date dateTo){
		TagService.Util.getInstance().getTags(ch, dateFrom, dateTo,
			new AsyncCallback<List<Tag>>() {
				@Override
				public void onSuccess(List<Tag> result) {
					m_tagsView.setTags(result);
				}
				@Override
				public void onFailure(Throwable caught) {
					GWT.log("loadTags", caught);
				}
			}
		);
	}
	
	private void refreshChannelBox(final User u){
		m_channelBox.clear();
		if (u == null){
			return;
		}
		AsyncCallback<List<Channel>> callback = new AsyncCallback<List<Channel>>() {
			@Override
			public void onSuccess(List<Channel> result) {
				m_channelBox.setData(result);
				refreshByChannel();
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("fillChannelBox", caught);
			}
		};
		ChannelService.Util.getInstance().getUserChannels(u, callback);
	}

	public interface TagsView extends IsWidget {
		public void setTags(List<Tag> tags);
		public void clear();
	}
	
	// customize of GWT's DateBox class 
	private static class GTDateBox extends DateBox {
		private final static int SIZE = 120;
		public GTDateBox(){
			super();
			getDatePicker().setSize(SIZE + "px", SIZE + "px");
		}

		@Override
		public void showDatePicker() {
			//it's a trick
			super.showDatePicker();
			super.hideDatePicker();
			final int offsetY = SIZE + 10;
			PopupPanel popup = (PopupPanel)getDatePicker().getParent();
			popup.setPopupPosition(getAbsoluteLeft(), getAbsoluteTop() - offsetY);
			popup.show();
		}
	}
}

