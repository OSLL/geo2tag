package ru.spb.osll.json;

import java.io.IOException;
import java.net.URISyntaxException;

import org.json.JSONException;
import org.json.JSONObject;

abstract class JsonRequest {
	
	public JSONObject doRequest(){
		JSONObject resultJSON = null; 
		try {
			resultJSON = doRequestInternal(); 
		} catch (JSONException e) {
			e.printStackTrace();
			// TODO Auto-generated catch block
		} catch (IOException e) {
			e.printStackTrace();
			// TODO: handle exception
		} catch (URISyntaxException e) {
			e.printStackTrace();
			// TODO Auto-generated catch block
		}
		return resultJSON;
	}	

	abstract protected JSONObject doRequestInternal() 
		throws JSONException, IOException, URISyntaxException;	
	
}
