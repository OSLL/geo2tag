/*
 * Copyright 2011-2012 OSLL
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.web.client.ui.widgets;

import java.util.Date;
import java.util.List;

import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.services.channels.ChannelService;
import ru.spb.osll.web.client.services.objects.WChannel;
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
	
	private SmartListBox<WChannel> m_channelBox;
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
		showWarningMessage(user);
		refreshFilterPanel(user);
		m_tagsView.refresh();
	}

	private void showWarningMessage(User u){
		if (u != null){
			hideWarningMessage();
		} else {
			showWarningMessage(LOC.wrngNotLogged());
		}
	}
	
	private void initFiltersPanel(VerticalPanel container){
	    m_dateBoxFrom = new GTDateBox();
	    m_dateBoxTo = new GTDateBox();
	    
	    m_dateBoxFrom.setFormat(new DateBox.DefaultFormat());
	    m_dateBoxTo.setFormat(new DateBox.DefaultFormat());

	    m_channelBox = new SmartListBox<WChannel>(Accessors.CHANNEL_ACC_NAME);
	    m_channelBox.setWidth("200px");
	    
	    final String uniqueId = UIUtil.getUniqueId("channle.type"); 
	    m_radioBtnAll = new RadioButton(uniqueId, LOC.radioBtnInChannel(), false);
	    m_radioBtnAll.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
			@Override
			public void onValueChange(ValueChangeEvent<Boolean> event) {
				m_channelBox.setEnabled(event.getValue());
			}
		});
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
		m_tagsView.refresh();
	}
	
	private void refreshByChannel(){
		final Date dateFrom = m_dateBoxFrom.getValue();
		final Date dateTo = m_dateBoxTo.getValue();

		final Object obj = m_channelBox.getSelectedObject();
		if(obj == null){
			return;
		}
		else if (obj instanceof WChannel){
			final WChannel ch = (WChannel) obj;
			loadTags(ch, dateFrom, dateTo);
		}
		else if (obj instanceof List<?>){
			@SuppressWarnings("unchecked")
			final List<WChannel> chs = (List<WChannel>)obj;
			loadTags(chs, dateFrom, dateTo);
		}
		m_tagsView.refresh();
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
	
	private void loadTags(WChannel ch, Date dateFrom, Date dateTo){
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

	private void loadTags(List<WChannel> chs, Date dateFrom, Date dateTo){
		TagService.Util.getInstance().getTags(chs, dateFrom, dateTo,
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
	
	private void refreshFilterPanel(final User u){
		m_channelBox.clear();
		if (u == null){
			return;
		}
		AsyncCallback<List<WChannel>> callback = new AsyncCallback<List<WChannel>>() {
			@Override
			public void onSuccess(List<WChannel> result) {
				m_channelBox.setData(result, LOC.itemAllChannels());
				if (result.size() > 0){
					m_radioBtnAll.setValue(true);
					m_radioBtnMy.setValue(false);
					refreshByChannel();
				} else {
					m_radioBtnMy.setValue(true);
					m_radioBtnAll.setValue(false);
					refreshByUser();
				}
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
		public void refresh();
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

