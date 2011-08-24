package ru.spb.osll.web.client.ui.core;

import java.util.List;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.user.client.ui.FlexTable;

public class TableWidget<T> extends FlexTable implements ClickHandler {
	private String m_headerStyle;
	private String m_selectedStyle;
	private int m_selectedRow;
	private List<TableField<T>> m_fields;

	public TableWidget(List<TableField<T>> fields) {
		this(fields, null);
	}

	public TableWidget(List<TableField<T>> fields, List<T> data) {
		super();
		setBorderWidth(1);
		setCellPadding(1);
		setCellSpacing(0);
		setWidth("90%");
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
		int row = getRowCount();
		for (int i = 0; i < m_fields.size(); i++) {
			final String s = m_fields.get(i).getAccessor().toCell(obj);
			setText(row, i, s);
		}
	}

	public void addRows(List<T> objs) {
		for (T obj : objs){
			addRow(obj);
		}
	}
	
	public void deleteRow(int row) {
		row += 1;
		if (row < getRowCount()){
			super.removeRow(row);
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


	
	public static class TableField<T> {
		private final String m_id;
		private final String m_title;
		private IsTableAccessor<T> m_accessor;

		public TableField(String id, String title, IsTableAccessor<T> acc) {
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

		public IsTableAccessor<T> getAccessor() {
			return m_accessor;
		}

	}

	public static interface IsTableAccessor<T> {
		String toCell(T object);
	}
}