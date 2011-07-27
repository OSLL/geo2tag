package ru.spb.osll.web.client.ui.core;

import java.util.List;

import ru.spb.osll.web.client.GTShell;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.MenuItem;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.ui.Anchor;
import com.google.gwt.user.client.ui.Composite;
import com.google.gwt.user.client.ui.DecoratedStackPanel;
import com.google.gwt.user.client.ui.HTML;
import com.google.gwt.user.client.ui.HasVerticalAlignment;
import com.google.gwt.user.client.ui.HorizontalPanel;
import com.google.gwt.user.client.ui.Image;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;

abstract public class SimpleMenu <T extends SimpleComposite> extends Composite {

	private SimpleMenuTree<T> m_menuTree 
		= new SimpleMenuTree<T>();
	
	public SimpleMenu() {
		initMenu();
		initWidget(onInitialize());
	}
	
	public SimpleMenuTree<T> getMenuTree(){
		return m_menuTree;
	}

	abstract protected void initMenu();

	public Widget onInitialize() {
		DecoratedStackPanel stackPanel = new DecoratedStackPanel();
		stackPanel.setWidth("224px");
		
		for (GroupItem<T> group : m_menuTree.getGroups()){
			final boolean containtPicture = group.getImage() != null;
			final String header = containtPicture ? 
				getHeaderString(group.getName(), group.getImage()): group.getName();
			stackPanel.add(createItemsWidget(group.getItems()), header, containtPicture);
		}
		stackPanel.ensureDebugId("cwStackPanel");
		return stackPanel;
	}	
		
	private String getHeaderString(String text, ImageResource image) {
		HorizontalPanel hPanel = new HorizontalPanel();
		hPanel.setSpacing(6);
		hPanel.setVerticalAlignment(HasVerticalAlignment.ALIGN_MIDDLE);
		if (image != null){
			hPanel.add(new Image(image));
		}
		HTML headerText = new HTML(text);
		headerText.setStyleName("cw-StackPanelHeader");
		hPanel.add(headerText);

		// Return the HTML string for the panel
		return hPanel.getElement().getString();
	}
	
	private VerticalPanel createItemsWidget(List<MenuItem<T>> items) {
		VerticalPanel panel = new VerticalPanel();
		for (MenuItem<T> item : items) {
			final Widget w = item.getWidget();
			ClickHandler handler = new ClickHandler() {
				public void onClick(ClickEvent event) {
					GTShell shell = GTShell.Instance;
					if (shell != null){
						shell.setContent(w);
					}
				}
			};
			
			final HorizontalPanel imlink = new HorizontalPanel();
			imlink.setSpacing(6);
			
			if (item.getImage() != null){
				final Image im = new Image(item.getImage());
				im.addClickHandler(handler);
				imlink.add(im);
			}
			final Anchor link = new Anchor(item.getName());
			link.addClickHandler(handler);
			imlink.add(link);
			
			panel.add(imlink);
		}
		return panel;
	}

}
