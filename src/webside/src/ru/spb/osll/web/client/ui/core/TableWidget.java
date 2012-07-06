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
		m_data.clear();
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