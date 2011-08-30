package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.GTState.ChannelStateListener;
import ru.spb.osll.web.client.GTState.UserStateListener;
import ru.spb.osll.web.client.services.channels.ChannelService;
import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.tags.TagService;
import ru.spb.osll.web.client.ui.common.Accessors;
import ru.spb.osll.web.client.ui.common.Fields;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SmartListBox;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;
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
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.RadioButton;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.user.datepicker.client.DateBox;

public class TagsTableWidget extends SimpleComposite
	implements UserStateListener, ChannelStateListener {
	
	private TableWidget<Tag> m_tagsTable;
	
	@Override
	protected String getName() {
		return "Tags table";
	}

	@Override
	protected Widget onInitialize() {
		GTState.Instanse().addOnUserStateListerer(this);
		GTState.Instanse().addOnChannelStateListerer(this);
		
		VerticalPanel contaier = UIUtil.getVerticalPanel();
		contaier.setSpacing(10);
		contaier.setWidth("100%");
		initFiltersPanel(contaier);
		
		List<TableField<Tag>> fields = new ArrayList<TableField<Tag>>();
		fields.add(Fields.TAG_FIELD_LABEL);
		fields.add(Fields.TAG_FIELD_DESC);
		fields.add(Fields.TAG_FIELD_URL);
		fields.add(Fields.TAG_FIELD_TIME);
		fields.add(Fields.TAG_FIELD_LAT);
		fields.add(Fields.TAG_FIELD_LON);
		m_tagsTable = new TableWidget<Tag>(fields);
		
		contaier.add(m_tagsTable);
		loadTags();
		return contaier;
	}
	
	@Override
	public void onChannelChange(Channel ch){
		m_tagsTable.erase();
		loadTags(ch);
	}
	
	@Override
	public void onUserChange(User u) {
		// FIXME why it call twice
		fillChannelBox(u);
	}

	private SmartListBox<Channel> m_channelBox;
    private DateBox m_dateBoxFrom;
    private DateBox m_dateBoxTo;
    private RadioButton m_radioBtnAll;		
    private RadioButton m_radioBtnMy;
    
	private void initFiltersPanel(VerticalPanel container){
	    m_dateBoxFrom = new DateBox();
	    m_dateBoxTo = new DateBox();
	    m_dateBoxFrom.setFormat(new DateBox.DefaultFormat());
	    m_dateBoxTo.setFormat(new DateBox.DefaultFormat());

	    m_channelBox = new SmartListBox<Channel>(Accessors.CHANNEL_ACC_NAME, false);
	    m_channelBox.setWidth("250px");
	    
	    m_radioBtnAll = new RadioButton("channle.type", "all", false);
	    m_radioBtnAll.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				m_channelBox.setEnabled(event.getValue());
			}
		});
	    m_radioBtnAll.setValue(true);
	    m_radioBtnMy = new RadioButton("channle.type", "my", false);

	    Button refreshBtn = new Button("Refresh");	// TODO localize
	    refreshBtn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				final Channel ch = m_channelBox.getSelectedObject();
				final Date dateFrom = m_dateBoxFrom.getValue();
				final Date dateTo = m_dateBoxTo.getValue();
				refresh(ch, dateFrom, dateTo);
			}
		});

	    Widget widgets[] = {
	    		cosntuctPair("Channel", m_channelBox),
	    		cosntuctPair("From", m_dateBoxFrom),
	    		cosntuctPair("To", m_dateBoxTo),
	    		m_radioBtnAll,
	    		m_radioBtnMy,
	    		refreshBtn
	    };

	    DecoratorPanel decPanel = new DecoratorPanel();
		decPanel.setWidth("100%");
		decPanel.setWidget(cosntuctLine(widgets));
		container.add(decPanel);
	}
	
	private Widget cosntuctPair(String label, Widget w){
		Grid container = new Grid(1, 2);
		container.setCellSpacing(8);
		container.setWidget(0, 0, new Label(label));
		container.setWidget(0, 1, w);
		return container;
	}

	private Widget cosntuctLine(Widget widgets[]){
		Grid container = new Grid(1, widgets.length);
		container.setCellSpacing(10);
		for (int i = 0; i < widgets.length; i++){
			container.setWidget(0, i, widgets[i]);
		}
		return container;
	}

	private void refresh(Channel ch, Date dateFrom, Date dateTo){
		// TODO
		if(ch == null){
			return;
		}
		GWT.log("dateFrom: " + dateFrom);
		GWT.log("dateTo: " + dateTo);
		
		m_tagsTable.erase();
		loadTags(ch, dateFrom, dateTo);
	}
	
	private void loadTags(){
		final Channel ch = GTState.Instanse().getCurChannel();
		if (ch == null){
			return;
		}
		loadTags(ch);
	}
	
	private void loadTags(Channel ch){
		TagService.Util.getInstance().getTags(ch, new AsyncCallback<List<Tag>>() {
			@Override
			public void onSuccess(List<Tag> result) {
				m_tagsTable.addRows(result);
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("loadTags", caught);
			}
		});
	}

	private void loadTags(Channel ch, Date dateFrom, Date dateTo){
		TagService.Util.getInstance().getTags(ch, dateFrom, dateTo,
			new AsyncCallback<List<Tag>>() {
				@Override
				public void onSuccess(List<Tag> result) {
					m_tagsTable.addRows(result);
				}
				@Override
				public void onFailure(Throwable caught) {
					GWT.log("loadTags", caught);
				}
			}
		);
	}
	
	private void fillChannelBox(final User u){
		if (u == null){
			return;
		}
		AsyncCallback<List<Channel>> callback = new AsyncCallback<List<Channel>>() {
			@Override
			public void onSuccess(List<Channel> result) {
				m_channelBox.setData(result);
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("fillChannelBox", caught);
			}
		};
		ChannelService.Util.getInstance().getUserChannels(u, callback);
	}
}
