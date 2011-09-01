package ru.spb.osll.web.client;

import java.util.ArrayList;
import java.util.List;

import com.google.gwt.visualization.client.AbstractDataTable.ColumnType;
import com.google.gwt.visualization.client.DataTable;
import com.google.gwt.visualization.client.LegendPosition;

import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.ui.core.SimpleComposite;
import ru.spb.osll.web.client.ui.core.SimpleMenu;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree;
import ru.spb.osll.web.client.ui.core.SimpleMenuTree.GroupItem;
import ru.spb.osll.web.client.ui.image.Images;
import ru.spb.osll.web.client.ui.widgets.LoginWidget;
import ru.spb.osll.web.client.ui.widgets.OSMWidget;
import ru.spb.osll.web.client.ui.widgets.TestRPC;
import ru.spb.osll.web.client.ui.widgets.WidgetByDesigner;
import ru.spb.osll.web.client.ui.widgets.charts.ChartLine;
import ru.spb.osll.web.client.ui.widgets.charts.ChartPie;

import com.google.gwt.visualization.client.visualizations.corechart.AxisOptions;
import com.google.gwt.visualization.client.visualizations.corechart.Options;
import com.google.gwt.visualization.client.visualizations.corechart.PieChart;

public class GTMenu extends SimpleMenu<SimpleComposite> {

	@Override
	protected void initMenu() {
		final SimpleMenuTree<SimpleComposite> menu = getMenuTree();
		GroupItem<SimpleComposite> groupItem ; 
		    
		 // group1
		 {
			groupItem = menu.getGroupMenuItem("Group 1", Images.res().folder());
			groupItem.addMenuItem(new WidgetByDesigner());
			groupItem.addMenuItem(new WidgetByDesigner());
			groupItem.addMenuItem(new LoginWidget());
			groupItem.addMenuItem(new OSMWidget() {
				@Override
				public List<Tag> getTags() {
					List<Tag> list = new ArrayList<Tag>();
					Tag firstTag = new Tag("firstTestLabel", "firstTestUrl", 5);
					firstTag.setLongitude((float) 6.95);
					firstTag.setLatitude((float) 50.94);
					list.add(firstTag);
					Tag secondTag = new Tag("secondTestLabel", "secondTestUrl", 6);
					secondTag.setLongitude((float) 6.96);
					secondTag.setLatitude((float) 50.94);
					list.add(secondTag); 
					return list;
				}
			}
			
			);
		 }

		// group2
		{
			groupItem = menu.getGroupMenuItem("Group 2", Images.res().folder());
			groupItem.addMenuItem(new TestRPC());
			groupItem.addMenuItem(new TestRPC());
			groupItem.addMenuItem(new ChartPie() {
				@Override
				public DataTable getData() {
					 /* create a datatable */
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
				
				@Override
				public PieChart.PieOptions getOptions() {
					PieChart.PieOptions options = PieChart.createPieOptions();
				    options.setWidth(400);
				    options.setHeight(240);
				    options.set3D(true);
				    options.setTitle("My Daily Activities");
				    options.setLegend(LegendPosition.LEFT);
				    return options;
				}
				
			});
			groupItem.addMenuItem(new ChartLine() {
				@Override
				public DataTable getData() {
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
				
				@Override
				public Options getOptions() {
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
				
			});
		} 	
	}

	@Override
	protected boolean setContentWidget(SimpleComposite widget) {
		GTShell shell = GTShell.Instance;
		if (shell != null){
			shell.setContent(widget.getInitializedWidget());
			return true;
		}
		return false;
	}
}