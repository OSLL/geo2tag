package ru.spb.osll;

import java.io.BufferedReader;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.net.URI;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicHeader;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;
import org.apache.http.params.HttpParams;
import org.apache.http.protocol.HTTP;
import org.json.JSONException;
import org.json.JSONObject;
import org.json.JSONTokener;

import ru.spb.osll.json.JsonApplyChannelRequest;
import ru.spb.osll.json.JsonApplyMarkRequest;
import ru.spb.osll.json.JsonBase;
import ru.spb.osll.json.JsonLoginRequest;

import android.app.Activity;
import android.content.Context;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class TrackerActivity extends Activity {
	public static String LOG = "Tracker";
	private static final String SD_CARD_PATH = "/sdcard";	// FIXME hardcoding style; use later Environment.getExternalStorageState()
	private Logger m_loggerLocationListner = new Logger(SD_CARD_PATH + "/loggerLocationListner.txt");
//	private Logger m_loggerGetLocation = new Logger(SD_CARD_PATH + "/loggerGetLocation.txt");
	
	TextView logView;

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		logView = (TextView) findViewById(R.id.TextField);
		Button btnTest = (Button) findViewById(R.id.TestButton);
		btnTest.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                Log.d(LOG, "TestButton is activate");
                mySendPostJSON();
            }
        });
		

//		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
//		locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, locationListener);
		//runOnUiThread(new LogUpdateder());
	}

	@Override
	protected void onDestroy() {
		super.onDestroy();
		LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
		locationManager.removeUpdates(locationListener);

		m_loggerLocationListner.destroy();
	}

	public static Location getLocation(Context ctx) {
		Location location = null;
		String provider = LocationManager.NETWORK_PROVIDER;
		LocationManager lm = (LocationManager) ctx
				.getSystemService(Context.LOCATION_SERVICE);

		if (lm != null) {
			if (lm.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
				provider = LocationManager.GPS_PROVIDER;
			}
			location = lm.getLastKnownLocation(provider);
		}
		return location;
	}

	// TODO
	class LogUpdateder implements Runnable {
		@Override
		public void run() {
			int i = 0;
			while (i < 10) {
				i++;
				try {
					Location location = getLocation(getApplicationContext());
					String data = location != null ? location.toString() : "null";
					Thread.sleep(5000);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	LocationListener locationListener = new LocationListener() {
		int count = 0;
		
		public void onLocationChanged(Location location) {
			if (location != null){
				Log.v(LOG, "onLocationChanged  " + location.toString());
				m_loggerLocationListner.println("onLocationChanged  " + location.toString());

				logView.append("\n" + count + ": onLocationChanged " + location.getLatitude() + " " + location.getLongitude());
				count++;
			}
		}

		public void onStatusChanged(String provider, int status, Bundle extras) {
			Log.v(LOG, "onStatusChanged " + provider + " " + status + " " + extras);
			
			m_loggerLocationListner.println("onStatusChanged " + provider + " " + status + " " + extras);
			logView.append("\n" + count + ": onStatusChanged " + status);
			count++;
		}

		public void onProviderEnabled(String provider) {
			Log.v(LOG, "onProviderEnabled " + provider);
			m_loggerLocationListner.println("onProviderEnabled " + provider);
		}

		public void onProviderDisabled(String provider) {
			Log.v(LOG, "onProviderDisabled " + provider);
			m_loggerLocationListner.println("onProviderDisabled " + provider);
		}
	};

	
	private class Logger {
		private PrintWriter m_logger;
		DateFormat m_dateFormat = new SimpleDateFormat("yyyy/dd/MM/HH/mm/ss");
		
		private Logger(String logPath){
			try {
				m_logger = new PrintWriter(new OutputStreamWriter(new FileOutputStream(logPath, true), "UTF-8"));
				m_logger.println(m_dateFormat.format(new Date()) + " \t -------------- START LOG -------------");
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		
		public void println(String str){
			m_logger.println(m_dateFormat.format(new Date()) + "\t\t:" + str);
		}

		public void destroy(){
			if (m_logger != null)
				m_logger.close();
		}
	}
	
	
	// ----------------------------------------------------------------


	public void mySendPostJSON(){
		try {
			JSONObject JSONResponse = new JsonLoginRequest("Mark", "test").doRequest();			

			logView.append("\n" + JSONResponse.get("auth_token") + "\n" + 
					JSONResponse.get("status") + "\n" +
					JSONResponse.get("status_description"));

			JSONResponse = new JsonApplyChannelRequest("KKKKKKKKKK", 
					"Test channel", "my new super chanel", "http://osll.spb.ru/", 3000
					).doRequest();
			
			logView.append(
					"\n" + 
					JSONResponse.get("status") + "\n" +
					JSONResponse.get("status_description"));
			
			JSONResponse = new JsonApplyMarkRequest("KKKKKKKKKK", "My channel", "title",
					"unknown", "this tag was generated automaticaly by tracker application",
					60.0, 30.0, "04 03 2011 15:33:47.630"
					).doRequest();
			
			logView.append(
					"\n" + 
					JSONResponse.get("status") + "\n" +
					JSONResponse.get("status_description"));
			
			
		} catch (Exception e){
			logView.append("\n" + e.getMessage());
		}
	}
	
}
