package ru.spb.osll.web.client.ui.widgets.charts;

import ru.spb.osll.web.client.ui.core.SimpleComposite;

import com.google.gwt.user.client.ui.Label;
import com.google.gwt.user.client.ui.VerticalPanel;
import com.google.gwt.user.client.ui.Widget;
import com.google.gwt.visualization.client.AbstractDataTable;
import com.google.gwt.visualization.client.AbstractDataTable.ColumnType;
import com.google.gwt.visualization.client.DataTable;
import com.google.gwt.visualization.client.LegendPosition;
import com.google.gwt.visualization.client.visualizations.corechart.PieChart;
import com.google.gwt.visualization.client.visualizations.corechart.PieChart.PieOptions;

/**
 * Demo for PieChart visualization.
 */
public class ChartPie extends SimpleComposite {

	@Override
	protected String getName() {
		return "Pie chart";
	}
	
	@Override
	public Widget onInitialize() { 
		VerticalPanel result = new VerticalPanel();
	    PieChart viz = new PieChart(createTable(), createOptions());
	    Label status = new Label();
	    Label onMouseOverAndOutStatus = new Label();
	    viz.addSelectHandler(new SelectionChart(viz, status));
	    viz.addReadyHandler(new ReadyChart(status));
	    //viz.addOnMouseOverHandler(new OnMouseOverChart(onMouseOverAndOutStatus));
	    //viz.addOnMouseOutHandler(new OnMouseOutChart(onMouseOverAndOutStatus));
	    result.add(status); 
	    result.add(viz);
	    result.add(onMouseOverAndOutStatus);
	    return result;
	}
    
    protected AbstractDataTable createTable() {
	    DataTable data = DataTable.create();
	    data.addColumn(ColumnType.STRING, "Task");
	    data.addColumn(ColumnType.NUMBER, "Hours per Day");
	    data.addRows(5);
	    data.setValue(0, 0, "Work");
	    data.setValue(0, 1, 11);
	    data.setValue(1, 0, "Eat");
	    data.setValue(1, 1, 2);
	    data.setValue(2, 0, "Commute");
	    data.setValue(2, 1, 2);
	    data.setValue(3, 0, "Watch TV");
	    data.setValue(3, 1, 2);
	    data.setValue(4, 0, "Sleep");
	    data.setValue(4, 1, 7);
        return data;
    }
    
    protected PieOptions createOptions() {
		PieChart.PieOptions options = PieChart.createPieOptions();
	    options.setWidth(400);
	    options.setHeight(240);
	    options.set3D(true);
	    options.setTitle("My Daily Activities");
	    options.setLegend(LegendPosition.LEFT);
	    return options;
    }
  
}