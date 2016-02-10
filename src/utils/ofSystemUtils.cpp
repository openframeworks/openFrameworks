
#include "ofConstants.h"
#include "ofSystemUtils.h"
#include "ofFileUtils.h"
#include "ofLog.h"
#include "ofUtils.h"
#include "ofAppRunner.h"
#include <condition_variable>
#include <mutex>

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
	// objective-c++ so as able to use Cocoa dialog panels
	// This is done with this compiler flag
	//		-x objective-c++
	// http://www.yakyak.org/viewtopic.php?p=1475838&sid=1e9dcb5c9fd652a6695ac00c5e957822#p1475838

	#include <Cocoa/Cocoa.h>
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

std::wstring convertNarrowToWide( const std::string& as ){
    // deal with trivial case of empty string
    if( as.empty() )    return std::wstring();

    // determine required length of new string
    size_t reqLength = ::MultiByteToWideChar( CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0 );

    // construct new string of required length
    std::wstring ret( reqLength, L'\0' );

    // convert old string to new string
    ::MultiByteToWideChar( CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length() );

    // return new string ( compiler should optimize this away )
    return ret;
}

#endif

#if defined( TARGET_OSX )
static void restoreAppWindowFocus(){
	NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
	if(appWindow) {
		[appWindow makeKeyAndOrderFront:nil];
	}
}
#endif

#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
#include <gtk/gtk.h>
#include "ofGstUtils.h"
#include "Poco/Condition.h"

#if GTK_MAJOR_VERSION>=3
#define OPEN_BUTTON "_Open"
#define SELECT_BUTTON "_Select All"
#define SAVE_BUTTON "_Save"
#define CANCEL_BUTTON "_Cancel"
#else
#define OPEN_BUTTON GTK_STOCK_OPEN
#define SELECT_BUTTON GTK_STOCK_SELECT_ALL
#define SAVE_BUTTON GTK_STOCK_SAVE
#define CANCEL_BUTTON GTK_STOCK_CANCEL
#endif

gboolean init_gtk(gpointer userdata){
	int argc=0; char **argv = nullptr;
	gtk_init (&argc, &argv);

	return FALSE;
}

struct FileDialogData{
	GtkFileChooserAction action;
	string windowTitle;
	string defaultName;
	string results;
	bool done;
	Poco::Condition condition;
	std::mutex mutex;
};

gboolean file_dialog_gtk(gpointer userdata){
	FileDialogData * dialogData = (FileDialogData*)userdata;
	const gchar* button_name = nullptr;
	switch(dialogData->action){
	case GTK_FILE_CHOOSER_ACTION_OPEN:
		button_name = OPEN_BUTTON;
		break;
	case GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER:
		button_name = SELECT_BUTTON;
		break;
	case GTK_FILE_CHOOSER_ACTION_SAVE:
		button_name = SAVE_BUTTON;
		break;
	default:
		break;
	}

	if(button_name!=nullptr){
		GtkWidget *dialog = gtk_file_chooser_dialog_new (dialogData->windowTitle.c_str(),
							  nullptr,
							  dialogData->action,
							  button_name, GTK_RESPONSE_ACCEPT,
							  CANCEL_BUTTON, GTK_RESPONSE_CANCEL,
							  nullptr);

		gtk_file_chooser_set_filename(GTK_FILE_CHOOSER(dialog),dialogData->defaultName.c_str());

		if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
			dialogData->results = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		}
		gtk_widget_destroy (dialog);
	}

	dialogData->mutex.lock();
	dialogData->condition.signal();
	dialogData->done = true;
	dialogData->mutex.unlock();
	return FALSE;
}

struct TextDialogData{
	string text;
	string question;
	bool done;
	std::condition_variable condition;
	std::mutex mutex;
};

gboolean alert_dialog_gtk(gpointer userdata){
	TextDialogData * dialogData = (TextDialogData*)userdata;
	GtkWidget* dialog = gtk_message_dialog_new (nullptr, (GtkDialogFlags) 0, GTK_MESSAGE_INFO, GTK_BUTTONS_OK, "%s", dialogData->text.c_str());
	gtk_widget_grab_focus(gtk_dialog_get_widget_for_response(GTK_DIALOG(dialog), GTK_RESPONSE_OK));
	gtk_dialog_run (GTK_DIALOG (dialog));
	gtk_widget_destroy (dialog);
	dialogData->mutex.lock();
	dialogData->condition.notify_all();
	dialogData->done = true;
	dialogData->mutex.unlock();

	return FALSE;
}

