/*
 * Copyright 2011-2012 OSLL
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.web.client.localization;

import com.google.gwt.i18n.client.Constants;

public interface GTLocalization extends Constants {
	// buttons
	String btnOk();
	String btnCancel();
	String btnSignin();
	String btnRegister();
	String btnSignout();
	String btnSubmit();
	
	// warnings
	String wrngLoginNull();
	String wrngEmailNull();
	String wrngPasswordNull();
	String wrngConfirmNull();
	String wrngPassAndConf(); 
	String wrngNotLogged();
	
	// --------------------- Menu --------------------- 
	String menuGrForUser();
	String menuGrServices();
	
	// -------------------- Widgets -------------------
	// Widget : UnderConstructionWidget
	String disabledRegistration();
	String disabledTagsOnMap();
	
	// Widget : HomePage
	String pageHome();
	String currentBuild();
	String version();
	String deployDate();
	
	// Widget : LoginWidget
	String pageLogin();
	String login();
	String password();
	String confirm();
	String loginFailed();
	String invalidCredentials();

	// Widget : RegistrationWidget
	String email();
	String userExists();
	String waitForMessage();
	String registration();
	String successReg();

	// Widget : Channel
	String channels();
	String btnSubscribe();
	String btnUnsubscribe();
	String btnShowTags();
	String labYourChannels();
	String labAvalChannels();
	String itemAllChannels();

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
	
	// Widget : RestorePasswordWidget
	String passwRestoring();
	String forgotPassword();
	String waitForPwdRestoring();
}
