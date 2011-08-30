package ru.spb.osll.web.client.ui.core;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.FlexTable;

public class TableWidget<T> extends FlexTable implements ClickHandler {
	private String m_headerStyle;
	private String m_selectedStyle;
	private int m_selectedRow;
	private List<TableField<T>> m_fields;
	private List<T> m_data = new ArrayList<T>();

	public TableWidget(List<TableField<T>> fields) {
		this(fields, null);
	}

	public TableWidget(List<TableField<T>> fields, List<T> data) {
		super();
		setBorderWidth(1);
		setCellPadding(1);
		setCellSpacing(0);
		setWidth("100%");
		setStyleName("gtTable");
		m_selectedStyle = "gtTable-selected";
		m_headerStyle = "gtTable-header";
		addClickHandler(this);
		
		m_fields = fields;
		initHeaders();
		
		if (data != null){
			addRows(data);
		}
	}
	
	public void setHeaderStyle(String style){
		m_headerStyle = style;
	}
	
	public void setSelectedStyle(String style){
		m_selectedStyle = style;
	}
	
	public int getSelectedRow(){
		return m_selectedRow - 1;
	}

	public T getSelectedObject(){
		final int index = getSelectedRow();
		return index >= 0 ? m_data.get(index) : null;
	}
	
	@Override
	public void onClick(ClickEvent event) {
		final int row = getCellForEvent(event).getRowIndex();
		getRowFormatter().removeStyleName(m_selectedRow, m_selectedStyle);
		if (row > 0) {
			this.getRowFormatter().addStyleName(row, m_selectedStyle);
			m_selectedRow = row;
		}
	}
	
	public void addRow(T obj) {
		m_data.add(obj);
		int row = getRowCount();
		for (int i = 0; i < m_fields.size(); i++) {
			final String s = m_fields.get(i).getAccessor().extractData(obj);
			setText(row, i, s);
		}
	}

	public void addRows(List<T> objs) {
		for (T obj : objs){
			addRow(obj);
		}
	}
	
	public void deleteRow(int row) {
		m_data.remove(row);
		row += 1;
		if (row < getRowCount()){
			super.removeRow(row);
		}

		m_selectedRow--;
		if (m_selectedRow > 0){
			getRowFormatter().addStyleName(m_selectedRow, m_selectedStyle);
		}
	}

	public void erase(){
		removeAllRows();
		initHeaders();
	}
	
	private void initHeaders(){
		for(int i = 0; i < m_fields.size(); i++){
			setText(0, i, m_fields.get(i).getTitle());
		}
		getRowFormatter().addStyleName(0, m_headerStyle);
	}

	public boolean contains(T obj) {
		return m_data.contains(obj);
	}
	
	public static class TableField<T> {
		private final String m_id;
		private final String m_title;
		private IsDataAccessor<T> m_accessor;

		public TableField(String id, String title, IsDataAccessor<T> acc) {
			m_id = id;
			m_title = title;
			m_accessor = acc;
		}

		public String getId() {
			return m_id;
		}

		public String getTitle() {
			return m_title;
		}

		public IsDataAccessor<T> getAccessor() {
			return m_accessor;
		}
	}
}