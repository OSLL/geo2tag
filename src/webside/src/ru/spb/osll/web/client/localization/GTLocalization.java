package ru.spb.osll.web.client.localization;

import com.google.gwt.i18n.client.Constants;

public interface GTLocalization extends Constants {
	// buttons
	String btnOk();
	String btnCancel();
	String btnSignin();
	String btnSignout();
	
	// warnings
	String wrngLoginNull();
	String wrngPasswordNull();
	String wrngConfirmNull();
	String wrngPassAndConf(); 
	
	
	// --------------------- Menu --------------------- 
	String menuGrForUser();
	String menuGrServices();
	
	// -------------------- Widgets -------------------
	// Widget : UnderConstructionWidget
	String disabledRegistration();
	String disabledTagsOnMap();
	
	// Widget : HomePage
	String pageHome();

	// Widget : LoginWidget
	String login();
	String password();
	String confirm();
	String registration();
	String loginFailed();

	// Widget : Channel
	String channels();
	String btnSubscribe();
	String btnUnsubscribe();
	String btnShowTags();
	String labYourChannels();
	String labAvalChannels();

	// Widget : TagsMapWidget
	String TagsMapWidget();

	// Widget : TagsTableWidget
	String TagsTableWidget();

	// Widget : TagsTableWidget
	String radioBtnInChannel();
	String radioBtnMy();
	String btnRefresh();
	
	// Widget : TestAccountWidget
	String TestAccountWidget();
}
