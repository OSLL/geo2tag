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