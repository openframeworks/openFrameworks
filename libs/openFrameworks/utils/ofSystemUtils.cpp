
#include "ofMain.h"	
#include "ofSystemUtils.h"

#ifdef TARGET_WIN32
#include <winuser.h>
#include <commdlg.h>
#endif

void ofCreateAlertDialog(string errorMessage){	
	
	
	#ifdef TARGET_WIN32
		MessageBox(NULL, LPWSTR(errorMessage.c_str()), L"Critical Error", MB_OK | MB_ICONERROR);
	#endif
	
	
	#ifdef TARGET_OSX
		CFStringRef msgStrRef = CFStringCreateWithCString(NULL, errorMessage.c_str(), kCFStringEncodingASCII);
		DialogRef theItem;
		DialogItemIndex itemIndex;
		CreateStandardAlert(kAlertNoteAlert, msgStrRef, NULL, NULL, &theItem);
		RunStandardAlert(theItem, NULL, &itemIndex);
	#endif
	
}


//----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------       OSX
//----------------------------------------------------------------------------------------
#ifdef TARGET_OSX
//---------------------------------------------------------------------
// Gets a file to open from the user. Caller must release the CFURLRef.
CFURLRef GetOpenFileFromUser(void)
{
	NavDialogCreationOptions dialogOptions;
	NavDialogRef dialog;
	NavReplyRecord replyRecord;
	CFURLRef fileAsCFURLRef = NULL;
	FSRef fileAsFSRef;
	OSStatus status;
	
	// Get the standard set of defaults
	status = NavGetDefaultDialogCreationOptions(&dialogOptions);
	require_noerr( status, CantGetNavOptions );
	
	// Make the window app-wide modal
	dialogOptions.modality = kWindowModalityAppModal;
	
	// Create the dialog
	status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	require_noerr( status, CantCreateDialog );
	
	// Show it
	status = NavDialogRun(dialog);
	require_noerr( status, CantRunDialog );
	
	// Get the reply
	status = NavDialogGetReply(dialog, &replyRecord);
	require( ((status == noErr) || (status == userCanceledErr)), CantGetReply );
	
	// If the user clicked "Cancel", just bail
	if ( status == userCanceledErr ) goto UserCanceled;
	
	// Get the file
	status = AEGetNthPtr(&(replyRecord.selection), 1, typeFSRef, NULL, NULL, &fileAsFSRef, sizeof(FSRef), NULL);
	require_noerr( status, CantExtractFSRef );
	
	// Convert it to a CFURL
	fileAsCFURLRef = CFURLCreateFromFSRef(NULL, &fileAsFSRef);
	
	// Cleanup
CantExtractFSRef:
UserCanceled:
	verify_noerr( NavDisposeReply(&replyRecord) );
CantGetReply:
CantRunDialog:
	NavDialogDispose(dialog);
CantCreateDialog:
CantGetNavOptions:
    return fileAsCFURLRef;
}
#endif TARGET_OSX
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------



// OS specific results here.  "" = cancel or something bad like can't load, can't save, etc...
string ofFileLoadDialog(){
	
	
	string fileNameToLoad = "";
	
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------       OSX
	//----------------------------------------------------------------------------------------
#ifdef TARGET_OSX
	CFURLRef cfUrl = GetOpenFileFromUser();
	
	CFStringRef cfString = NULL;
	
	
	if ( cfUrl != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );
		
		// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
		const int kBufferSize = 255;
		
		char folderURL[kBufferSize];
		Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);
		
		//char fileName[kBufferSize];
		//Boolean bool2 = CFStringGetCString(reply.saveFileName,fileName,kBufferSize,kCFStringEncodingMacRoman);
		
		// append strings together
		
		string url1 = folderURL;
		string url2 = "";
		string finalURL = url1 + "/" + url2;
		fileNameToLoad = finalURL;
	}
	
	
	if (fileNameToLoad.length () < 1) return"";
	fileNameToLoad.erase(fileNameToLoad.length()-1);	// has a / at the end, ie /user/me/desktop/blah.txt/.... let's kill that :)
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#ifdef TARGET_WIN32
	
	char szFileName[MAX_PATH] = "";
	
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	HWND hwnd = WindowFromDC(wglGetCurrentDC());
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0");
	ofn.lpstrFile = LPWSTR(szFileName);
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = 0;
	
	if(GetOpenFileName(&ofn)) {
		printf("GetOpenFileName worked, got %s \n", szFileName);
		
		fileNameToLoad = string(szFileName);
	}
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#endif
	
	
	return fileNameToLoad;
	
}



