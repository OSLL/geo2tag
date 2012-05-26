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
