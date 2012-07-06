package ru.spb.osll.web.client;

import com.google.gwt.user.client.ui.Widget;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SiteMap;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.Images;
import ru.spb.osll.web.client.ui.widgets.*;

public class GTSiteMap extends SiteMap {

	@Override
	protected void initSiteMap() {
		addWidget(HomePage.Instance());
		addWidget(TestAccountWidget.Instance());
		addWidget(Channels.Instance());
		addWidget(TagsTableWidget.Instance());
		addWidget(TagsMapWidget.Instance());
		addWidget(LoginWidget.Instance());
		addWidget(RegistrationWidget.Instance());
	}
	
	@Override
	protected SimpleMenu createMenu() {
		return new SimpleMenu() {

			@Override
			protected void initMenu() {
				final SimpleMenuTree menu = getMenuTree();
				GroupItem groupItem;
				String group;
				{
					group = Localizer.res().menuGrServices();
					groupItem = menu.getGroupMenuItem(group, Images.res().folder());
					groupItem.addMenuItem(HomePage.Instance());
					groupItem.addMenuItem(TestAccountWidget.Instance());
					groupItem.addMenuItem(Channels.Instance());
					groupItem.addMenuItem(TagsTableWidget.Instance());
					groupItem.addMenuItem(TagsMapWidget.Instance());
				}

				{
					group = Localizer.res().menuGrForUser();
					groupItem = menu.getGroupMenuItem(group, Images.res().folder());
					groupItem.addMenuItem(LoginWidget.Instance());
					groupItem.addMenuItem(RegistrationWidget.Instance());
				}				
			}

			@Override
			protected void setContentWidget(Widget widget) {
				GTShell shell = GTShell.Instance;
				if (shell != null){
					shell.setContent(widget);
				}				
			}
			
		};
	}
	
}
