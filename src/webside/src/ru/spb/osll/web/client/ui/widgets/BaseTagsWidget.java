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

import ru.spb.osll.web.client.GTState;
import ru.spb.osll.web.client.services.GTService;
import ru.spb.osll.web.client.services.objects.WChannel;
import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.services.objects.WUser;
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
	private SmartListBox<Integer>  m_amountBox;
//	private RadioButton m_radioBtnAll;		
//    private RadioButton m_radioBtnMy;
	
    protected abstract TagsView getTagsView();
    
	@Override
	protected Widget onInitialize() {
		m_tagsView = getTagsView();
		VerticalPanel contaier = UIUtil.getVerticalPanel();
		contaier.setSpacing(10);
		contaier.setWidth("100%");
		initFiltersPanel(contaier);
		contaier.add(m_tagsView);
		m_channelBox.setEnabled(true);
		return contaier;
	}
    
	@Override
	public void onResume() {
		final WUser user = GTState.Instanse().getCurUser();
		showWarningMessage(user);
		refreshFilterPanel(user);
		m_tagsView.refresh();
		m_channelBox.setEnabled(true);
	}

	private void showWarningMessage(WUser u){
		if (u != null){
			hideWarningMessage();
		} else {
			showWarningMessage(LOC.wrngNotLogged());
		}
	}
	
	private void initFiltersPanel(VerticalPanel container){
	    m_channelBox = new SmartListBox<WChannel>(Accessors.CHANNEL_ACC_NAME);
	    m_channelBox.setWidth("200px");
	    
	    m_amountBox = new SmartListBox<Integer>(Accessors.INTEGRE_ACC);
	    m_amountBox.setWidth("40px");
	    List<Integer> amountData = new ArrayList<Integer>();
	    amountData.add(10);
	    amountData.add(20);
	    amountData.add(30);
	    amountData.add(40);
	    amountData.add(50);
	    m_amountBox.setData(amountData);
        	    
        m_channelBox.setEnabled(true);
        m_amountBox.setEnabled(true);
	    
//	    final String uniqueId = UIUtil.getUniqueId("channle.type"); 
//	    m_radioBtnAll = new RadioButton(uniqueId, LOC.radioBtnInChannel(), false);
//	    m_radioBtnAll.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
//			@Override
//			public void onValueChange(ValueChangeEvent<Boolean> event) {
//				m_channelBox.setEnabled(event.getValue());
//				m_amountBox.setEnabled(event.getValue());
//			}
//		});
//	    m_radioBtnMy = new RadioButton(uniqueId, LOC.radioBtnMy(), false);
//	    m_radioBtnMy.addValueChangeHandler(new ValueChangeHandler<Boolean>() {
//			@Override
//			public void onValueChange(ValueChangeEvent<Boolean> event) {
//				m_channelBox.setEnabled(!event.getValue());
//				m_amountBox.setEnabled(!event.getValue());
//			}
//		});
	    
	    Button refreshBtn = new Button(LOC.btnRefresh());
	    refreshBtn.addClickHandler(new ClickHandler() {
			@Override
			public void onClick(ClickEvent event) {
//				if (m_radioBtnAll.getValue()){
//					refreshByChannel();
//				} else {
//					refreshByUser();
//				}
			    refreshByChannel();
			}
		});

        refreshByChannel();
        
	    Widget widgets[] = {
//	    		new Label("Tags"),
//	    		m_radioBtnAll,
//	    		m_radioBtnMy,
	    		cosntuctPair("Channel", m_channelBox),
	    		cosntuctPair("Amount", m_amountBox),
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
	    System.out.println("called refreshByUser()");
		final WUser u = GTState.Instanse().getCurUser();
        double radius = Double.MAX_VALUE;
		double latitude = 60.166504;
		double longitude = 24.841204;
		loadTags(u, latitude, longitude, radius);		
		m_tagsView.refresh();
	}
	
	private void refreshByChannel(){
		final WUser u = GTState.Instanse().getCurUser();
		final Object obj = m_channelBox.getSelectedObject();
        final int amount = (Integer) m_amountBox.getSelectedObject();
		
		if (obj == null) {
		    return;
		} else if (obj instanceof WChannel) {
			final WChannel ch = (WChannel) obj;
			loadTags(u, ch, amount);
		} else if (obj instanceof List<?>) {
            @SuppressWarnings("unchecked")
            final List<WChannel> chs = (List<WChannel>)obj;
            loadTags(u, chs, amount);
		}
		m_tagsView.refresh();
	}

	private void loadTags(WUser u, double latitude, double longitude, double radius)
	{
	    System.out.println("called loadTags(WUser u, double latitude, double longitude, double radius)");
		GTService.Util.getInstance().getTags(u, latitude, longitude, radius, 
			new AsyncCallback<List<WMark>>() {
				@Override
				public void onSuccess(List<WMark> result) {
					m_tagsView.setTags(result);
				}
				@Override
				public void onFailure(Throwable caught) {
					GWT.log("loadTags", caught);
				}			
			}
		);
	}
		
	private void loadTags(WUser u, WChannel ch, int amount)
	{
		GTService.Util.getInstance().getTags(u, ch, amount, 
			new AsyncCallback<List<WMark>>() {
				@Override
				public void onSuccess(List<WMark> result) {
					m_tagsView.setTags(result);
				}
				@Override
				public void onFailure(Throwable caught) {
					GWT.log("loadTags", caught);
				}			
			}
		);
	}
	
    private void loadTags(WUser u, List<WChannel> chs, int amount) {
        GTService.Util.getInstance().getTags(u, chs, amount,
                new AsyncCallback<List<WMark>>() {
                    @Override
                    public void onSuccess(List<WMark> result) {
                        m_tagsView.setTags(result);
                    }

                    @Override
                    public void onFailure(Throwable caught) {
                        GWT.log("loadTags", caught);
                    }
                });
    }
	
	private void refreshFilterPanel(final WUser u){
		m_channelBox.clear();
		if (u == null){
			return;
		}
		AsyncCallback<List<WChannel>> callback = new AsyncCallback<List<WChannel>>() {
			@Override
			public void onSuccess(List<WChannel> result) {
				m_channelBox.setData(result, LOC.itemAllChannels());
				refreshByUser();
//				if (result.size() > 0){
//					m_radioBtnAll.setValue(true);
//					m_radioBtnMy.setValue(false);
//					refreshByChannel();
//				} else {
//					m_radioBtnMy.setValue(true);
//					m_radioBtnAll.setValue(false);
//					refreshByUser();
//				}
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("fillChannelBox", caught);
			}
		};
		GTService.Util.getInstance().subscribedChannels(u, callback);
	}
	
	public interface TagsView extends IsWidget {
		public void setTags(List<WMark> tags);
		public void refresh();
	}
	
	/*
	 * it will be used with flat shape filtration
	 */
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

