/*
 * Copyright 2012 OSLL
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

package ru.spb.osll.json;

import java.util.HashMap;
import java.util.Map;

public class Errno {

    public static int SUCCESS                           = 0;
    public static int WRONG_TOKEN_ERROR                 = 1;
    public static int USER_ALREADY_EXIST_ERROR          = 2;
    public static int USER_DOES_NOT_EXIST_ERROR         = 3;
    public static int CHANNEL_ALREADY_EXIST_ERROR       = 4;
    public static int CHANNEL_DOES_NOT_EXIST_ERROR      = 5;
    public static int SUBSCRIPTION_ALREADY_EXIST        = 6;
    public static int INTERNAL_DB_ERROR                 = 7;
    public static int INCORRECT_QUERY_NAME_ERROR        = 8;
    public static int INCORRECT_JSON_ERROR              = 9;
    public static int INCORRECT_CREDENTIALS_ERROR       = 10;
    public static int CHANNEL_NOT_SUBCRIBED_ERROR       = 11;
    public static int CHANNEL_ALREADY_SUBSCRIBED_ERROR  = 12;
    public static int TAG_DOES_NOT_EXIST_ERROR          = 13;
    public static int TAG_ALREADY_EXIST_ERROR           = 14;
    public static int NULL_TIMESLOT_ERROR               = 15;
    public static int UNKNOWN_ERROR                     = 16;
    public static int TMP_USER_ALREADY_EXIST_ERROR      = 17;
    public static int NETWORK_ERROR                     = 18;
    public static int EMAIL_ALREADY_EXIST_ERROR         = 19;

    private static Map<Integer, String> emap = new HashMap<Integer, String>();
    static {
        emap.put(SUCCESS,                           "SUCCESS");                         // 0
        emap.put(WRONG_TOKEN_ERROR,                 "WRONG_TOKEN_ERROR");               // 1
        emap.put(USER_ALREADY_EXIST_ERROR,          "USER_ALREADY_EXIST_ERROR");        // 2
        emap.put(USER_DOES_NOT_EXIST_ERROR,         "USER_DOES_NOT_EXIST_ERROR");       // 3
        emap.put(CHANNEL_ALREADY_EXIST_ERROR,       "CHANNEL_ALREADY_EXIST_ERROR");     // 4
        emap.put(CHANNEL_DOES_NOT_EXIST_ERROR,      "CHANNEL_DOES_NOT_EXIST_ERROR");    // 5
        emap.put(SUBSCRIPTION_ALREADY_EXIST,        "SUBSCRIPTION_ALREADY_EXIST");      // 6
        emap.put(INTERNAL_DB_ERROR,                 "INTERNAL_DB_ERROR");               // 7
        emap.put(INCORRECT_QUERY_NAME_ERROR,        "INCORRECT_QUERY_NAME_ERROR");      // 8
        emap.put(INCORRECT_JSON_ERROR,              "INCORRECT_JSON_ERROR");            // 9
        emap.put(INCORRECT_CREDENTIALS_ERROR,       "INCORRECT_CREDENTIALS_ERROR");     // 10
        emap.put(CHANNEL_NOT_SUBCRIBED_ERROR,       "CHANNEL_NOT_SUBCRIBED_ERROR");     // 11
        emap.put(CHANNEL_ALREADY_SUBSCRIBED_ERROR,  "CHANNEL_ALREADY_SUBSCRIBED_ERROR");// 12
        emap.put(TAG_DOES_NOT_EXIST_ERROR,          "TAG_DOES_NOT_EXIST_ERROR");        // 13
        emap.put(TAG_ALREADY_EXIST_ERROR,           "TAG_ALREADY_EXIST_ERROR");         // 14
        emap.put(NULL_TIMESLOT_ERROR,               "NULL_TIMESLOT_ERROR");             // 15
        emap.put(UNKNOWN_ERROR,                     "UNKNOWN_ERROR");                   // 16
        emap.put(TMP_USER_ALREADY_EXIST_ERROR,      "TMP_USER_ALREADY_EXIST_ERROR");    // 17
        emap.put(NETWORK_ERROR,                     "NETWORK_ERROR");                   // 18
        emap.put(EMAIL_ALREADY_EXIST_ERROR,         "EMAIL_ALREADY_EXIST_ERROR");       // 19
    }
	
	public static String getErrorByCode(int error) {
		return emap.get(error);
	}

}