gboolean text_dialog_gtk(gpointer userdata){
	TextDialogData * dialogData = (TextDialogData*)userdata;
	GtkWidget* dialog = gtk_message_dialog_new (nullptr, (GtkDialogFlags) 0, GTK_MESSAGE_QUESTION, (GtkButtonsType) GTK_BUTTONS_OK_CANCEL, "%s", dialogData->question.c_str() );
	GtkWidget* content_area = gtk_dialog_get_content_area (GTK_DIALOG (dialog));
	GtkWidget* textbox = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(textbox),dialogData->text.c_str());
	gtk_container_add (GTK_CONTAINER (content_area), textbox);
	gtk_widget_show_all (dialog);
	if(gtk_dialog_run (GTK_DIALOG (dialog))==GTK_RESPONSE_OK){
		dialogData->text = gtk_entry_get_text(GTK_ENTRY(textbox));
	}
	gtk_widget_destroy (dialog);
	dialogData->mutex.lock();
	dialogData->condition.notify_all();
	dialogData->done = true;
	dialogData->mutex.unlock();

	return FALSE;
}

static void initGTK(){
	static bool initialized = false;
	if(!initialized){
		#if !defined(TARGET_RASPBERRY_PI) 
		XInitThreads();
		#endif
		int argc=0; char **argv = nullptr;
		gtk_init (&argc, &argv);
		ofGstUtils::startGstMainLoop();
		initialized = true;
	}

}

static string gtkFileDialog(GtkFileChooserAction action,string windowTitle,string defaultName=""){
	initGTK();
	FileDialogData dialogData;
	dialogData.action = action;
	dialogData.windowTitle = windowTitle;
	dialogData.defaultName = defaultName;
	dialogData.done = false;

	g_main_context_invoke(g_main_loop_get_context(ofGstUtils::getGstMainLoop()), &file_dialog_gtk, &dialogData);
	if(!dialogData.done){
		dialogData.mutex.lock();
		dialogData.condition.wait(dialogData.mutex);
	}
	return dialogData.results;
}

#endif

#ifdef TARGET_ANDROID
#include "ofxAndroidUtils.h"
#endif

#ifdef TARGET_EMSCRIPTEN
#include <emscripten/emscripten.h>
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
		MessageBoxW(nullptr, widearray, L"alert", MB_OK);
		// clear the allocated memory:
		delete widearray;
	#endif

	#ifdef TARGET_OSX
		@autoreleasepool {
			NSAlert* alertDialog = [[[NSAlert alloc] init] autorelease];
			alertDialog.messageText = [NSString stringWithUTF8String:errorMessage.c_str()];
			[alertDialog runModal];
			restoreAppWindowFocus();
		}
	#endif

	#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
		initGTK();
		TextDialogData dialogData;
		dialogData.text = errorMessage;
		dialogData.done = false;
		g_main_context_invoke(g_main_loop_get_context(ofGstUtils::getGstMainLoop()), &alert_dialog_gtk, &dialogData);
		if(!dialogData.done){
			std::unique_lock<std::mutex> lock(dialogData.mutex);
			dialogData.condition.wait(lock);
		}
	#endif

	#ifdef TARGET_ANDROID
		ofxAndroidAlertBox(errorMessage);
	#endif

	#ifdef TARGET_EMSCRIPTEN
		emscripten_run_script((string("alert(")+errorMessage+");").c_str());
	#endif
}

//----------------------------------------------------------------------------------------
#ifdef TARGET_WIN32
//---------------------------------------------------------------------
static int CALLBACK loadDialogBrowseCallback(
  HWND hwnd,
  UINT uMsg,
  LPARAM lParam,
  LPARAM lpData
){
    string defaultPath = *(string*)lpData;
    if(defaultPath!="" && uMsg==BFFM_INITIALIZED){
		wchar_t         wideCharacterBuffer[MAX_PATH];
		wcscpy(wideCharacterBuffer, convertNarrowToWide(ofToDataPath(defaultPath)).c_str());
        SendMessage(hwnd,BFFM_SETSELECTION,1,(LPARAM)wideCharacterBuffer);
    }

	return 0;
}
//----------------------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------

