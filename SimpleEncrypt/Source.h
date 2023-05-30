//	+-------------------------------------------------------------------------------+
//	|	Version: 1.1																|
//	|	Developer: Syperlol ake DronKon ake DronSyperlol ake andre_syperlol			|
//	|	Contacts: t.me/Syperlol github.com/DronSyperlol								|
//	|	App name: TxtEncryptor														|
//	|	Old name: SimpleEncrypt														|
//	|	License: MPL v2.0															|
//	+-------------------------------------------------------------------------------+

#pragma once

#include <windows.h>
#include <string>
#include <fstream>

#include <cryptopp/modes.h>
#include <cryptopp/aes.h>
#include <cryptopp/filters.h>

#include "resource.h"

//	================  //
//					  //
//	#define DEBUG	  //
//					  //
//	================  //

#ifdef DEBUG
#pragma comment(lib, "Cryptlib/Debug/cryptlib.lib")
#else
#pragma comment(lib, "Cryptlib/Release/cryptlib.lib")
#endif

#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//	===== Consts =====

const short WINDOW_WIDTH = 515;
const short WINDOW_HEIGHT = 520;

const short MAIN_EDIT_SIZE = 480;

const short SCROLL_PAGE_HEIGHT = 16;
const short MAX_ROWS_ON_PAGE = MAIN_EDIT_SIZE / SCROLL_PAGE_HEIGHT;

const short MAIN_EDIT_ID = 1030;
const short MAIN_SCROLLBAR_ID = 1031;

const short PASSWORD_PREV_EDIT_ID = 1035;		//	ids 1035 - 1038 reserved for previous passwords edits
const short PASSWORD_EDIT_ID = 1039;			//	It's last password edit

const short SAVE_BTN_ID = 1040;
const short LOAD_BTN_ID = 1041;
const short CANCEL_BTN_ID = 1042;


//	===== Headers functions =====
//	Procedures:

LRESULT CALLBACK main_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK save_dialog(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK load_dialog(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK custom_main_edit_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
LRESULT CALLBACK custom_password_edit_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


//	Dialogs:

void saveDialog(HWND hWnd);
void loadDialog(HWND hWnd);


//	Encrypt and decpypt functions:

bool save(char path[]);
bool load(char path[]);
bool readFileTo(std::string& str);

//	Grag and drop functions:

bool getDropedFilePath(HDROP hDrop);
void SetOpenFileNameParams(HWND hWnd);


//	===== Program vars =====

HINSTANCE global_hInst;

HWND global_hWnd;

HWND password_edit_prev_wnd;	//	prev password edit (it firts password edit in save dialog)
HWND password_edit_wnd;			//	last password edit

HWND action_btn_wnd;

HWND save_dialog_wnd;
HWND load_dialog_wnd;

HWND main_edit_wnd;
WNDPROC original_edit_proc;

OPENFILENAMEA userDirectory;

bool startFromFile;

short current_password_edit_focus_id = 0;

char path[MAX_PATH] = "new file.rij";


#include "Structs.h"
HotKeys hot_key;

#include "Classes.h"
TextInfoBar* txt_info_bar;
ScrollBar* main_scroll;


int CenterX(int arg) {
	int monitorSizeX = GetSystemMetrics(SM_CXSCREEN); // высота экрана
	return ((monitorSizeX / 2) - (arg / 2));
}

int CenterY(int arg) {
	int monitorSizeY = GetSystemMetrics(SM_CYSCREEN); // ширина экрана
	return ((monitorSizeY / 2) - (arg / 2));
}
