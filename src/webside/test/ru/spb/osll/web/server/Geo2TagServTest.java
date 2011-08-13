package ru.spb.osll.web.server;

import junit.framework.TestCase;

import ru.spb.osll.web.client.services.objects.Channel;
import ru.spb.osll.web.client.services.objects.Tag;
import ru.spb.osll.web.client.services.objects.User;
import ru.spb.osll.web.server.db.Channels;
import ru.spb.osll.web.server.db.Tags;
import ru.spb.osll.web.server.db.Users;

public class Geo2TagServTest extends TestCase{
	
	public void testUsers() {
		User testUser = new User("Dima", "pass");
	    Users.Instance().delete(testUser);
		
		testUser = Users.Instance().insert(testUser);
	    testUser = testUser != null ? testUser : Users.Instance().select("Dima");
	    assertTrue(Users.Instance().select("Dima").getPassword().equals("pass"));

	    testUser.setToken("blablabla");
	    Users.Instance().update(testUser);
	    String testToken = Users.Instance().select("Dima").getToken();
	    assertTrue(testToken.equals("blablabla"));
	    
	    assertTrue(Users.Instance().delete(testUser));
	    assertTrue(Users.Instance().select("Dima") == null); 
	}
	
//	public void testChannels() {
//	    Channel testChannel = new Channel("Channel1", "testurl");
//	    testChannel = Channels.insert(testChannel);
//	    assertTrue(Channels.selectAll().get((int)testChannel.getId()-1).getName().equals("Channel1")); 		
//	    testChannel.setDescription("blablabla");
//	    Channels.update(testChannel);
//	    assertTrue(Channels.selectAll().get((int)testChannel.getId()-1).getDescription().equals("blablabla"));
//	    boolean success = Channels.delete(testChannel);
//	    assertTrue(success == true);
//	    //assertTrue(Channels.select("Dima") == null);
//	  } 

//	public void testTags() {
//		User testUser = new User("Dima", "Parol");
//		testUser = Users.insert(testUser);
//	    Tag testTag = new Tag("testlabel", "testurl", testUser.getId());
//	    testTag = Tags.insert(testTag);
//	    assertTrue(Tags.selectByUser(testUser).get(0).getLabel().equals("testlabel")); 		
//	    testTag.setDescription("blablabla");
//	    assertTrue(Tags.update(testTag));
//	    assertTrue(Tags.selectByUser(testUser).get(0).getDescription().equals("blablabla"));
//	    Channel testChannel = new Channel("Channel1", "testurl");
//	    testChannel = Channels.insert(testChannel);
//	    Tags.addTagToChannel(testChannel, testTag);
//	    assertTrue(Tags.selectByChannel(testChannel).get(0).getDescription().equals("blablabla"));
//	    boolean success = Users.delete(testUser);
//	    assertTrue(success == true);
//	    success = Channels.delete(testChannel);
//	    assertTrue(success == true);
//	    success = Tags.removeTagFromChannel(testChannel, testTag);
//	    assertTrue(success == true);
//	    success = Tags.delete(testTag);
//	    assertTrue(success == true);
//	}
}
