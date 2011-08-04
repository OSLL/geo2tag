package ru.spb.osll.web.client.ui.core;

import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.MenuItem;

import com.google.gwt.event.dom.client.ClickEvent;
import com.google.gwt.event.dom.client.ClickHandler;
import com.google.gwt.event.logical.shared.ValueChangeEvent;
import com.google.gwt.event.logical.shared.ValueChangeHandler;
import com.google.gwt.resources.client.ImageResource;
import com.google.gwt.user.client.History;
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

	private SimpleMenuTree<T> m_menuTree = new SimpleMenuTree<T>();
	
	public SimpleMenu() {
		initMenu();
		initWidget(onInitialize());
	}
	
	public SimpleMenuTree<T> getMenuTree(){
		return m_menuTree;
	}

	abstract protected void initMenu();

	abstract protected boolean setContentWidget(T widget);
	
	protected T getDefaultWidget(){
		return null;
	}

	protected int getDefaultGroup(){
		return 0;
	}
	
	public Widget onInitialize() {
		final DecoratedStackPanel stackPanel = new DecoratedStackPanel();
		stackPanel.setWidth("224px");
		for (GroupItem<T> group : m_menuTree.getGroups()){
			final boolean containtPicture = group.getImage() != null;
			final String header = containtPicture ? 
					getHeaderString(group.getName(), group.getImage()): group.getName();
			stackPanel.add(createItemsWidget(group), header, containtPicture);
		}
		stackPanel.ensureDebugId("stackPanel");

		History.addValueChangeHandler(new ValueChangeHandler<String>() {
			public void onValueChange(ValueChangeEvent<String> event) {
				final String token = event.getValue();
				final Pair pair = getWidgetByToken(token);
				final int group = pair.left;
				final T widget = pair.right;

				if (setContentWidget(widget)){
					stackPanel.showStack(group);
				}
			}
		});
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
		hPanel.add(headerText);

		// Return the HTML string for the panel
		return hPanel.getElement().getString();
	}
	
	private VerticalPanel createItemsWidget(GroupItem<T> group) {
		VerticalPanel panel = new VerticalPanel();
		for (MenuItem<T> item : group.getItems()) {
			final T widget = item.getWidget();
			final String token = getToken(group, widget);
			
			ClickHandler handler = new ClickHandler() {
				public void onClick(ClickEvent event) {
					if (setContentWidget(widget)){
						History.newItem(token);
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
	
	private Pair getWidgetByToken(String token){
		Pair defaultPair = new Pair(getDefaultGroup(), getDefaultWidget());
		if (token.equals("")){
			return defaultPair;
		}
		int groupIndex = 0;
		for(GroupItem<T> group : m_menuTree.getGroups()){
			for(MenuItem<T> item : group.getItems()){
				final T widget = item.getWidget();
				if (getToken(group, widget).contains(token)){
					return new Pair(groupIndex, widget);
				}
			}
			groupIndex++;
		}
		return defaultPair;
	}

	public String getToken(GroupItem<T> group, T content) {
		return group.getName() + "_" + getTokenByClass(content.getClass());
	}
	
	public static <T>String getTokenByClass(Class<T> cwClass) {
		String className = cwClass.getName();
		className = className.substring(className.lastIndexOf('.') + 1);
		return className;
	}
	
	private class Pair{
		final int left;
		final T right;

		Pair(int left, T right) {
			this.left = left;
			this.right = right;
		}
	}
}
