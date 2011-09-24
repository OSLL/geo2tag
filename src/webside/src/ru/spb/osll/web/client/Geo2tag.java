package ru.spb.osll.web.client;

import com.google.gwt.core.client.EntryPoint;
import com.google.gwt.user.client.ui.RootLayoutPanel;

/**
 * Entry point classes define <code>onModuleLoad()</code>.
 */
public class Geo2tag implements EntryPoint {

	public void onModuleLoad() {
		// Create the application shell.
		final GTShell shell = new GTShell();
		RootLayoutPanel.get().add(shell);

		// demo visualization
//		Runnable onLoadCallback = new Runnable() {
//			public void run() {
//				GTShell.Instance.setContent(new ChartPie());
//			}
//		};
//		VisualizationUtils.loadVisualizationApi(onLoadCallback, PieChart.PACKAGE);
//		Runnable onLoadCallback = new Runnable() {
//			public void run() {
//				GTShell.Instance.setContent(new ChartLine());
//			}
//		};
//		VisualizationUtils.loadVisualizationApi(onLoadCallback, PieChart.PACKAGE);
	}
	
}
