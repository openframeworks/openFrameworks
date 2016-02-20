#pragma once
#include "ofConstants.h"

/// file dialog box selection or save result
class ofFileDialogResult{
	public:
		ofFileDialogResult();
		
		/// \returns the name of the selected file, or folder, if set
		/// currently returns only 1 file, this may change in the future
		string getName();
	
		/// \returns the full path of the selected file or folder, if set
		string getPath();
	
		string filePath; //< full path to selected file or folder
		string fileName; //< selected file or folder name
		bool bSuccess; //< true if the dialog action was successful, aka file select not cancel
};

/// show an error message in an alert dialog box
void ofSystemAlertDialog(string errorMessage);

/// show a file load dialog box with optional window title, folder selection, and
/// default folder path to open in
ofFileDialogResult ofSystemLoadDialog(string windowTitle="", bool bFolderSelection = false, string defaultPath="");

/// show a file save dialog with default file name & message text
ofFileDialogResult ofSystemSaveDialog(string defaultName, string messageName);

/// show a text entry dialog box with question text & optional default text
string ofSystemTextBoxDialog(string question, string text="");
