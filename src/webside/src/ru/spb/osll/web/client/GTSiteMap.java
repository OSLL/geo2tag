package ru.spb.osll.web.client;

import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SiteMap;
import ru.spb.osll.web.client.ui.widgets.Channels;
import ru.spb.osll.web.client.ui.widgets.HomePage;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;
import ru.spb.osll.web.client.ui.widgets.RegistrationWidget;
import ru.spb.osll.web.client.ui.widgets.TagsMapWidget;
import ru.spb.osll.web.client.ui.widgets.TagsTableWidget;
import ru.spb.osll.web.client.ui.widgets.TestAccountWidget;

public class GTSiteMap extends SiteMap {

	public GTSiteMap(SimpleMenu menu) {
		super(menu);
		addWidget(HomePage.Instance());
		addWidget(TestAccountWidget.Instance());
		addWidget(Channels.Instance());
		addWidget(TagsTableWidget.Instance());
		addWidget(TagsMapWidget.Instance());
		addWidget(LoginWidget.Instance());
		addWidget(RegistrationWidget.Instance());
	}
}
