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

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.resources.client.ImageResource;

public class SimpleMenuTree {
		
	private List<GroupItem> m_grops = new ArrayList<GroupItem>();

	public GroupItem getGroupMenuItem(final String name){
		return getGroupMenuItem(name, null);
	}
	
	public GroupItem getGroupMenuItem(final String name, final ImageResource im){
		GroupItem groupItem = new GroupItem(name, im);
		m_grops.add(groupItem);
		return groupItem;
	}
	
	public List<GroupItem> getGroups(){
		return m_grops;
	}

	private static class Item {
		private final String m_name;
		private final ImageResource m_image;
		
		Item(String name, ImageResource im){
			m_name = name;
			m_image = im;
		}
		
		public String getName(){
			return m_name;
		}
		
		public ImageResource getImage(){
			return m_image;
		}
	}
	
	public static class GroupItem extends Item{
		private List<MenuItem> m_items = new ArrayList<MenuItem>();
		private List<MenuAction> m_actions = new ArrayList<MenuAction>();
		
		public GroupItem (final String name, final ImageResource im){
			super(name, im);
		}
		public void addMenuItem(SimpleComposite w){
			m_items.add(new MenuItem(w.getName(), w.getImage(), w));
		}
		public List<MenuItem> getItems(){
			return m_items;
		}

		public void addMenuAction(final String name, final ImageResource im, Runnable a){
			m_actions.add(new MenuAction(name, im, a));
		}
		public void addMenuAction(MenuAction menuAction){
			m_actions.add(menuAction);
		}
		public List<MenuAction> getActions(){
			return m_actions;
		}
	}

	public static class MenuItem extends Item {
		private SimpleComposite m_widget; 

		public MenuItem(final String name, final ImageResource im, SimpleComposite w) {
			super(name, im);
			m_widget = w;
		}
		
		public SimpleComposite getWidget(){
			return m_widget;
		}
	}
	
	public static class MenuAction extends Item {
		private Runnable m_action; 

		public MenuAction(final String name, final ImageResource im, Runnable a) {
			super(name, im);
			m_action = a;
		}
		
		public Runnable getAction(){
			return m_action;
		}
	}

}

