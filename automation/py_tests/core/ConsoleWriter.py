'''
Created on Jun 16, 2012

@author: bac1ca
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
            
      
      

        
