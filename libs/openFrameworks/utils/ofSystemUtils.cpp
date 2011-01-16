
#include "ofConstants.h"
#include "ofSystemUtils.h"
#include "ofFileUtils.h"

#ifdef TARGET_WIN32
#include <winuser.h>
#include <commdlg.h>
#define _WIN32_DCOM

#include <windows.h>
#include <shlobj.h>
#include <tchar.h>
#include <stdio.h>

#endif

#ifdef TARGET_OSX
	#include <Carbon/Carbon.h>
	#include <sys/param.h> // for MAXPATHLEN
#endif 

#ifdef TARGET_WIN32
#include <locale>
#include <sstream>
#include <string>

std::string convertWideToNarrow( const wchar_t *s, char dfault = '?', 
                      const std::locale& loc = std::locale() )
{
  std::ostringstream stm;

  while( *s != L'\0' ) {
    stm << std::use_facet< std::ctype<wchar_t> >( loc ).narrow( *s++, dfault );
  }
  return stm.str();
}
#endif

#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
#include <gtk/gtk.h>
static gboolean closeGTK(GtkWidget *widget){
	//gtk_widget_destroy(widget);
    gtk_main_quit();
    return (FALSE);
}
static void initGTK(){
	int argc=0; char **argv = NULL;
	gtk_init (&argc, &argv);

}
static void startGTK(GtkWidget *dialog){
	gtk_init_add( (GSourceFunc) closeGTK, NULL );
	gtk_quit_add_destroy(1,GTK_OBJECT(dialog));
	//g_timeout_add(10, (GSourceFunc) destroyWidgetGTK, (gpointer) dialog);
	gtk_main();
}

static string gtkFileDialog(GtkFileChooserAction action,string windowTitle,string defaultName=""){
	initGTK();
	string results;

	const gchar* button_name;
	switch(action){
	case GTK_FILE_CHOOSER_ACTION_OPEN:
		button_name = GTK_STOCK_OPEN;
		break;
	case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER:
		button_name = GTK_STOCK_SELECT_ALL;
		break;
	case GTK_FILE_CHOOSER_ACTION_SAVE:
		button_name = GTK_STOCK_SAVE;
		break;
	}

	GtkWidget *dialog = gtk_file_chooser_dialog_new (windowTitle.c_str(),
						  NULL,
						  action,
						  button_name, GTK_RESPONSE_ACCEPT,
						  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
						  NULL);

	gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),defaultName.c_str());

	if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
		results = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
	}
	startGTK(dialog);
	return results;
}

#endif


//------------------------------------------------------------------------------
ofFileDialogResult::ofFileDialogResult(){
	filePath = "";
	fileName = "";
	bSuccess = false;
}

//------------------------------------------------------------------------------
string ofFileDialogResult::getName(){
	return fileName;
}

//------------------------------------------------------------------------------
string ofFileDialogResult::getPath(){
	return filePath;
}


//------------------------------------------------------------------------------
void ofCreateAlertDialog(string errorMessage){	
	
	
	#ifdef TARGET_WIN32
		// we need to convert error message to a wide char message. 
		// first, figure out the length and allocate a wchar_t at that length + 1 (the +1 is for a terminating character)
		int length = strlen(errorMessage.c_str());
		wchar_t * widearray = new wchar_t[length+1];
		memset(widearray, 0, sizeof(wchar_t)*(length+1));
		// then, call mbstowcs: 
		// http://www.cplusplus.com/reference/clibrary/cstdlib/mbstowcs/
		mbstowcs(widearray, errorMessage.c_str(), length);
		// launch the alert: 
		MessageBox(NULL, widearray, L"alert", MB_OK);
		// clear the allocated memory:
		delete widearray;
	#endif
	
	
	#ifdef TARGET_OSX
		CFStringRef msgStrRef = CFStringCreateWithCString(NULL, errorMessage.c_str(), kCFStringEncodingASCII);
		DialogRef theItem;
		DialogItemIndex itemIndex;
		CreateStandardAlert(kAlertNoteAlert, msgStrRef, NULL, NULL, &theItem);
		RunStandardAlert(theItem, NULL, &itemIndex);
	#endif
	
	#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
		initGTK();
		GtkWidget* dialog = gtk_message_dialog_new (NULL, (GtkDialogFlags) 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, errorMessage.c_str());
		gtk_dialog_run (GTK_DIALOG (dialog));
		startGTK(dialog);
	#endif
}


//----------------------------------------------------------------------------------------
//------------------------------------------------------------------------------       OSX
//----------------------------------------------------------------------------------------
#ifdef TARGET_OSX
//---------------------------------------------------------------------
// Gets a file to open from the user. Caller must release the CFURLRef.
CFURLRef GetOpenFileFromUser(bool bFolder)
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
	dialogOptions.optionFlags != kNavAllowOpenPackages;
	
	// Create the dialog
	if( bFolder ){
		status = NavCreateChooseFolderDialog(&dialogOptions, NULL, NULL, NULL, &dialog);
	}else{
		status = NavCreateGetFileDialog(&dialogOptions, NULL, NULL, NULL, NULL, NULL, &dialog);
	}
	
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
	//TODO: for multiple files - 1 specifies index	
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
#endif
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------



