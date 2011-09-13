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
			labels.add(UIUtil.constructLabel(LOC.password() + ": Test", 18));	
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