string ofFileSaveDialog(string defaultName, string messageName){
	
	string fileNameOutput;
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------       OSX
	//----------------------------------------------------------------------------------------
#ifdef TARGET_OSX
	
	short fRefNumOut;
	FSRef output_file;
	OSStatus err;
	
	NavDialogCreationOptions options;
	NavGetDefaultDialogCreationOptions( &options );
	
	options.optionFlags = kNavNoTypePopup + kNavSupportPackages + kNavAllowOpenPackages;
	options.modality = kWindowModalityAppModal;
	
	options.optionFlags = kNavDefaultNavDlogOptions;
	options.message = CFStringCreateWithCString(NULL, messageName.c_str(), kCFStringEncodingASCII);;
	options.saveFileName = CFStringCreateWithCString(NULL, defaultName.c_str(), kCFStringEncodingASCII);
	NavDialogRef dialog;
	
	err = NavCreatePutFileDialog(&options, '.mov', 'Moov', NULL, NULL, &dialog);
	
	printf("NavCreatePutFileDialog returned %i\n", err );
	
	err = NavDialogRun(dialog);
	printf("NavDialogRun returned %i\n", err );
	
	NavUserAction action;
	action = NavDialogGetUserAction( dialog );
	printf("got action %i\n", action);
	if (action == kNavUserActionNone || action == kNavUserActionCancel) {
		
		return "";
	}
	
	// get dialog reply
	NavReplyRecord reply;
	err = NavDialogGetReply(dialog, &reply);
	if ( err != noErr )
		return "";
	
	if ( reply.replacing )
	{
		printf("need to replace\n");
	}
	
	AEKeyword keyword;
	DescType actual_type;
	Size actual_size;
	FSRef output_dir;
	err = AEGetNthPtr(&(reply.selection), 1, typeFSRef, &keyword, &actual_type,
					  &output_dir, sizeof(output_file), &actual_size);
	
	//printf("AEGetNthPtr returned %i\n", err );
	
	
	CFURLRef cfUrl = CFURLCreateFromFSRef( kCFAllocatorDefault, &output_dir );
	CFStringRef cfString = NULL;
	if ( cfUrl != NULL )
	{
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );
	}
	
	// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
	const int kBufferSize = 255;
	
	char folderURL[kBufferSize];
	Boolean bool1 = CFStringGetCString(cfString,folderURL,kBufferSize,kCFStringEncodingMacRoman);
	
	char fileName[kBufferSize];
	Boolean bool2 = CFStringGetCString(reply.saveFileName,fileName,kBufferSize,kCFStringEncodingMacRoman);
	
	// append strings together
	
	string url1 = folderURL;
	string url2 = fileName;
	string finalURL = url1 + "/" + url2;
	
	fileNameOutput =  finalURL.c_str();
	
	//printf("url %s\n", finalURL.c_str());
	
	// cleanup dialog
	NavDialogDispose(dialog);
	
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#ifdef TARGET_WIN32
	
	
	char fileName[260] = {0};
	fileName[0] = 0;
	static char info_fn[_MAX_PATH];		// hmmm what is this for?
	char *extension;
	OPENFILENAME ofn;
	
	
	
	//lstrcpy(fileName, info_fn);
	//extension = strrchr(fileName, '.');
	//lstrcpy(extension, ".mp4");
	//
    memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	HWND hwnd = WindowFromDC(wglGetCurrentDC());
	ofn.hwndOwner = hwnd;
	ofn.hInstance = GetModuleHandle(0);
	ofn.nMaxFileTitle = 31;
	ofn.lpstrFile = LPWSTR(fileName);
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0");
	ofn.lpstrDefExt = _T("");	// we could do .rxml here?
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	ofn.lpstrTitle = _T("Select Output File");
	//
	
	if (GetSaveFileName(&ofn))
	{
	    fileNameOutput = string(info_fn) + "/" + string(fileName);
		if (fileNameOutput.length() > 1) fileNameOutput.erase(0,1);
        cout << "GetSaveFileName worked, got " << fileNameOutput << endl;
		
		
	}
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	
	return fileNameOutput;
	
}