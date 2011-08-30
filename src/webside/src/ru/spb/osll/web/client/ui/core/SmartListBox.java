package ru.spb.osll.web.client.ui.core;

import java.util.List;

import com.google.gwt.user.client.ui.ListBox;

public class SmartListBox<T> extends ListBox {
	
	private IsDataAccessor<T> m_accessor; 
	
	private List<T> m_data;
	
	public SmartListBox(IsDataAccessor<T> acc, boolean isMultipleSelect){
		super(isMultipleSelect);
		m_accessor = acc;
	}
	
	public void setData(List<T> data){
		clear();
		m_data = data;
		for (T d : data){
			addItem(m_accessor.extractData(d));
		}
	}
	
	public T getSelectedObject(){
		if (getSelectedIndex() < 0){
			return null;
		}
		return m_data != null ? m_data.get(getSelectedIndex()) : null;
	}	
}
