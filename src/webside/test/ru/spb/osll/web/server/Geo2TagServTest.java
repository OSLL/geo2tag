package ru.spb.osll.web.server;

import java.util.List;

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
	
	public void testChannels() {
	    Channel testChannel = new Channel("Channel1", "TestChannel", "www.bac1ca.com/channel");
	    
	    Channels.Instance().delete(testChannel);
	    assertTrue(Channels.Instance().delete(testChannel)); // FIXME
	    
	    assertTrue(null != Channels.Instance().insert(testChannel)); 		
	    assertTrue(null != Channels.Instance().insert(testChannel));
	    
	    testChannel.setDescription("blablabla");
	    assertTrue(Channels.Instance().update(testChannel));
	    
	    List<Channel> channels = Channels.Instance().selectAll();
	    assertTrue(channels.get(channels.size() -1).getDescription().equals("blablabla"));

	    assertTrue(Channels.Instance().delete(testChannel));
	} 

	public void testSubscribe() {
		User testUser = new User("Dima", "pass");
		Users.Instance().delete(testUser);
		testUser = Users.Instance().insert(testUser);
		
		Channel testChannel1 = new Channel("Channel1", "TestChannel", "www.bac1ca.com/channel");
	    Channel testChannel2 = new Channel("Channel2", "TestChannel", "www.bac1ca.com/channel");
	    Channels.Instance().delete(testChannel1);
	    Channels.Instance().delete(testChannel2);
	    testChannel1 = Channels.Instance().insert(testChannel1); 		
	    testChannel2 = Channels.Instance().insert(testChannel2); 		

	    assertTrue(Users.Instance().subscribeToChannel(testChannel1, testUser));
	    assertTrue(Users.Instance().subscribeToChannel(testChannel2, testUser));
//	    assertFalse(Users.Instance().subscribeToChannel(testChannel1, testUser));	// FIXME
//	    assertFalse(Users.Instance().subscribeToChannel(testChannel2, testUser));	// FIXME
	    
	    List<Channel> channels = Channels.Instance().selectByUser(testUser);
	    assertTrue(2 == channels.size());
	
	    assertTrue(Users.Instance().unsubscribeFromChannel(testChannel1, testUser));
//	    assertFalse(Users.Instance().unsubscribeFromChannel(testChannel1, testUser));	//FIXME

	    channels = Channels.Instance().selectByUser(testUser);
	    assertTrue(1 == channels.size());
	    
	    Users.Instance().delete(testUser);
	    Channels.Instance().delete(testChannel1);
	    Channels.Instance().delete(testChannel2);
	} 
	
	public void testTags() {
		User testUser = new User("Dima", "pass");
		Users.Instance().delete(testUser);
		testUser = Users.Instance().insert(testUser);
		
	    Tag testTag1 = new Tag(1f, 1f, "testlabel1", "testTag1", "testurl", testUser.getId());
	    testTag1 = Tags.Instance().insert(testTag1);
	    assertTrue(Tags.Instance().selectByUser(testUser).get(0).getLabel().equals("testlabel1"));

	    Tag testTag2 = new Tag(2f, 2f, "testlabel2", "testTag2", "testurl", testUser.getId());
	    testTag2 = Tags.Instance().insert(testTag2);

	    assertTrue(2 == Tags.Instance().selectByUser(testUser).size());
	    Tags.Instance().delete(testTag2);
	    assertTrue(1 == Tags.Instance().selectByUser(testUser).size());
	    
	    testTag1.setDescription("blablabla");
	    assertTrue(Tags.Instance().update(testTag1));
	    assertTrue(Tags.Instance().selectByUser(testUser).get(0).getDescription().equals("blablabla"));

	    // CHANNELS
		Channel testChannel1 = new Channel("Channel1", "TestChannel", "www.bac1ca.com/channel");
	    Channel testChannel2 = new Channel("Channel2", "TestChannel", "www.bac1ca.com/channel");
	    Channels.Instance().delete(testChannel1);
	    Channels.Instance().delete(testChannel2);
	    testChannel1 = Channels.Instance().insert(testChannel1); 		
	    testChannel2 = Channels.Instance().insert(testChannel2); 		

	    Tags.Instance().addTagToChannel(testChannel1, testTag1);
	    Tags.Instance().addTagToChannel(testChannel2, testTag1);
	    
	    assertTrue(1 == Tags.Instance().selectByChannel(testChannel1).size());
	    Tags.Instance().removeTagFromChannel(testChannel1, testTag1);
	    assertTrue(0 == Tags.Instance().selectByChannel(testChannel1).size());
	    
	    Users.Instance().delete(testUser);
	    Channels.Instance().delete(testChannel1);
	    Channels.Instance().delete(testChannel2);    
	    Tags.Instance().delete(testTag1);
	    Tags.Instance().delete(testTag2);
	}
	
}
