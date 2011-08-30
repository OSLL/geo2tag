package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.services.channels.ChannelService;
import ru.spb.osll.web.client.services.tags.TagService;
import ru.spb.osll.web.client.ui.common.Accessors;
import ru.spb.osll.web.client.ui.common.Fields;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;
import ru.spb.osll.web.client.ui.core.*;
import ru.spb.osll.web.client.services.objects.*;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.*;
import com.google.gwt.user.datepicker.client.DateBox;

public class TagsTableWidget extends SimpleComposite {
	
	private TableWidget<Tag> m_tagsTable;
	
	@Override
	protected String getName() {
		return "Tags table";
	}

	@Override
	protected Widget onInitialize() {
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
		return contaier;
	}
	
	@Override
	public void onResume() {
		m_tagsTable.erase();
		final User user = GTState.Instanse().getCurUser();
		fillChannelBox(user);
	}

	// TODO 
	// setSelectedChannel(Channel ch){
	// }
	
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
	    m_channelBox.setWidth("200px");
	    
	    m_radioBtnAll = new RadioButton("channle.type", "In channel", false);
	    m_radioBtnAll.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				m_channelBox.setEnabled(event.getValue());
			}
		});
	    m_radioBtnAll.setValue(true);
	    m_radioBtnMy = new RadioButton("channle.type", "My", false);
	    m_radioBtnMy.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				m_channelBox.setEnabled(!event.getValue());
			}
		});
	    
	    Button refreshBtn = new Button("Refresh");	// TODO localize
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
		m_tagsTable.erase();
		loadTags(u, dateFrom, dateTo);
	}
	
	private void refreshByChannel(){
		final Channel ch = m_channelBox.getSelectedObject();
		final Date dateFrom = m_dateBoxFrom.getValue();
		final Date dateTo = m_dateBoxTo.getValue();
		if(ch == null){
			return;
		}
		m_tagsTable.erase();
		loadTags(ch, dateFrom, dateTo);
	}

	private void loadTags(User u, Date dateFrom, Date dateTo){
		TagService.Util.getInstance().getTags(u, dateFrom, dateTo,
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
				refreshByChannel();
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("fillChannelBox", caught);
			}
		};
		ChannelService.Util.getInstance().getUserChannels(u, callback);
	}

	
	public static TagsTableWidget Instance(){
		if(instance == null){
			instance = new TagsTableWidget();
		}
		instance.resume();
		return instance;
	}
	private static TagsTableWidget instance;
	private TagsTableWidget(){
		super();
	};
}
