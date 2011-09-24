package ru.spb.osll.web.client.ui.widgets.charts;

import com.google.gwt.core.client.JsArray;
import com.google.gwt.user.client.ui.Label;
import com.google.gwt.visualization.client.Selectable;
import com.google.gwt.visualization.client.Selection;
import com.google.gwt.visualization.client.events.SelectHandler;

/**
 * Demo for SelectHandler that can be applied to any Selectable visualization.
 */
class SelectionChart extends SelectHandler {
  private final Selectable viz;
  private final Label label;

  SelectionChart(Selectable viz, Label label) {
    this.viz = viz;
    this.label = label;
  }

  @Override
  public void onSelect(SelectEvent event) {
    StringBuffer b = new StringBuffer();
    JsArray<Selection> s = getSelections();
    for (int i = 0; i < s.length(); ++i) {
      if (s.get(i).isCell()) {
        b.append(" cell ");
        b.append(s.get(i).getRow());
        b.append(":");
        b.append(s.get(i).getColumn());
      } else if (s.get(i).isRow()) {
        b.append(" row ");
        b.append(s.get(i).getRow());
      } else {
        b.append(" column ");
        b.append(s.get(i).getColumn());
      }
    }
    label.setText("selection changed " + b.toString()); 
  }

  private JsArray<Selection> getSelections() {
    return viz.getSelections();
  }
}