package ru.spb.osll.web.client.ui.core;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.resources.client.ImageResource;

/*
 * T Type of leaf
 */
public class SimpleMenuTree <T extends SimpleComposite> {
		
	private List<GroupItem<T>> m_grops = new ArrayList<GroupItem<T>>();

	public GroupItem<T> getGroupMenuItem(final String name){
		return getGroupMenuItem(name, null);
	}
	
	public GroupItem<T> getGroupMenuItem(final String name, final ImageResource im){
		GroupItem<T> groupItem = new GroupItem<T>(name, im);
		m_grops.add(groupItem);
		return groupItem;
	}
	
	public List<GroupItem<T>> getGroups(){
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
	
	public static class GroupItem<T extends SimpleComposite> extends Item{
		private List<MenuItem<T>> m_items = new ArrayList<MenuItem<T>>();
		
		public GroupItem (final String name, final ImageResource im){
			super(name, im);
		}
		
		public void addMenuItem(T w){
			m_items.add(new MenuItem<T>(w.getName(), w.getImage(), w));
		}
		public List<MenuItem<T>> getItems(){
			return m_items;
		}
	}

	public static class MenuItem <T extends SimpleComposite> extends Item {
		private T m_widget; 

		public MenuItem(final String name, final ImageResource im, T w) {
			super(name, im);
			m_widget = w;
		}
		
		public T getWidget(){
			return m_widget;
		}
	}
}

