package ru.spb.osll.web.client;

import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.Images;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;
import ru.spb.osll.web.client.ui.widgets.RegistrationWidget;
import ru.spb.osll.web.client.ui.widgets.TestRPC;
import ru.spb.osll.web.client.ui.widgets.WidgetByDesigner;

public class GTMenu extends SimpleMenu<SimpleComposite> {

	@Override
	protected void initMenu() {
		final SimpleMenuTree<SimpleComposite> menu = getMenuTree();
		GroupItem<SimpleComposite> groupItem ; 
		
		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 1", Images.res().folder());
			groupItem.addMenuItem(new LoginWidget());
			groupItem.addMenuItem(new RegistrationWidget());
		}

		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 2", Images.res().folder());
			groupItem.addMenuItem(new TestRPC());
			groupItem.addMenuItem(new TestRPC());
		}
		
	}

	@Override
	protected boolean setContentWidget(SimpleComposite widget) {
		GTShell shell = GTShell.Instance;
		if (shell != null){
			shell.setContent(widget);
			return true;
		}
		return false;
	}
}