// OS specific results here.  "" = cancel or something bad like can't load, can't save, etc...
ofFileDialogResult ofFileLoadDialog(string windowTitle, bool bFolderSelection){
	
	ofFileDialogResult results;
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------       OSX
	//----------------------------------------------------------------------------------------
#ifdef TARGET_OSX
	CFURLRef cfUrl = GetOpenFileFromUser(bFolderSelection);
	
	CFStringRef cfString = NULL;
		
	if ( cfUrl != NULL ){
		cfString = CFURLCopyFileSystemPath( cfUrl, kCFURLPOSIXPathStyle );
		CFRelease( cfUrl );
		
		// copy from a CFString into a local c string (http://www.carbondev.com/site/?page=CStrings+)
		const int kBufferSize = MAXPATHLEN;
		
		char fileUrl[kBufferSize];
		Boolean bool1 = CFStringGetCString(cfString,fileUrl,kBufferSize,kCFStringEncodingMacRoman);
		
		//char fileName[kBufferSize];
		//Boolean bool2 = CFStringGetCString(reply.saveFileName,fileName,kBufferSize,kCFStringEncodingMacRoman);
		
		// append strings together
		
		results.filePath = fileUrl;
	}
		
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#ifdef TARGET_WIN32
	
	// TODO pc file choose dialog is now mega broken, please fix!

	if (bFolderSelection == false){
		
		wchar_t szFileName[MAX_PATH] = L"";
		OPENFILENAME ofn;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		HWND hwnd = WindowFromDC(wglGetCurrentDC());
		ofn.hwndOwner = hwnd;
		ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0");
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = 0;
	
		if(GetOpenFileName(&ofn)) {
			results.filePath = convertWideToNarrow(szFileName);
			// TODO: name conversion, please!! 
		}


		

	} else {

		BROWSEINFO      bi;
		wchar_t         wideCharacterBuffer[MAX_PATH]; 
		LPITEMIDLIST    pidl; 
		LPMALLOC		lpMalloc;

		// Get a pointer to the shell memory allocator
		if(SHGetMalloc(&lpMalloc) != S_OK){
			//TODO: deal with some sort of error here?
		}
		bi.hwndOwner        =   NULL; 
		bi.pidlRoot         =   NULL;
		bi.pszDisplayName   =   wideCharacterBuffer; 
		bi.lpszTitle        =   _T("Select Directory"); 
		bi.ulFlags          =   BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS; 
		bi.lpfn             =   NULL; 
		bi.lParam           =   0;
	
		if(pidl = SHBrowseForFolder(&bi)){
			// Copy the path directory to the buffer
			if(SHGetPathFromIDList(pidl,wideCharacterBuffer)){
				results.filePath = convertWideToNarrow(wideCharacterBuffer);
			}
			lpMalloc->Free(pidl);
		}
		lpMalloc->Release();
	}

	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#endif
	



	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   linux
	//----------------------------------------------------------------------------------------
#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
		if(bFolderSelection) results.filePath = gtkFileDialog(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,windowTitle);
		else results.filePath = gtkFileDialog(GTK_FILE_CHOOSER_ACTION_OPEN,windowTitle);
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------



	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFileUtils::getFilenameFromPath(results.filePath);		
	}
	
	return results;
}



ofFileDialogResult ofFileSaveDialog(string defaultName, string messageName){
	
	ofFileDialogResult results;
	
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
	
	//printf("NavCreatePutFileDialog returned %i\n", err );
	
	err = NavDialogRun(dialog);
	//printf("NavDialogRun returned %i\n", err );
	
	NavUserAction action;
	action = NavDialogGetUserAction( dialog );
	//printf("got action %i\n", action);
	if (action == kNavUserActionNone || action == kNavUserActionCancel) {
		
		return results;
	}
	
	// get dialog reply
	NavReplyRecord reply;
	err = NavDialogGetReply(dialog, &reply);
	if ( err != noErr )
		return results;
	
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
	
	results.filePath = finalURL.c_str();
	
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
	
	
	wchar_t fileName[MAX_PATH] = L"";
	char * extension;
	OPENFILENAME ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	HWND hwnd = WindowFromDC(wglGetCurrentDC());
	ofn.hwndOwner = hwnd;
	ofn.hInstance = GetModuleHandle(0);
	ofn.nMaxFileTitle = 31;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = _T("All Files (*.*)\0*.*\0");
	ofn.lpstrDefExt = _T("");	// we could do .rxml here?
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	ofn.lpstrTitle = _T("Select Output File");

	if (GetSaveFileName(&ofn)){
		results.filePath = convertWideToNarrow(fileName);
	}

#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------


	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   linux
	//----------------------------------------------------------------------------------------
#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)

	gtkFileDialog(GTK_FILE_CHOOSER_ACTION_SAVE, messageName,defaultName);

#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------

	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFileUtils::getFilenameFromPath(results.filePath);		
	}
	
	return results;	
}

