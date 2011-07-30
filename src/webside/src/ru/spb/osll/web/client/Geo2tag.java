package ru.spb.osll.web.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.core.client.GWT;
import com.google.gwt.user.client.ui.RootLayoutPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Geo2tag implements EntryPoint {

	public void onModuleLoad() {
		// Create the application shell.
		GTShell shell = new GTShell();
		RootLayoutPanel.get().add(shell);
	}
	
}
