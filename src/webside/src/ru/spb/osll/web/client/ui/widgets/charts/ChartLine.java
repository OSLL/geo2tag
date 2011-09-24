package ru.spb.osll.web.client.ui.widgets.charts;

import ru.spb.osll.web.client.ui.core.SimpleComposite;

import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.visualization.client.DataTable;
import com.google.gwt.visualization.client.AbstractDataTable.ColumnType;
import com.google.gwt.visualization.client.visualizations.corechart.AxisOptions;
import com.google.gwt.visualization.client.visualizations.corechart.LineChart;
import com.google.gwt.visualization.client.visualizations.corechart.Options;

/**
 * Demo for LineChart visualization.
 */
public class ChartLine extends SimpleComposite {

	@Override
	public String getName() {
		return "Line chart";
	}
	
	@Override
	protected Widget onInitialize() {
	    VerticalPanel result = new VerticalPanel();
        LineChart viz = new LineChart(getData(), getOptions());
        Label status = new Label();
        Label onMouseOverAndOutStatus = new Label();
        viz.addSelectHandler(new SelectionChart(viz, status));
        viz.addReadyHandler(new ReadyChart(status));
        viz.addOnMouseOverHandler(new OnMouseOverChart(onMouseOverAndOutStatus));
        viz.addOnMouseOutHandler(new OnMouseOutChart(onMouseOverAndOutStatus));
        result.add(status);
        result.add(viz);
        result.add(onMouseOverAndOutStatus);
        return result;
    }
	
	protected DataTable getData() {
		DataTable data = DataTable.create();
	    data.addColumn(ColumnType.STRING, "Year");
	    data.addColumn(ColumnType.NUMBER, "Sales");
	    data.addColumn(ColumnType.NUMBER, "Expenses");
	    data.addRows(4);
	    data.setValue(0, 0, "2004");
	    data.setValue(0, 1, 1000);
	    data.setValue(0, 2, 400);
	    data.setValue(1, 0, "2005");
	    data.setValue(1, 1, 1170);
	    data.setValue(1, 2, 460);
	    data.setValue(2, 0, "2006");
	    data.setValueNull(2, 1);
	    data.setValueNull(2, 2);
	    data.setValue(3, 0, "2007");
	    data.setValue(3, 1, 1030);
	    data.setValue(3, 2, 540);
	    return data;
	}
	
	protected Options getOptions() {
		Options options = Options.create();
	    options.setHeight(240);
	    options.setTitle("Company Performance");
	    options.setWidth(400);
	    options.setInterpolateNulls(true);
	    AxisOptions vAxisOptions = AxisOptions.create();
	    vAxisOptions.setMinValue(0);
	    vAxisOptions.setMaxValue(2000);
	    options.setVAxisOptions(vAxisOptions);
	    return options;
	}
	
}
