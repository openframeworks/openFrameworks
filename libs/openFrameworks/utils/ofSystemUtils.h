#pragma once


class ofFileDialogResult{
	public:
		ofFileDialogResult();
		
    		vector<string> results;
		string getName();
		string getPath();
		
		string filePath;
		string fileName;
		bool bSuccess;
};

void ofSystemAlertDialog(string errorMessage,string alertTitle = "alert");
ofFileDialogResult ofSystemLoadDialog(string windowTitle="", bool bFolderSelection = true, string defaultPath="",bool multipleItems=false);
ofFileDialogResult ofSystemSaveDialog(string defaultName, string messageName);
string ofSystemTextBoxDialog(string question, string text="");
string ofSystemPassword(string password,string initialPass="");
bool ofSystemYesNoDialog(string question,string alerttitle="?");


string ofEncodeToUtf8(string item);
