/*
 * Copyright 2010-2011  Vasily Romanikhin  bac1ca89@gmail.com
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
 */

/*! ---------------------------------------------------------------
 * PROJ: OSLL/geo2tag
 * ---------------------------------------------------------------- */

package ru.spb.osll.json;

public interface IRequest {
		
	public interface ILogin{
		String REQUEST = "/service/login";
		
		String LOGIN = "login";
		String PASSWORD = "password";
	}

	public interface IAddUser{
		String REQUEST = "/service/addUser";
		
		String LOGIN = "login";
		String PASSWORD = "password";
		String EMAIL = "email";
	}

	public interface IApplyChannel{
		String REQUEST = "/service/addChannel";
		
		String AUTH_TOKEN = "auth_token";
		String NAME = "name";
		String DESCRIPTION = "description";
		String URL = "url";
		String ACTIVE_RADIUS = "activeRadius";
	}

	public interface IApplyMark{
		String REQUEST = "/service/writeTag";

		String AUTH_TOKEN = "auth_token";
		String CHANNEL = "channel";
		String TITLE = "title";
		String LINK = "link";
		String DESCRIPTION = "description";
		String LATITUDE = "latitude";
		String LONGITUDE = "longitude";
		String TIME = "time";
		String ALTITUDE = "altitude";
	}
	
	public interface ISubscribeChannel {
		String REQUEST = "/service/subscribe";
		
		String AUTH_TOKEN = "auth_token";
		String CHANNEL = "channel";
	}
	
	public interface IResponse{
		String STATUS = "status";
		String STATUS_DESCRIPTION = "status_description";
		String AUTH_TOKEN = "auth_token";
		
		String OK_STATUS = "Ok";
		String ERROR_STATUS = "Error";

		String USER_EXTSTS = "Username already exists!";
		String CHANNEL_EXTSTS = "Channel exists!";
		
		// Codes of errors:
		public enum geo2tagError
		{
			SUCCESS ,
			WRONG_TOKEN_ERROR ,
			USER_ALREADY_EXIST_ERROR ,
			USER_DOES_NOT_EXIST_ERROR ,
			CHANNEL_ALREADY_EXIST_ERROR ,
			CHANNEL_DOES_NOT_EXIST_ERROR ,
			SUBSCRIPTION_ALREADY_EXIST ,
			INTERNAL_DB_ERROR ,
			INCORRECT_QUERY_NAME_ERROR ,
			INCORRECT_JSON_ERROR ,
			INCORRECT_CREDENTIALS_ERROR ,
			CHANNEL_NOT_SUBCRIBED_ERROR ,
		  	CHANNEL_ALREADY_SUBSCRIBED_ERROR ,
		  	TAG_DOES_NOT_EXIST_ERROR ,
		  	TAG_ALREADY_EXIST_ERROR ,
		  	NULL_TIMESLOT_ERROR ,
		  	UNKNOWN_ERROR
		};
	}
	
	public interface IBaseFilter{
		String AUTH_TOKEN 		= "auth_token";
		String TIME_FROM 		= "time_from";
		String TIME_TO 			= "time_to";
		
		String ALT_SHIFT 		= "altitude_shift";
		String ALT1 			= "altitude1";
		String ALT2 			= "altitude2";		
	}
	
}
