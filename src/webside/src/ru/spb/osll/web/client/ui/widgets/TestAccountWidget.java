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

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.StubWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.Widget;

public class TestAccountWidget extends SimpleComposite {

	@Override
	protected String getName() {
		return LOC.TestAccountWidget();
	}

	@Override
	protected Widget onInitialize() {
		final TestAccountView v = new TestAccountView();
		// ??? 
//		UserStateListener l = new UserStateListener() {
//			@Override
//			public void onUserChange(User u) {
//				GWT.log("onUserChange");
//				v.setLinksVisible(u == null);
//			}
//		};
//		GTState.Instanse().addOnUserStateListerer(l);
//		GTState.Instanse().checkAuth();
		return v;
	}
	
	public static TestAccountWidget Instance(){
		if (instance == null){
			instance = new TestAccountWidget();
		}
		instance.resume();
		return instance;
	}
	private static TestAccountWidget instance;
	private TestAccountWidget(){
		super();
	}
	
	private class TestAccountView extends StubWidget {
		@Override
		protected ImageResource getImage() {
			return Images.res().testing();
		}

		@Override
		protected List<Label> getLabels() {
			List<Label> labels = new ArrayList<Label>();
			labels.add(UIUtil.constructLabel(LOC.TestAccountWidget(), 20));
			labels.add(UIUtil.constructLabel(LOC.login() + ": Paul", 18));		
			labels.add(UIUtil.constructLabel(LOC.password() + ": test", 18));	
			return labels;
		}

		@Override
		protected List<Anchor> getLinks() {
			Anchor loginLink = new Anchor(LOC.login());
			loginLink.addClickHandler(new ClickHandler() {
				@Override
				public void onClick(ClickEvent event) {
					GTShell.Instance.setContent(LoginWidget.Instance());
				}
			});
			List<Anchor> links = super.getLinks();
			links.add(loginLink);
			return links;
		}
		
		public void setLinksVisible(boolean visible){
			for(Anchor link : getLinks()){
				link.setVisible(visible);
			}
		}
	}
}
