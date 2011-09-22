package ru.spb.osll.web.client.ui.widgets.charts;

import com.google.gwt.user.client.ui.Label;
import com.google.gwt.visualization.client.events.ReadyHandler;

/**
 * Demo for ReadyHandler that can be applied to any visualization.
 */
public class ReadyChart extends ReadyHandler {
  private final Label label;

  public ReadyChart(Label label) {
    this.label = label;
  }

  @Override
  public void onReady(ReadyEvent event) {
    label.setText("The visualization is ready"); 
  }
}
