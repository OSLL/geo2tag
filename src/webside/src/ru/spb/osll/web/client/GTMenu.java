package ru.spb.osll.web.client;

import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.ResImages;
import ru.spb.osll.web.client.ui.widgets.TestRPC;
import ru.spb.osll.web.client.ui.widgets.WidgetByDesigner;

import com.google.gwt.core.client.GWT;

public class GTMenu extends SimpleMenu<SimpleComposite> {

	public interface Images extends ResImages {
	}

	@Override
	protected void initMenu() {
		Images images = (Images) GWT.create(Images.class);
		final SimpleMenuTree<SimpleComposite> menu = getMenuTree();
		GroupItem<SimpleComposite> groupItem ; 
		
		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 1", images.folder());
			groupItem.addMenuItem(new WidgetByDesigner());
			groupItem.addMenuItem(new WidgetByDesigner());
		}

		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 2", images.folder());
			groupItem.addMenuItem(new TestRPC());
			groupItem.addMenuItem(new TestRPC());
		}
		
	}
}
