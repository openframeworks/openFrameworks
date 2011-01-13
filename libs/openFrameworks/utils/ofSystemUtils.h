#pragma once


class ofFileDialogResults{
	public:
		ofFileDialogResults();
		
		//TODO: only 1 file for now
		string getName();
		string getPath();
		
		string filePath;
		string fileName;
		bool bSuccess;
};

void ofCreateAlertDialog(string errorMessage);
ofFileDialogResults ofFileLoadDialog(bool bFolderSelection = false);
ofFileDialogResults ofFileSaveDialog(string defaultName, string messageName);