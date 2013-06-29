#pragma once

#include "ofConstants.h"
#include <Poco/Pipe.h>
#include <Poco/PipeStream.h>
#include <Poco/Process.h>

// ofSystemDialog ----------------------------------

class ofFileDialogResult{
	public:
		ofFileDialogResult();
		
		//TODO: only 1 file for now
		string getName();
		string getPath();
		
		string filePath;
		string fileName;
		bool bSuccess;
};

void               ofSystemAlertDialog( string errorMessage );
ofFileDialogResult ofSystemLoadDialog( string windowTitle="", bool bFolderSelection = false, string defaultPath="" );
ofFileDialogResult ofSystemSaveDialog( string defaultName, string messageName );
string             ofSystemTextBoxDialog( string question, string text="" );

// ofSystem ----------------------------------------

typedef Poco::Process::Args ofSystemArgs;

class ofSystemResult {
public:
	ofSystemResult() 
		: result("")
		, exitCode(0) {
		
	}
	
	string getResult();
	int getExitCode();
	
	string result;
	int    exitCode;
};

ofSystemResult ofSystem( string command, ofSystemArgs& args );