/*
 * Copyright 2010-2012  Vasily Romanikhin  bac1ca89@gmail.com
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * 3. The name of the author may not be used to endorse or promote
 *    products derived from this software without specific prior written
 *    permission.
 *
 * The advertising clause requiring mention in adverts must never be included.
 * PROJ: OSLL/geo2tag
 */

package ru.spb.osll.tracker.services;

import java.util.List;

import ru.spb.osll.tracker.TrackerActivity;
import ru.spb.osll.tracker.exception.ExceptionHandler;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.util.Log;

class LocationService extends Service {

    private boolean isDeviceReady = false;
    
    @Override
    public void onCreate() {
        Thread.setDefaultUncaughtExceptionHandler(new ExceptionHandler(this));
    }

    @Override
    public void onStart(Intent intent, int startId) {
		super.onStart(intent, startId);
        Log.d(TrackerActivity.LOG, "LocationService.onStart()");
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        locationManager.requestLocationUpdates(LocationManager.NETWORK_PROVIDER, 0, 0, mLocationListener);
        locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 0, 0, mLocationListener);        
    }

    @Override
	public void onDestroy() {
        Log.d(TrackerActivity.LOG, "LocationService.onDestroy()");
        LocationManager locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        locationManager.removeUpdates(mLocationListener);
	}

    private LocationListener mLocationListener = new LocationListener() {
        public void onLocationChanged(Location location) {
            if (!isDeviceReady && location != null) {
                isDeviceReady = true;
            } else if (isDeviceReady && location == null) {
                isDeviceReady = false;
            }
        }

        public void onStatusChanged(String provider, int status, Bundle extras) {
            Log.d(TrackerActivity.LOG, "onStatusChanged: " + provider + ", " + status);
        }

        public void onProviderEnabled(String provider) {
            Log.d(TrackerActivity.LOG, "onProviderEnabled: " + provider);
        }

        public void onProviderDisabled(String provider) {
            Log.d(TrackerActivity.LOG, "onProviderDisabled: " + provider);
        }
    };

	public boolean isDeviceReady(){
		return isDeviceReady;
	}

    protected Location getLocation() {
        Location bestResult = null;
        long minTime = 30 * 1000;   // 1 sec
        float bestAccuracy = Float.MAX_VALUE;
        long bestTime = Long.MIN_VALUE;

        LocationManager  locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
        List<String> matchingProviders = locationManager.getAllProviders();
        for (String provider : matchingProviders) {
            Location location = locationManager.getLastKnownLocation(provider);
            if (location != null) {
                float accuracy = location.getAccuracy();
                long time = location.getTime();
                if ((time > minTime && accuracy < bestAccuracy)) {
                    bestResult = location;
                    bestAccuracy = accuracy;
                    bestTime = time;
                } else if (time < minTime && bestAccuracy == Float.MAX_VALUE
                    && time > bestTime) {
                    bestResult = location;
                    bestTime = time;
                }
            }
        }
        return bestResult;
    }	
    
    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }    
}
