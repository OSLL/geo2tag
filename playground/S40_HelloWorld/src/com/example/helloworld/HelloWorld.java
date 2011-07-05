package com.example.helloworld;

 
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;
 
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
    private Command exitCommand;
 
    public void commandAction(Command command, Displayable displayable) {
        if (displayable == form) {
            if (command == exitCommand) {
                exitMIDlet();
            }
        }
    }
 
    public void startApp() {
        // Create form
        stringItem = new StringItem("Hello", "Hello World!");
        form = new Form(null, new Item[] {stringItem});
        exitCommand = new Command("Exit", Command.EXIT, 1);
        form.addCommand(exitCommand);
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
 
    public void exitMIDlet() {
        display.setCurrent(null);
        notifyDestroyed();
    }
 
}