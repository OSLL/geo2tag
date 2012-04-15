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

import ru.spb.osll.web.client.localization.GTLocalization;
import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleComposite extends Composite {
	protected GTLocalization LOC;
	private Label m_specMess;
	
	public SimpleComposite(){
		LOC = Localizer.res();
		m_specMess = UIUtil.constructLabel(16);
		m_specMess.setStyleName("warning-label-style");
		m_specMess.setVisible(false);
		
		VerticalPanel cotainer = UIUtil.getVerticalPanel(true);
		cotainer.add(m_specMess);
		cotainer.add(onInitialize());
		initWidget(cotainer);
	}

	public void resume(){
		onResume();
	}
	
	abstract protected String getName();

	abstract protected Widget onInitialize();

	protected void onResume(){}

	protected ImageResource getImage() {
		return Images.res().peek();
	}
	
	public void showWarningMessage(String mess){
		m_specMess.setText(mess);
		m_specMess.setVisible(true);
	}

	public void hideWarningMessage(){
		m_specMess.setVisible(false);
	}
	
}
