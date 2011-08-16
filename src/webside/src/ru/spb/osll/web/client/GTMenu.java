package ru.spb.osll.web.client;

import com.google.gwt.user.client.rpc.AsyncCallback;

import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.client.services.users.LoginService;
import ru.spb.osll.web.client.services.users.LoginServiceAsync;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.Images;
import ru.spb.osll.web.client.ui.widgets.HomePage;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;
import ru.spb.osll.web.client.ui.widgets.RegistrationWidget;
import ru.spb.osll.web.client.ui.widgets.TestRPC;

public class GTMenu extends SimpleMenu<SimpleComposite> {

	@Override
	protected void initMenu() {
		final SimpleMenuTree<SimpleComposite> menu = getMenuTree();
		GroupItem<SimpleComposite> groupItem ; 
		
		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 1", Images.res().folder());
			groupItem.addMenuItem(new HomePage());
			groupItem.addMenuItem(LoginWidget.Instance());
			groupItem.addMenuItem(new RegistrationWidget());
		}

		// group1
		{
			groupItem = menu.getGroupMenuItem("Group 2", Images.res().folder());
			groupItem.addMenuItem(new TestRPC());
			groupItem.addMenuItem(new TestRPC());
		}


		LoginServiceAsync service = LoginService.Util.getInstance();
		service.isAuthorized(new AsyncCallback<User>() {
			@Override
			public void onSuccess(User user) {
				if (user == null){
					setContentWidget(LoginWidget.Instance());
				} else {
					setContentWidget(new HomePage());
				}				
			}

			@Override
			public void onFailure(Throwable caught) {
			}
		});

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
