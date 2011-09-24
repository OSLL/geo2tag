package ru.spb.osll.web.client.ui.widgets.charts;

import com.google.gwt.user.client.ui.Label;
import com.google.gwt.visualization.client.events.OnMouseOverHandler;

/**
 * Demo for OnMouseOverHandler that can be applied to any visualization.
 */
public class OnMouseOverChart extends OnMouseOverHandler {

  private final Label label;
  
  OnMouseOverChart(Label label) {
    this.label = label;
  }

  @Override
  public void onMouseOverEvent(OnMouseOverEvent event) {
    int row = event.getRow();
    int column = event.getColumn();
    StringBuffer b = new StringBuffer();
    b.append(" row: ");
    b.append(row);
    b.append(", column: ");
    b.append(column);
    label.setText("Mouse over " + b.toString()); 
  }
}

