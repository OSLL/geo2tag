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

package ru.spb.osll.web.client.ui.core;

import java.util.List;

import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.MenuAction;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.MenuItem;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.DecoratedStackPanel;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.HasVerticalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleMenu extends Composite {

	private SimpleMenuTree m_menuTree = new SimpleMenuTree();
	
	private DecoratedStackPanel m_stackPanel;

	abstract protected void initMenu();

	abstract protected void setContentWidget(Widget widget);
	
	public SimpleMenu() {
		initMenu();
		initWidget(onInitialize());
	}
	
	public SimpleMenuTree getMenuTree(){
		return m_menuTree;
	}

	public void setSecectedGroup(Widget w){
		int index = m_stackPanel.getSelectedIndex();
		final List<GroupItem> groups = m_menuTree.getGroups();
		for (int i = 0; i < groups.size(); i++){
			for (MenuItem item : groups.get(i).getItems()){
				if (item.getWidget() == w){
					index = i;
				}
			}
		}
		m_stackPanel.showStack(index);
	}

	public String getGroup(Widget w){ 
		final List<GroupItem> groups = m_menuTree.getGroups();
		for (GroupItem group : groups){
			for (MenuItem item : group.getItems()){
				if (item.getWidget() == w){
					return group.getName();
				}
			}
		}
		return null;
	}

	// FIXME common part
	public void setVisibleWidget(Widget w, boolean visible){
		boolean isFound = false;
		int idxGroup = 0;
		int idxItem = 0;
		final List<GroupItem> groups = m_menuTree.getGroups();
		for (int i = 0; i < groups.size(); i++){
			GroupItem group = groups.get(i);
			for (int j = 0; j < group.getItems().size(); j++){
				MenuItem item = group.getItems().get(j);
				if (item.getWidget() == w){
					isFound = true;
					idxGroup = i;
					idxItem =  j;
					break;
				}
			}
		}

		//GWT.log("disableWidget " + isFound + " " + idxGroup + " " + idxItem);
		if (isFound){
			VerticalPanel vp = (VerticalPanel) m_stackPanel.getWidget(idxGroup);
			vp.getWidget(idxItem).setVisible(visible);
		}
	}

	public Widget onInitialize() {
		m_stackPanel = new DecoratedStackPanel();
		m_stackPanel.setWidth("224px");
		for (GroupItem group : m_menuTree.getGroups()){
			final boolean containtPicture = group.getImage() != null;
			final String header = containtPicture ? 
					getHeaderString(group.getName(), group.getImage()): group.getName();
			m_stackPanel.add(createItemsWidget(group), header, containtPicture);
		}
		m_stackPanel.ensureDebugId("stackPanel");
		return m_stackPanel;
	}	
		
	private String getHeaderString(String text, ImageResource image) {
		HorizontalPanel hPanel = new HorizontalPanel();
		hPanel.setSpacing(6);
		hPanel.setVerticalAlignment(HasVerticalAlignment.ALIGN_MIDDLE);
		if (image != null){
			hPanel.add(new Image(image));
		}
		HTML headerText = new HTML(text);
		hPanel.add(headerText);
		return hPanel.getElement().getString();
	}
	
	private VerticalPanel createItemsWidget(GroupItem group) {
		VerticalPanel panel = new VerticalPanel();
		for (MenuItem item : group.getItems()) {
			final SimpleComposite widget = item.getWidget();
			ClickHandler handler = new ClickHandler() {
				public void onClick(ClickEvent event) {
					widget.resume();
					setContentWidget(widget);
				}
			};
			
			final HorizontalPanel imlink = new HorizontalPanel();
			imlink.setSpacing(6);

			if (item.getImage() != null){
				final Image im = new Image(item.getImage());
				im.addClickHandler(handler);
				imlink.add(im);
			}
			final Anchor link = new Anchor(item.getName());
			link.addClickHandler(handler);
			imlink.add(link);
			
			panel.add(imlink);
		}
		
		
		class MenuHandler implements ClickHandler {
			private MenuAction m_action;
			private MenuHandler(MenuAction action){
				m_action = action;
			}
			@Override
			public void onClick(ClickEvent event) {
				m_action.getAction().run();
			}
		}
		
		// MenuActions
		for (MenuAction action : group.getActions()) {
			final HorizontalPanel imlink = new HorizontalPanel();
			imlink.setSpacing(6);

			if (action.getImage() != null){
				final Image im = new Image(action.getImage());
				im.addClickHandler(new MenuHandler(action));
				imlink.add(im);
			}
			final Anchor link = new Anchor(action.getName());
			link.addClickHandler(new MenuHandler(action));
			imlink.add(link);
			
			panel.add(imlink);
		}
		
		return panel;
	}
}
