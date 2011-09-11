package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.Widget;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.StubWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
import ru.spb.osll.web.client.ui.image.Images;

public class TestAccountWidget extends SimpleComposite {

	@Override
	protected String getName() {
		return LOC.TestAccountWidget();
	}

	@Override
	protected Widget onInitialize() {
		return new TestAccountView();
	}
	
	public static TestAccountWidget Instance(){
		if (instance == null){
			instance = new TestAccountWidget();
		}
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
			labels.add(UIUtil.constructLabel("Test Account", 20));
			labels.add(UIUtil.constructLabel("Login: Paul", 18));
			labels.add(UIUtil.constructLabel("Password: Test", 18));
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
	}

}
