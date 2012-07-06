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

package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.services.objects.WMark;
import ru.spb.osll.web.client.ui.common.Fields;
import ru.spb.osll.web.client.ui.core.TableWidget;
import ru.spb.osll.web.client.ui.core.TableWidget.TableField;

import com.google.gwt.user.client.ui.Composite;

public class TagsTableWidget extends BaseTagsWidget {
	@Override
	protected String getName() {
		return LOC.TagsTableWidget();
	}

	@Override
	protected TagsView getTagsView() {
		return new TagsTablePanel();
	};
	
	public static TagsTableWidget Instance(){
		if(instance == null){
			instance = new TagsTableWidget();
		}
		instance.resume();
		return instance;
	}
	private static TagsTableWidget instance;
	private TagsTableWidget(){
		super();
	}

	private class TagsTablePanel extends Composite implements TagsView {
		private TableWidget<WMark> m_tagsTable;
		
		public TagsTablePanel(){
			List<TableField<WMark>> fields = new ArrayList<TableField<WMark>>();
			fields.add(Fields.TAG_FIELD_LABEL);
			fields.add(Fields.TAG_FIELD_DESC);
			fields.add(Fields.TAG_FIELD_URL);
			fields.add(Fields.TAG_FIELD_TIME);
			fields.add(Fields.TAG_FIELD_LAT);
			fields.add(Fields.TAG_FIELD_LON);
			m_tagsTable = new TableWidget<WMark>(fields);
			
			initWidget(m_tagsTable);
		}
		
		@Override
		public void setTags(List<WMark> tags) {
			m_tagsTable.erase();
			m_tagsTable.addRows(tags);
		}

		@Override
		public void refresh(){
			m_tagsTable.erase();
		}
	}
}
