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
