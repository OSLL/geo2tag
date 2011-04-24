package ru.spb.osll.gui;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
 
public class RadioButtonDialog{
	protected Context myContext;
	private String myTitle;
	private String[] myItems;
	private int mySelectedItem;
	
	public RadioButtonDialog(Context context, String title, String[] items, int selectedItem){
		myContext = context;
		myTitle = title;
		myItems = items;
		mySelectedItem = selectedItem;
	}

	protected void itemSelected(DialogInterface dialog, int item){
		dialog.dismiss();
	}
	
	public void show(){
		AlertDialog.Builder builder = new AlertDialog.Builder(myContext);
		builder.setTitle(myTitle);
		builder.setSingleChoiceItems(myItems, mySelectedItem, new DialogInterface.OnClickListener() {
		    public void onClick(DialogInterface dialog, int item) {
		    	itemSelected(dialog, item);
		    }
		});
		builder.create().show();
	}
}