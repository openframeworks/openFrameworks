#pragma once


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

void ofCreateAlertDialog(string errorMessage);
ofFileDialogResult ofFileLoadDialog(string windowTitle="", bool bFolderSelection = false);
ofFileDialogResult ofFileSaveDialog(string defaultName, string messageName);
