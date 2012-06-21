'''
Created on Jun 12, 2012

@author: bac1ca
'''
from TestUtil import getTests, forName, testName

class TestCore(object):
    
    obj = None                           
    def __new__(cls, *dt, **mp):
        if cls.obj is None:
            cls.obj = object.__new__(cls, *dt, **mp) 
        return cls.obj  
    
    def __init__(self):
        pass
    
    def runTests(self, context, writer):
        testDir = context['test_dir']
        for test in getTests(testDir):
            tInstance = forName(testName(test))
            self.testProcess(tInstance, context, writer)
    
    def testProcess(self, test, context, writer):
        name = test.getName()
        enabled = test.isEnabled()
        description = test.getDescription()
        
        if enabled:
            result = False
            log = []
            try:
                result, log = test.execute(context)
            except Exception:
                log.append('Test Execution Fail!')
            writer.addTestItem(name, description, enabled, result, log)
        else:
            print 'test', test.getDescription(), 'is disabled!'
        
            
            
