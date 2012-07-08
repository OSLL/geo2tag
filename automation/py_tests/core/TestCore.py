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
            self.testProcess(tInstance, context, writer, testDir)
    
    def testProcess(self, test, context, writer, testDir):
        name = test.getName()
        enabled = test.isEnabled()
        description = test.getDescription()
        
        if enabled:
            result = False
            log = []
            try:
                result, log = test.execute(context, testDir)
            except Exception:
                log.append('Test Execution Fail!')
            writer.addTestItem(name, description, enabled, result, log)
        else:
            print 'test', test.getDescription(), 'is disabled!'
        
            
            
