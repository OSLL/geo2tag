package ru.spb.osll.web.client.ui.core;

import java.util.List;

import com.google.gwt.user.client.ui.ListBox;

public class SmartListBox<T> extends ListBox {
	
	private IsDataAccessor<T> m_accessor; 
	
	private List<T> m_data;

	private int m_showAll = 0;
	
	public SmartListBox(IsDataAccessor<T> acc){
		super(false);
		m_accessor = acc;
	}

	public void setData(List<T> data){
		setData(data, null);
	}

	public void setData(List<T> data, String allTitle){
		clear();

		if (allTitle != null){
			m_showAll =  1;
			addItem(allTitle); 
		}

		m_data = data;
		for (T d : data){
			addItem(m_accessor.extractData(d));
		}
	}
	
	public Object getSelectedObject(){
		if (getSelectedIndex() < 0){
			return null;
		}
		if (getSelectedIndex() == 0 && m_showAll == 1){
			return m_data;
		}
		final int idx = getSelectedIndex() - m_showAll;
		return m_data != null ? m_data.get(idx) : null;
	}	
}
