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

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.GTServiceAsync;
import ru.spb.osll.web.client.services.objects.WUser;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.ui.common.Fields;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
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

public class Channels extends SimpleComposite {
	private TableWidget<WChannel> m_userChannels;
	private TableWidget<WChannel> m_avalChannels;
	
	@Override
	protected String getName() {
		return LOC.channels(); 
	}

	@Override
	protected Widget onInitialize() {
		List<TableField<WChannel>> fields = new ArrayList<TableField<WChannel>>();
		fields.add(Fields.CHANNEL_FIELD_NAME);
		fields.add(Fields.CHANNEL_FIELD_DESC);
		fields.add(Fields.CHANNEL_FIELD_URL);

		m_userChannels = new TableWidget<WChannel>(fields);
		m_avalChannels = new TableWidget<WChannel>(fields);
		
		refreshUserChannels(true);
		//refreshAvaiChannels();
		
		VerticalPanel vp = UIUtil.getVerticalPanel();
		vp.setSpacing(10);

		vp.add(constructTitle(LOC.labYourChannels(), 20)); 
		vp.add(borderOnTable(m_userChannels));
		vp.add(initButtons());
		vp.add(constructTitle(LOC.labAvalChannels(), 20));	
		vp.add(borderOnTable(m_avalChannels));
		vp.setWidth("100%");
		return vp;
	}
	
	@Override
	public void onResume() {
		showWarningMessage(GTState.Instanse().getCurUser());
		refreshUserChannels(true);
		//refreshAvaiChannels();
	}

	private void showWarningMessage(WUser u){
		if (u != null){
			hideWarningMessage();
		} else {
			showWarningMessage(LOC.wrngNotLogged());
		}
	}
	
	private HorizontalPanel initButtons(){
		Button subscribeBtn = new Button(LOC.btnSubscribe(), new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				subscribe();
			}
		});
		
		Button unsubscribeBtn = new Button(LOC.btnUnsubscribe(), new ClickHandler() {	
			@Override
			public void onClick(ClickEvent event) {
				unsubscribe();
			}
		});
		
		Button showTagsBtn = new Button(LOC.btnShowTags(), new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
				// TODO setSelected Channel
				GTState.Instanse().setCurChannel(m_userChannels.getSelectedObject());
				GTShell.Instance.setContent(TagsTableWidget.Instance());
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
		final WUser u = GTState.Instanse().getCurUser();
		final WChannel ch = m_avalChannels.getSelectedObject();
		if (null == u || ch == null){
			return;
		}
		
		GTService.Util.getInstance().subscribe(ch, u, new AsyncCallback<Boolean>() {
			@Override
			public void onSuccess(Boolean result) {
				if (result){
					m_userChannels.addRow(ch);
				}
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("subscribe : " + caught.getMessage());
			}
		});
		
	}
	
	private void unsubscribe(){
		final WUser u = GTState.Instanse().getCurUser();
		final WChannel ch = m_userChannels.getSelectedObject();
		if (null == u || ch == null){
			return;
		}
		
		GTService.Util.getInstance().unsubscribe(ch, u, new AsyncCallback<Boolean>() {
			@Override
			public void onSuccess(Boolean result) {
				if (result){
					m_userChannels.deleteRow(m_userChannels.getSelectedRow());
				}
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("unsubscribe : " + caught.getMessage());
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
	
	private void refreshUserChannels(final Boolean toRefreshAvaiChannels){
		m_userChannels.erase();
		final WUser u = GTState.Instanse().getCurUser();
		if (null == u){
			return;
		}
		
		final GTServiceAsync service = GTService.Util.getInstance();
		service.subscribedChannels(u, new AsyncCallback<List<WChannel>>() {
			@Override
			public void onSuccess(List<WChannel> result) {
				m_userChannels.addRows(result);
				if (toRefreshAvaiChannels)
					refreshAvaiChannels(false);
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("loadUserChannels : " + caught.getMessage());
				if (toRefreshAvaiChannels)
					refreshAvaiChannels(false);
			}
		});
	}
	
	private void refreshAvaiChannels(final Boolean toRefreshUserChannels){
		m_avalChannels.erase();
		final WUser u = GTState.Instanse().getCurUser();
		if (null == u){
			return;
		}
		
		final GTServiceAsync service = GTService.Util.getInstance();
		service.availableChannels(u, new AsyncCallback<List<WChannel>>() {
			@Override
			public void onSuccess(List<WChannel> result) {
				m_avalChannels.addRows(result);
				if (toRefreshUserChannels)
					refreshUserChannels(false);				
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("loadUserChannels : " + caught.getMessage());
				if (toRefreshUserChannels)
					refreshUserChannels(false);
			}
		});
	}
	
	
	public static Channels Instance(){
		if(instance == null){
			instance = new Channels();
		}
		instance.resume();
		return instance;
	}
	private static Channels instance;
	private Channels(){
		super();
	};

}
