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

class TestAddChannel(TestTemplate):
    
    authToken = "unknown"
    name = "testChannel_"+ str(datetime.now())
    description = "test_description"
    url = "url"
    radius = 0.0
   
    def execute(self, context, testDir):
        log = []
        server = context['server']

        parser = ConfigParser.SafeConfigParser()
        parser.read(testDir + "/tests.conf")
        section = "Tests_Params";
        self.authToken = parser.get(section, "auth_token")

        # work with JSON 
        jdata = json.dumps({"auth_token":self.authToken, "name":self.name, "description":self.description, "url":self.url, "activeRadius":self.radius})

        log.append(str(jdata))
        
        resData = urllib2.urlopen(server + "/service/addChannel", jdata)
        response = json.loads(resData.read())
        log.append(str(response))

        config = ConfigParser.ConfigParser()
        config.read(testDir + '/tests.conf')
        config.set('Tests_Params', 'channel_name', self.name)
        with open('tests/tests.conf', 'w') as configFile:
            config.write(configFile)
        
        result = response['errno'] == 0

        return (result, log)

    def isEnabled(self): return True

    def getName(self): 
        return 'TestAddChannel'

    def getDescription(self): 
        return "TestAddChannel: 1) send addChannel request 2) parse response 3) analyze data"
