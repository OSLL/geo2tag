package ru.spb.osll.web.client;

import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.Images;
import ru.spb.osll.web.client.ui.widgets.Channels;
import ru.spb.osll.web.client.ui.widgets.HomePage;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;
import ru.spb.osll.web.client.ui.widgets.RegistrationWidget;
import ru.spb.osll.web.client.ui.widgets.TestRPC;

public class GTMenu extends SimpleMenu {

	@Override
	protected void initMenu() {
		final SimpleMenuTree menu = getMenuTree();
		GroupItem groupItem ; 

		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 1", Images.res().folder());
			groupItem.addMenuItem(new Channels());
			groupItem.addMenuItem(HomePage.Instance());
			groupItem.addMenuItem(LoginWidget.Instance());
			groupItem.addMenuItem(RegistrationWidget.Instance());
		}

		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 2", Images.res().folder());
			groupItem.addMenuItem(new TestRPC());
			groupItem.addMenuItem(new TestRPC());
		}

	}
	
	@Override
	protected void setContentWidget(SimpleComposite widget) {
		GTShell shell = GTShell.Instance;
		if (shell != null){
			shell.setContent(widget);
		}
	}
}
