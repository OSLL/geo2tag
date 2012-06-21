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

from TestWriter import TestWriter

class ConsoleWriter(TestWriter):
    mOutput = ''
    
    mTestCount = 0
    mPassCount = 0
    
    def __init__(self):
        self.mTestCount = 0
        self.mPassCount = 0
        
    mFail = 'FAIL'
    mPass = 'PASS'
    mErrn = 'WEB SERVICE ERROR'
    mDsbl = 'DISABLED'

    def __logMessage__(self, log):
        logMess = "LOG:"
        for line in log:
            logMess = logMess + "\t" + line + '\n'
        return logMess

    def addTestItem(self, name, description, enabled, result, log):
        self.mOutput = self.mOutput + '\n----------------------------------------------\n'
        
        self.mTestCount += 1
        self.mOutput = self.mOutput + 'TEST: ' + name + '\n'
        if (enabled):
            self.mOutput = self.mOutput + "DESCRIPTION: " + description + '\n'
            if (result):
                self.mPassCount += 1
                self.mOutput = self.mOutput + 'STATUS: ' + self.mPass + '\n'
            else:
                self.mPassCount += 0
                self.mOutput = self.mOutput + 'STATUS: ' + self.mFail + '\n'
            self.mOutput = self.mOutput + self.__logMessage__(log)
        else:
            self.mPassCount += 1
            self.mOutput = self.mOutput + 'STATUS: ' + self.mDsbl + '\n'


    def getPage(self):
        sumInfo = ''
        sumInfo = sumInfo + '------------------SUMMARY---------------------\n'
        sumInfo = sumInfo + str(self.mTestCount) + ' Tests \n'
        sumInfo = sumInfo + str(self.mPassCount) + ' Passed\n'
        sumInfo = sumInfo + str(self.mTestCount-self.mPassCount) + ' Failed\n'
        return sumInfo + '\n\n' + self.mOutput
            
      
      

        
