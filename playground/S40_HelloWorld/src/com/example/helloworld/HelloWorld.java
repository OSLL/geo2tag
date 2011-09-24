package com.example.helloworld;

import java.io.*;
import javax.microedition.io.*;
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
import javax.microedition.location.Coordinates;
import javax.microedition.location.Criteria;
import javax.microedition.location.Location;
import javax.microedition.location.LocationException;
import javax.microedition.location.LocationProvider;
 
public class HelloWorld extends MIDlet implements CommandListener {
 
    public HelloWorld() {
    }
    // Display
    private Display display;
    // Main form
    private Form form;
    // For the message
    private StringItem stringItem;
    // For the exit command
    private Command exitCommand, getCommand, postCommand,coordCommand;
 
    public void commandAction(Command command, Displayable displayable){
        if (displayable == form) {
        try{
            if (command == exitCommand) {
                exitMIDlet();
            }else if (command == getCommand) {
                doGETRequest();
            }else if(command == postCommand) {
                doPOSTRequest();
            }else if (command == coordCommand){
            	printLocation();
            }
        }catch (Exception e) {
        	stringItem.setText("Exception!");
        }
        }
    }
 
    public void startApp() {
        // Create form
        stringItem = new StringItem("Hello", "Hello World!");
        form = new Form(null, new Item[] {stringItem});
        exitCommand = new Command("Exit", Command.EXIT, 1);
        //Adding external controls
        getCommand = new Command("GET", Command.SCREEN, 2);
        postCommand = new Command("POST", Command.SCREEN, 2);
        coordCommand = new Command("Get Location", Command.SCREEN, 2);
        form.addCommand(exitCommand);
        form.addCommand(getCommand);
        form.addCommand(postCommand);
        form.addCommand(coordCommand);
        form.setCommandListener(this);
 
        // Get display for drawning
        display = Display.getDisplay(this);        
        display.setCurrent(form);
    }
 
    // Your MIDlet should not call pauseApp(), only system will call this life-cycle method
    public void pauseApp() {
    }
 
    // Your MIDlet should not call destroyApp(), only system will call this life-cycle method    
    public void destroyApp(boolean unconditional) {
    }
 
    public void doGETRequest()throws IOException{
    	stringItem.setText("GET command called");
    	InputStream is = null;
        HttpConnection httpConn = null;
        try {
        	// Building request
        	httpConn = (HttpConnection)Connector.open("http://tracklife.ru");
        	httpConn.setRequestMethod(HttpConnection.GET);
        	httpConn.setRequestProperty("User-Agent","Profile/MIDP-1.0 Confirguration/CLDC-1.0");
        	if (httpConn.getResponseCode()==httpConn.HTTP_OK){
        		// Getting stream with response data
                is = httpConn.openDataInputStream();
                // Write on the screen response data
                stringItem.setText(streamToStr(is));
        	}else stringItem.setText("Error in GET request. Code "+
        			Integer.toString(httpConn.getResponseCode()));
        }finally{
            if(is!= null)
                is.close();
            if(httpConn != null)
                httpConn.close();
        }
    }
    
 //Make String from InputStream
    private String streamToStr(InputStream is) throws IOException{
    	StringBuffer sb = new StringBuffer();
        int chr;
        while ((chr = is.read()) != -1)
          sb.append((char) chr);
        return sb.toString();
    }
    
    public void doPOSTRequest()throws IOException{
    	// All things are the same as in GET request case except POST data setting
    	stringItem.setText("POST command called");
    	InputStream is = null;
        OutputStream os = null;
        HttpConnection httpConn = null;
        try {
        	httpConn = (HttpConnection)Connector.open("http://tracklife.ru/service?query=login");
        	httpConn.setRequestMethod(HttpConnection.POST);
        	httpConn.setRequestProperty("User-Agent","Profile/MIDP-1.0 Confirguration/CLDC-1.0");
        	// Here we setting up POST request data
        	os = httpConn.openOutputStream();
        	String json = new String("{\"login\":\"Vasja\",\"password\":\"VasjaPWD\"}");
        	os.write(json.getBytes());
        	// 
        	if (httpConn.getResponseCode()==httpConn.HTTP_OK){
                is = httpConn.openDataInputStream();
                stringItem.setText(streamToStr(is));
        	}else stringItem.setText("Error in POST request. Code "+
        			Integer.toString(httpConn.getResponseCode()));
        }finally{
            if(is!= null)
                is.close();
            if(os != null)
            	os.close();
            if(httpConn != null)
                httpConn.close();
        }
    }
    
    public void printLocation() throws LocationException, InterruptedException{
    	Location location;
        LocationProvider locationProvider;
        Coordinates coordinates;
        // Set criteria for selecting a location provider:
        // accurate to 500 meters horizontally
        Criteria criteria= new Criteria();
        criteria.setHorizontalAccuracy(500);
        // Get an instance of the provider
        locationProvider = LocationProvider.getInstance(criteria);
        // Request the location, setting a one-minute timeout
        location = locationProvider.getLocation(60);
        coordinates = location.getQualifiedCoordinates();
        stringItem.setText("Location:"+Double.toString(coordinates.getLatitude())+" "
        		+Double.toString(coordinates.getLongitude()));
    }
    
    public void exitMIDlet() {
        display.setCurrent(null);
        notifyDestroyed();
    }
 
}