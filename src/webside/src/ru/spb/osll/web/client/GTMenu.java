package ru.spb.osll.web.client;

import ru.spb.osll.web.client.localization.Localizer;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.Images;
import ru.spb.osll.web.client.ui.widgets.Channels;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;
import ru.spb.osll.web.client.ui.widgets.OSMWidget;
import ru.spb.osll.web.client.ui.widgets.RegistrationWidget;
import ru.spb.osll.web.client.ui.widgets.TagsMapWidget;
import ru.spb.osll.web.client.ui.widgets.TagsTableWidget;

import com.google.gwt.user.client.ui.Widget;

public class GTMenu extends SimpleMenu {

	@Override
	protected void initMenu() {
		final SimpleMenuTree menu = getMenuTree();
		GroupItem groupItem;
		String group;
		// group1
		{
			group = Localizer.res().menuGrServices();
			groupItem = menu.getGroupMenuItem(group, Images.res().folder());
			groupItem.addMenuItem(Channels.Instance());
			groupItem.addMenuItem(TagsTableWidget.Instance());
			groupItem.addMenuItem(TagsMapWidget.Instance());
		}

		// group1
		{
			group = Localizer.res().menuGrForUser();
			groupItem = menu.getGroupMenuItem(group, Images.res().folder());
			groupItem.addMenuItem(LoginWidget.Instance());
			groupItem.addMenuItem(RegistrationWidget.Instance());
			groupItem.addMenuItem(new OSMWidget());
		}

	}

	@Override
	protected void setContentWidget(Widget widget) {
		GTShell shell = GTShell.Instance;
		if (shell != null){
			shell.setContent(widget);
		}
	}
}