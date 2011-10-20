package ru.spb.osll.web.client.ui.widgets;

import java.util.ArrayList;
import java.util.List;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.services.objects.VersionInfo;
import ru.spb.osll.web.client.services.version.VersionService;
import ru.spb.osll.web.client.services.version.VersionServiceAsync;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.StubWidget;
import ru.spb.osll.web.client.ui.core.UIUtil;
import ru.spb.osll.web.client.ui.image.Images;

import com.google.gwt.core.client.GWT;
import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.rpc.AsyncCallback;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.Widget;

public class HomePage extends SimpleComposite {

	private CurVerionView m_verionView;
	
	@Override
	protected String getName() {
		return LOC.pageHome(); 
	}

	@Override
	protected Widget onInitialize() {
		m_verionView = new CurVerionView(); 
		return m_verionView; 
	}
	
	@Override
	protected void onResume() {
		super.onResume();
		
		final VersionServiceAsync service = VersionService.Util.getInstance();
		service.getVersionInfo(new AsyncCallback<VersionInfo>() {
			@Override
			public void onSuccess(VersionInfo result) {
				if (result != null){
					final String version = LOC.version() + result.getVersion();
					final String date = LOC.deployDate() + result.getDeployDate();
					m_verionView.setVersionMess(version);
					m_verionView.setDeployDateMess(date);
				}
			}
			@Override
			public void onFailure(Throwable caught) {
				GWT.log("getVersionInfo", caught);
			}
		});
	}

	public static HomePage Instance(){
		if (instance == null){
			instance = new HomePage();
		}
		instance.resume();
		return instance;
	}
	private static HomePage instance;
	private HomePage(){
		super();
	}
	
	private class CurVerionView extends StubWidget {
		private Label m_versionLabel;
		private Label m_deployDateLabel;
		
		public CurVerionView(){
			super("800px");
		}
		
		public void setVersionMess(String version){
			m_versionLabel.setText(version);
		}

		public void setDeployDateMess(String date){
			m_deployDateLabel.setText(date);
		}
		
		@Override
		protected ImageResource getImage() {
			return Images.res().globeLogo();
		}

		@Override
		protected List<Label> getLabels() {
			m_versionLabel = UIUtil.constructLabel(20);
			m_deployDateLabel = UIUtil.constructLabel(20);
			
			List<Label> labels = new ArrayList<Label>();
			labels.add(UIUtil.constructLabel(LOC.currentBuild(), 26)); 		
			labels.add(m_versionLabel);										
			labels.add(m_deployDateLabel);									
			return labels;
		}

		@Override
		protected List<Anchor> getLinks() {
			Anchor loginLink = new Anchor(LOC.TestAccountWidget());
			loginLink.addClickHandler(new ClickHandler() {
				@Override
				public void onClick(ClickEvent event) {
					GTShell.Instance.setContent(TestAccountWidget.Instance());
				}
			});
			List<Anchor> links = super.getLinks();
			links.add(loginLink);
			return links;
		}
	}
	
}
