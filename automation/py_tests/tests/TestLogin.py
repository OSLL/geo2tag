'''
 * Copyright 2012  OSLL Vasily Romanikhin  bac1ca89@gmail.com
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
 
 @author: Vasily Romanikhin bac1ca89@gmail.com
'''

from core.TestTemplate import TestTemplate
import urllib2
import json
import ConfigParser

class TestLogin(TestTemplate):
        
    login = "test_user"
    password = "test"

    def execute(self, context, testDir):
        log = []
        server = context['server']

        parser = ConfigParser.SafeConfigParser()
        parser.read(testDir + "/tests.conf")
        section = "Tests_Params";
        self.login = parser.get(section, "user_login")
        self.password = parser.get(section, "user_password")

        # work with JSON 
        jdata = json.dumps({'login':self.login, 'password':self.password})
        log.append(str(jdata))
        
        resData = urllib2.urlopen(server + "/service/login", jdata)
        response = json.loads(resData.read())
        log.append(str(response))
        
        config = ConfigParser.ConfigParser()
        config.read(testDir + '/tests.conf')
        config.set('Tests_Params', 'auth_token', response['auth_token'])
        with open('tests/tests.conf', 'w') as configFile:
            config.write(configFile)

        result = response['errno'] == 0

        return (result, log)

    def isEnabled(self): return True

    def getName(self): 
        return 'TestLogin'

    def getDescription(self): 
        return "TestLogin: 1) send login request 2) parse response 3) analyze data"
