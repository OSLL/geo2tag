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
		
		public GroupItem (final String name, final ImageResource im){
			super(name, im);
		}
		
		public void addMenuItem(SimpleComposite w){
			m_items.add(new MenuItem(w.getName(), w.getImage(), w));
		}
		public List<MenuItem> getItems(){
			return m_items;
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
}

