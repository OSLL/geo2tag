'''
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
 
 @author: OSLL team
'''

from core.TestTemplate import TestTemplate
from datetime import datetime
import urllib2
import json

class TestWriteTag(TestTemplate):
    
    authToken = "unknown"
    channel = "test_channel"
    description = "test_description"
    altitude = 0.0
    latitude = 0.0
    link = "link"
    longitude = 0.0
    time = "01 01 2012 12:00:00.000"
    title = "test_tag"
   
    def execute(self, context):
        log = []
        server = context['server']

        # work with JSON 
        jdata = json.dumps({"auth_token":self.authToken, "channel":self.channel, "description":self.description , "altitude":self.altitude, "latitude": self.latitude, "link":self.link, "longitude":self.longitude, "time":self.time, "title":self.title})

        log.append(str(jdata))
        
        resData = urllib2.urlopen(server + "/service/writeTag", jdata)
        response = json.loads(resData.read())
        log.append(str(response))
        
        result = response['errno'] == 0

        return (result, log)

    def isEnabled(self): return True

    def getName(self): 
        return 'TestWriteTag'

    def getDescription(self): 
        return "TestWriteTag: 1) send writeTag request 2) parse response 3) analyze data"
