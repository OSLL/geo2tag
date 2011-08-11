package ru.spb.osll.web.server;

import junit.framework.TestCase;

import ru.spb.osll.web.client.services.objects.CTag;
import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.CTags;
import ru.spb.osll.web.server.db.Channels;
import ru.spb.osll.web.server.db.Tags;
import ru.spb.osll.web.server.db.Users;
import java.sql.Timestamp;

public class Geo2TagServTest extends TestCase{
	
	  /*
	   * Tests class Users
	   */
	  
	public void testUsers() {
	    User testUser = new User("Dima", "Parol");
	    testUser = Users.insert(testUser);
	    assertTrue(Users.select("Dima").getPassword().equals("Parol"));
	    testUser.setToken("blablabla");
	    Users.update(testUser);
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
	    Channels.update(testChannel);
	    assertTrue(Channels.selectAll().get((int)testChannel.getId()-1).getDescription().equals("blablabla"));
	    boolean success = Channels.delete(testChannel);
	    assertTrue(success == true);
	    //assertTrue(Channels.select("Dima") == null);
	  } 

	public void testTags() {
		User testUser = new User("Dima", "Parol");
		testUser = Users.insert(testUser);
	    Tag testTag = new Tag("testlabel", "testurl", testUser.getId());
	    testTag = Tags.insert(testTag);
	    assertTrue(Tags.selectByUser(testUser).get(0).getLabel().equals("testlabel")); 		
	    testTag.setDescription("blablabla");
	    testTag = Tags.update(testTag);
	    assertTrue(Tags.selectByUser(testUser).get(0).getDescription().equals("blablabla"));
	    Channel testChannel = new Channel("Channel1", "testurl");
	    testChannel = Channels.insert(testChannel);
	    CTag testCTag = new CTag(testChannel.getId(), testTag.getId());
	    testCTag = CTags.insert(testCTag);
	    assertTrue(Tags.selectByChannel(testChannel).get(0).getDescription().equals("blablabla"));
	    boolean success = Users.delete(testUser);
	    assertTrue(success == true);
	    success = Channels.delete(testChannel);
	    assertTrue(success == true);
	    success = CTags.delete(testCTag);
	    assertTrue(success == true);
	    success = Tags.delete(testTag);
	    assertTrue(success == true);
	}
}
