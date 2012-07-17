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
import ConfigParser

class TestFilterFence(TestTemplate):
    
    authToken = "unknown"
    timeFrom = "2012 01 01 12:00:00.000"
    timeTo = "2012 01 13 12:00:00.000"
    number1 = 0
    latitude1 = 0.0
    longitude1 = 0.0
    number2 = 1
    latitude2 = 70.0
    longitude2 = 0.0
    number3 = 2
    latitude3 = 70.0
    longitude3 = 100.0
    altitude1 = -1.0
    altitude2 = 1.0
   
    def execute(self, context, testDir):
        log = []
        server = context['server']

        parser = ConfigParser.SafeConfigParser()
        parser.read(testDir + "/tests.conf")
        section = "Tests_Params";
        self.authToken = parser.get(section, "auth_token")

        # work with JSON 
        jdata = json.dumps({"auth_token":self.authToken, "time_from":self.timeFrom, "time_to":self.timeTo, "polygon":[{"number":self.number1,"latitude":self.latitude1,"longitude":self.longitude1}, {"number":self.number2,"latitude":self.latitude2,"longitude":self.longitude2}, {"number":self.number3,"latitude":self.latitude3,"longitude":self.longitude3}], "altitude_shift":{"altitude1":self.altitude1, "altitude2":self.altitude2}})

        log.append(str(jdata))
        
        resData = urllib2.urlopen(server + "/service/filterFence", jdata)
        response = json.loads(resData.read())
        log.append(str(response))
        
        result = response['errno'] == 0

        return (result, log)

    def isEnabled(self): return True

    def getName(self): 
        return 'TestFilterFence'

    def getDescription(self): 
        return "TestFilterFence: 1) send filterFence request 2) parse response 3) analyze data"
