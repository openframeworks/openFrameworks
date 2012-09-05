
#include "ofConstants.h"
#include "ofSystemUtils.h"
#include "ofFileUtils.h"
#include "ofLog.h"

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
	// ofSystemUtils.cpp is configured to build as
	// objective-c++ so as able to use NSAutoreleasePool.
	// This is done with this compiler flag
	//		-x objective-c++
	// http://www.yakyak.org/viewtopic.php?p=1475838&sid=1e9dcb5c9fd652a6695ac00c5e957822#p1475838

	#include <Carbon/Carbon.h>
	#include <sys/param.h> // for MAXPATHLEN
	#include <Cocoa/Cocoa.h>  // for NSAutoreleasePool
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

	const gchar* button_name = "";
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
	default:
		return "";
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
#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
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
void ofSystemAlertDialog(string errorMessage){


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
		MessageBoxW(NULL, widearray, L"alert", MB_OK);
		// clear the allocated memory:
		delete widearray;
	#endif


	#ifdef TARGET_OSX
		NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];  // The StandardAlert requires a NSAutoreleasePool to avoid memory leaks

		CFStringRef msgStrRef = CFStringCreateWithCString(NULL, errorMessage.c_str(), kCFStringEncodingASCII);
		DialogRef theItem;
		DialogItemIndex itemIndex;
		CreateStandardAlert(kAlertNoteAlert, msgStrRef, NULL, NULL, &theItem);
		RunStandardAlert(theItem, NULL, &itemIndex);

		[pool drain];
	#endif

	#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
		initGTK();
		GtkWidget* dialog = gtk_message_dialog_new (NULL, (GtkDialogFlags) 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", errorMessage.c_str());
		gtk_dialog_run (GTK_DIALOG (dialog));
		startGTK(dialog);
	#endif

	#ifdef TARGET_ANDROID
		ofxAndroidAlertBox(errorMessage);
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
ofFileDialogResult ofSystemLoadDialog(string windowTitle, bool bFolderSelection){

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
		CFRelease(cfString);
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

	if (bFolderSelection == false){

        OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		HWND hwnd = WindowFromDC(wglGetCurrentDC());
		ofn.hwndOwner = hwnd;
#ifdef __MINGW32_VERSION
		char szFileName[MAX_PATH];
		ofn.lpstrFilter = "All\0";
		ofn.lpstrFile = szFileName;
#else // VS2010
		wchar_t szFileName[MAX_PATH] = L"";
		ofn.lpstrFilter = L"All\0";
		ofn.lpstrFile = szFileName;
#endif
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = 0;

		if(GetOpenFileName(&ofn)) {
#ifdef __MINGW32_VERSION
			results.filePath = string(szFileName);
#else
			results.filePath = convertWideToNarrow(szFileName);
#endif

		}

	} else {

		BROWSEINFOW      bi;
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
		bi.lpszTitle        =   L"Select Directory";
		bi.ulFlags          =   BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
		bi.lpfn             =   NULL;
		bi.lParam           =   0;

		if(pidl = SHBrowseForFolderW(&bi)){
			// Copy the path directory to the buffer
			if(SHGetPathFromIDListW(pidl,wideCharacterBuffer)){
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
		results.fileName = ofFilePath::getFileName(results.filePath);
	}

	return results;
}



ofFileDialogResult ofSystemSaveDialog(string defaultName, string messageName){

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

	if ( reply.replacing ) {
		ofLog(OF_LOG_WARNING, "ofSystemSaveDialog: need to replace");
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
	OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	HWND hwnd = WindowFromDC(wglGetCurrentDC());
	ofn.hwndOwner = hwnd;
	ofn.hInstance = GetModuleHandle(0);
	ofn.nMaxFileTitle = 31;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = L"All Files (*.*)\0*.*\0";
	ofn.lpstrDefExt = L"";	// we could do .rxml here?
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;
	ofn.lpstrTitle = L"Select Output File";

	if (GetSaveFileNameW(&ofn)){
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

	results.filePath = gtkFileDialog(GTK_FILE_CHOOSER_ACTION_SAVE, messageName,defaultName);

#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------

	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFilePath::getFileName(results.filePath);
	}

	return results;
}

#ifdef TARGET_WIN32
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        /*case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;*/
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
#endif


string ofSystemTextBoxDialog(string question, string text){
#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
	initGTK();
	GtkWidget* dialog = gtk_message_dialog_new (NULL, (GtkDialogFlags) 0, GTK_MESSAGE_QUESTION, (GtkButtonsType) GTK_BUTTONS_OK_CANCEL, question.c_str() );
	GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	GtkWidget* textbox = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(textbox),text.c_str());
	gtk_container_add (GTK_CONTAINER (content_area), textbox);
	gtk_widget_show_all (dialog);
	if(gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_OK){
		text = gtk_entry_get_text(GTK_ENTRY(textbox));
	}
	startGTK(dialog);
#endif

#ifdef TARGET_OSX
	// create alert dialog
	NSAlert *alert = [[[NSAlert alloc] init] autorelease];
	[alert addButtonWithTitle:@"OK"];
	[alert addButtonWithTitle:@"Cancel"];
	[alert setMessageText:[NSString stringWithCString:question.c_str()
											 encoding:NSUTF8StringEncoding]];
	// create text field
	NSTextField* label = [[NSTextField alloc] initWithFrame:NSRectFromCGRect(CGRectMake(0,0,300,40))];
	[label setStringValue:[NSString stringWithCString:text.c_str()
											 encoding:NSUTF8StringEncoding]];
	// add text field to alert dialog
	[alert setAccessoryView:label];
	NSInteger returnCode = [alert runModal];
	// if OK was clicked, assign value to text
	if ( returnCode == NSAlertFirstButtonReturn )
		text = [[label stringValue] UTF8String];
#endif

#ifdef TARGET_WIN32
    // we need to convert error message to a wide char message.
    // first, figure out the length and allocate a wchar_t at that length + 1 (the +1 is for a terminating character)

    WNDCLASSEX wc;
    MSG Msg;
    const char g_szClassName[] = "myWindowClass";
    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = GetModuleHandle(0);
    wc.lpszClassName = (LPCWSTR)g_szClassName;
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wc.lpszMenuName  = NULL;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, (LPCWSTR)"Window Registration Failed!", (LPCWSTR)"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return text;
    }

    HWND dialog = CreateWindowEx(WS_EX_DLGMODALFRAME,
        (LPCWSTR)g_szClassName,
        (LPCWSTR)question.c_str(),
        WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 240, 140,
        WindowFromDC(wglGetCurrentDC()), NULL, GetModuleHandle(0),NULL);

    if(dialog == NULL)
    {
        MessageBox(NULL,(LPCWSTR)"Window Creation Failed!", (LPCWSTR)"Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return text;
    }

    EnableWindow(WindowFromDC(wglGetCurrentDC()), FALSE);
    HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCWSTR)"EDIT", (LPCWSTR)text.c_str(),
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        10, 10, 210, 40, dialog, (HMENU)101, GetModuleHandle(NULL), NULL);


    HWND okButton = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCWSTR)"BUTTON", (LPCWSTR)"OK",
        WS_CHILD | WS_VISIBLE | WS_TABSTOP,
        10, 60, 60, 30, dialog, (HMENU)IDOK, GetModuleHandle(NULL), NULL);

    HWND cancelButton = CreateWindowEx(WS_EX_CLIENTEDGE, (LPCWSTR)"BUTTON", (LPCWSTR)"Cancel",
        WS_CHILD | WS_VISIBLE,
        80, 60, 60, 30, dialog, (HMENU)IDCANCEL, GetModuleHandle(NULL), NULL);

    SetFocus( hEdit );

    ShowWindow(dialog, SW_SHOWNORMAL);
    bool bFirstEmpty;
    while (true){
         if (!PeekMessageW( &Msg, 0, 0, 0, PM_REMOVE )){
             if (bFirstEmpty){
                 // ShowWindow the first time the queue goes empty
                 ShowWindow( dialog, SW_SHOWNORMAL );
                 bFirstEmpty = FALSE;
             }
             if (!(GetWindowLongW( dialog, GWL_STYLE ) & DS_NOIDLEMSG)){
                 // No message present -> send ENTERIDLE and wait
                 SendMessageW( WindowFromDC(wglGetCurrentDC()), WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)dialog );
             }
             GetMessageW( &Msg, 0, 0, 0 );
         }

         if (Msg.message == WM_QUIT){
             PostQuitMessage( Msg.wParam );
             if (!IsWindow( dialog )){
                EnableWindow(WindowFromDC(wglGetCurrentDC()), TRUE);
                return text;
             }
             break;
         }

         if (!IsWindow( dialog )){
            EnableWindow(WindowFromDC(wglGetCurrentDC()), TRUE);
            return text;
         }

         TranslateMessage( &Msg );
         DispatchMessageW( &Msg );

         if((Msg.hwnd == okButton && Msg.message==WM_LBUTTONUP) || (Msg.message==WM_KEYUP && Msg.wParam==13)){
             break;
         }else if((Msg.hwnd == cancelButton && Msg.message==WM_LBUTTONUP) ||  (Msg.message==WM_KEYUP && Msg.wParam==27)){
             EnableWindow(WindowFromDC(wglGetCurrentDC()), TRUE);
             DestroyWindow(dialog);
             return text;
         }

         if (!IsWindow( dialog )){
            EnableWindow(WindowFromDC(wglGetCurrentDC()), TRUE);
            return text;
         }

         if (bFirstEmpty && Msg.message == WM_TIMER){
             ShowWindow( dialog, SW_SHOWNORMAL );
             bFirstEmpty = FALSE;
         }
     }
     char buf[16384];
     GetDlgItemTextA( dialog, 101, buf, 16384 );
     text = buf;
     DestroyWindow(dialog);
     EnableWindow(WindowFromDC(wglGetCurrentDC()), TRUE);
#endif

#ifdef TARGET_ANDROID
     text = ofxAndroidAlertTextBox(question,text);
#endif

	return text;
}
