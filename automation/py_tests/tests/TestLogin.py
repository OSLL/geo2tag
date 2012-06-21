'''
Created on Jun 12, 2012

@author: bac1ca
'''
from core.TestTemplate import TestTemplate
import urllib2
import json

class TestLogin(TestTemplate):
        
    def execute(self, context):
        log = []
        server = context['server']

        # work with JSON 
        jdata = json.dumps({'login':'Mark', 'password':'test'})
        log.append(str(jdata))
        
        resData = urllib2.urlopen(server + "/service/login", jdata)
        response = json.loads(resData.read())
        log.append(str(response))
        
        result = response['errno'] == 0
        return (result, log)

    def isEnabled(self): return True

    def getName(self): 
        return 'TestLogin'

    def getDescription(self): 
        return "TestLogin: 1) send login request 2) parse response 3) analyze data"
