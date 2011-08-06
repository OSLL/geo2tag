package ru.spb.osll.web.server;

import junit.framework.TestCase;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.Channels;
import ru.spb.osll.web.server.db.Users;


public class Geo2TagServTest extends TestCase{
	
	  /*
	   * Tests class Users
	   */
	  
	public void testUsers() {
	    User testUser = new User("Dima", "Parol");
	    testUser = Users.insert(testUser);
	    assertTrue(Users.select("Dima").getPassword().equals("Parol"));
	    testUser.setToken("blablabla");
	    testUser = Users.update(testUser);
	    String testToken = Users.select("Dima").getToken();
	    assertTrue(testToken.equals("blablabla"));
	    boolean success = Users.delete(testUser);
	    assertTrue(success == true);
	    assertTrue(Users.select("Dima") == null); 
	  } 
	
	public void testChannels() {
	    Channel testChannel = new Channel("Channel1", "testurl");
	    testChannel = Channels.insert(testChannel);
	    assertTrue(Channels.selectAll().get((int)testChannel.getId()-1).getName().equals("Channel1")); 		
	    testChannel.setDescription("blablabla");
	    testChannel = Channels.update(testChannel);
	    assertTrue(Channels.selectAll().get((int)testChannel.getId()-1).getDescription().equals("blablabla"));
	    boolean success = Channels.delete(testChannel);
	    assertTrue(success == true);
	    //assertTrue(Channels.select("Dima") == null);
	  } 

}
