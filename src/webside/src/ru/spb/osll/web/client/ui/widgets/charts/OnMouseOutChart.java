package ru.spb.osll.web.client.ui.widgets.charts;

import com.google.gwt.user.client.ui.Label;
import com.google.gwt.visualization.client.events.OnMouseOutHandler;

/**
 * Demo for OnMouseOutHandler that can be applied to any visualization.
 */
public class OnMouseOutChart extends OnMouseOutHandler {

  private final Label label;
  
  OnMouseOutChart(Label label) {
    this.label = label;
  }

  @Override
  public void onMouseOutEvent(OnMouseOutEvent event) { 
    StringBuffer b = new StringBuffer();
    b.append(" row: ");
    b.append(event.getRow());
    b.append(", column: ");
    b.append(event.getColumn());
    label.setText("Mouse out of " + b.toString()); 
  }
}