// OS specific results here.  "" = cancel or something bad like can't load, can't save, etc...
ofFileDialogResult ofSystemLoadDialog(string windowTitle, bool bFolderSelection, string defaultPath){

	ofFileDialogResult results;

	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------       OSX
	//----------------------------------------------------------------------------------------
#ifdef TARGET_OSX
	@autoreleasepool {
		NSOpenGLContext *context = [NSOpenGLContext currentContext];

		NSOpenPanel * loadDialog = [NSOpenPanel openPanel];
		[loadDialog setAllowsMultipleSelection:NO];
		[loadDialog setCanChooseDirectories:bFolderSelection];
		[loadDialog setResolvesAliases:YES];

		if(!windowTitle.empty()) {
			[loadDialog setTitle:[NSString stringWithUTF8String:windowTitle.c_str()]];
		}

		if(!defaultPath.empty()) {
			NSString * s = [NSString stringWithUTF8String:defaultPath.c_str()];
			s = [[s stringByExpandingTildeInPath] stringByResolvingSymlinksInPath];
			NSURL * defaultPathUrl = [NSURL fileURLWithPath:s];
			[loadDialog setDirectoryURL:defaultPathUrl];
		}

		NSInteger buttonClicked = [loadDialog runModal];
		[context makeCurrentContext];
		restoreAppWindowFocus();

		if(buttonClicked == NSFileHandlingPanelOKButton) {
			NSURL * selectedFileURL = [[loadDialog URLs] objectAtIndex:0];
			results.filePath = string([[selectedFileURL path] UTF8String]);
		}
	}
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#ifdef TARGET_WIN32
	wstring windowTitleW;
	windowTitleW.assign(windowTitle.begin(), windowTitle.end());

	if (bFolderSelection == false){

        OPENFILENAME ofn;

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		HWND hwnd = WindowFromDC(wglGetCurrentDC());
		ofn.hwndOwner = hwnd;

		//the file name and path
		wchar_t szFileName[MAX_PATH];
		memset(szFileName, 0, sizeof(szFileName));

		//the dir, if specified
		wchar_t szDir[MAX_PATH];

		//the title if specified
		wchar_t szTitle[MAX_PATH];
		if(defaultPath!=""){
			wcscpy(szDir,convertNarrowToWide(ofToDataPath(defaultPath)).c_str());
			ofn.lpstrInitialDir = szDir;
		}

		if (windowTitle != "") {
			wcscpy(szTitle, convertNarrowToWide(windowTitle).c_str());
			ofn.lpstrTitle = szTitle;
		} else {
			ofn.lpstrTitle = nullptr;
		}

		ofn.lpstrFilter = L"All\0";
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = MAX_PATH;
		ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
		ofn.lpstrDefExt = 0;
		ofn.lpstrTitle = windowTitleW.c_str();

		if(GetOpenFileName(&ofn)) {
			results.filePath = convertWideToNarrow(szFileName);
		}
		else {
			//this should throw an error on failure unless its just the user canceling out
			DWORD err = CommDlgExtendedError();
		}

	} else {

		BROWSEINFOW      bi;
		wchar_t         wideCharacterBuffer[MAX_PATH];
		wchar_t			wideWindowTitle[MAX_PATH];
		LPITEMIDLIST    pidl;
		LPMALLOC		lpMalloc;

		if (windowTitle != "") {
			wcscpy(wideWindowTitle, convertNarrowToWide(windowTitle).c_str());
		} else {
			wcscpy(wideWindowTitle, L"Select Directory");
		}

		// Get a pointer to the shell memory allocator
		if(SHGetMalloc(&lpMalloc) != S_OK){
			//TODO: deal with some sort of error here?
		}
		bi.hwndOwner        =   nullptr;
		bi.pidlRoot         =   nullptr;
		bi.pszDisplayName   =   wideCharacterBuffer;
		bi.lpszTitle        =   wideWindowTitle;
		bi.ulFlags          =   BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
		bi.lpfn             =   &loadDialogBrowseCallback;
		bi.lParam           =   (LPARAM) &defaultPath;
		bi.lpszTitle        =   windowTitleW.c_str();

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
		if(bFolderSelection) results.filePath = gtkFileDialog(GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,windowTitle,defaultPath);
		else results.filePath = gtkFileDialog(GTK_FILE_CHOOSER_ACTION_OPEN,windowTitle,defaultPath);
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
	@autoreleasepool {
		NSSavePanel * saveDialog = [NSSavePanel savePanel];
		NSOpenGLContext *context = [NSOpenGLContext currentContext];
		[saveDialog setMessage:[NSString stringWithUTF8String:messageName.c_str()]];
		[saveDialog setNameFieldStringValue:[NSString stringWithUTF8String:defaultName.c_str()]];

		NSInteger buttonClicked = [saveDialog runModal];
		restoreAppWindowFocus();
		[context makeCurrentContext];

		if(buttonClicked == NSFileHandlingPanelOKButton){
			results.filePath = string([[[saveDialog URL] path] UTF8String]);
		}
	}
#endif
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------   windoze
	//----------------------------------------------------------------------------------------
#ifdef TARGET_WIN32


	wchar_t fileName[MAX_PATH] = L"";
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
    //switch(msg)
    //{
    //    case WM_CLOSE:
    //        DestroyWindow(hwnd);
    //    break;
    //    case WM_DESTROY:
    //        PostQuitMessage(0);
    //    break;
    //    default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    //}
}
#endif


string ofSystemTextBoxDialog(string question, string text){
#if defined( TARGET_LINUX ) && defined (OF_USING_GTK)
	initGTK();
	TextDialogData dialogData;
	dialogData.text = text;
	dialogData.done = false;
	dialogData.question = question;
	g_main_context_invoke(g_main_loop_get_context(ofGstUtils::getGstMainLoop()), &text_dialog_gtk, &dialogData);
	if(!dialogData.done){
		std::unique_lock<std::mutex> lock(dialogData.mutex);
		dialogData.condition.wait(lock);
	}
	text = dialogData.text;
#endif

#ifdef TARGET_OSX
	@autoreleasepool {
		// create alert dialog
		NSAlert *alert = [[[NSAlert alloc] init] autorelease];
		[alert addButtonWithTitle:@"OK"];
		[alert addButtonWithTitle:@"Cancel"];
		[alert setMessageText:[NSString stringWithCString:question.c_str()
												 encoding:NSUTF8StringEncoding]];
		// create text field
		NSTextField* label = [[[NSTextField alloc] initWithFrame:NSRectFromCGRect(CGRectMake(0,0,300,40))] autorelease];
		[label setStringValue:[NSString stringWithCString:text.c_str()
												 encoding:NSUTF8StringEncoding]];
		// add text field to alert dialog
		[alert setAccessoryView:label];
		NSInteger returnCode = [alert runModal];
		restoreAppWindowFocus();
		// if OK was clicked, assign value to text
		if ( returnCode == NSAlertFirstButtonReturn )
			text = [[label stringValue] UTF8String];
	}
#endif

#ifdef TARGET_WIN32
    // we need to convert error message to a wide char message.
    // first, figure out the length and allocate a wchar_t at that length + 1 (the +1 is for a terminating character)

	WNDCLASSEX wc;
	MSG Msg;

        #define TMP_STR_CONVERT LPCWSTR

		const LPCWSTR g_szClassName = L"myWindowClass\0";

		//Step 1: Registering the Window Class
		wc.cbSize        = sizeof(WNDCLASSEX);
		wc.style         = CS_HREDRAW | CS_VREDRAW;
		wc.lpfnWndProc   = WndProc;
		wc.cbClsExtra    = 0;
		wc.cbWndExtra    = 0;
		wc.hInstance     = GetModuleHandle(0);
		wc.lpszClassName = g_szClassName;
		wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wc.lpszMenuName  = nullptr;
		wc.hCursor       = LoadCursor(nullptr, IDC_ARROW);
		wc.hIcon         = LoadIcon(nullptr, IDI_APPLICATION);
		wc.hIconSm       = LoadIcon(nullptr, IDI_APPLICATION);
		if(!RegisterClassEx(&wc)){
			DWORD err=GetLastError();
			if ((err==ERROR_CLASS_ALREADY_EXISTS)){
                ; // we are ok
                // http://stackoverflow.com/questions/5791996/re-registering-user-defined-window-class-c
            } else {
			MessageBox(nullptr, L"Window Registration Failed!\0", L"Error!\0",
				MB_ICONEXCLAMATION | MB_OK);
			return text;
		}
		}

		HWND dialog = CreateWindowEx(WS_EX_DLGMODALFRAME,
			g_szClassName,
			convertNarrowToWide(question).c_str(),
			WS_POPUP | WS_CAPTION | DS_MODALFRAME | WS_SYSMENU,
			CW_USEDEFAULT, CW_USEDEFAULT, 240, 140,
			WindowFromDC(wglGetCurrentDC()), nullptr, GetModuleHandle(0),nullptr);

		if(dialog == nullptr)
		{
			
			MessageBox(nullptr,L"Window Creation Failed!\0", L"Error!\0",
				MB_ICONEXCLAMATION | MB_OK);
			return text;
			
		}

		EnableWindow(WindowFromDC(wglGetCurrentDC()), FALSE);
		HWND hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT\0", convertNarrowToWide(text).c_str(),
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			10, 10, 210, 40, dialog, (HMENU)101, GetModuleHandle(nullptr), nullptr);


		HWND okButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON\0", L"OK\0",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			10, 60, 60, 30, dialog, (HMENU)IDOK, GetModuleHandle(nullptr), nullptr);

		HWND cancelButton = CreateWindowEx(WS_EX_CLIENTEDGE, L"BUTTON\0", L"Cancel\0",
			WS_CHILD | WS_VISIBLE,
			80, 60, 60, 30, dialog, (HMENU)IDCANCEL, GetModuleHandle(nullptr), nullptr);

		SetFocus( hEdit );

		ShowWindow(dialog, SW_SHOWNORMAL);
		bool bFirstEmpty = true;
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
     ofxAndroidAlertTextBox(question,text);
#endif

#ifdef TARGET_EMSCRIPTEN
     text = emscripten_run_script_string((string("prompt('") + question + "','')").c_str());
#endif
	return text;
}
