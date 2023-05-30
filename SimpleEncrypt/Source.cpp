#include "Source.h"

WNDCLASS NewWindClass(HBRUSH BgCol, HCURSOR Curs, HINSTANCE hInst, HICON Ico, LPCWSTR Name, WNDPROC Procedure) {
	WNDCLASS NWC = { 0 };

	NWC.hIcon = Ico;
	NWC.hCursor = Curs;
	NWC.hInstance = hInst;
	NWC.lpszClassName = Name;
	NWC.hbrBackground = BgCol;
	NWC.lpfnWndProc = Procedure;

	return NWC;
}

bool registerClass(HINSTANCE hInst) {

	WNDCLASS MainClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"MainWndClass", main_proc);
	if (!RegisterClassW(&MainClass)) {
		return 0;
	}

	WNDCLASS askSaveClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), global_hInst, LoadIcon(global_hInst, MAKEINTRESOURCE(IDI_ICON1)), L"askKeyForSave", save_dialog);
	if (!RegisterClassW(&askSaveClass)) {
		return 0;
	}

	WNDCLASS askLoadClass = NewWindClass((HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), hInst, LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON1)), L"askKeyForLoad", load_dialog);
	if (!RegisterClassW(&askLoadClass)) {
		return 0;
	}

	return 1;
}

#ifdef DEBUG

	#define __CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
	#define new DEBUG_NEW

#endif // DEBUG

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR args, int ncmdshow) {

	setlocale(LC_ALL, "ru");

#ifdef DEBUG
	//MessageBoxA(NULL, args, "LPSTR args", MB_OK);
#endif // DEBUG

	if (args[0] == 0) {
		startFromFile = false;
	}
	else {
		std::string argPath(args);

		for (std::string::const_iterator i = argPath.begin(), end = argPath.end(); i != end; i++)
			if (*i == '\"')
				argPath.erase(i);

		memcpy(path, argPath.c_str(), MAX_PATH);
		startFromFile = true;
	}

	registerClass(hInst);

	MSG MainMessage = { 0 };

	global_hWnd = CreateWindow(L"MainWndClass", L"TxtEncryptor", WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_THICKFRAME) | WS_VISIBLE, CenterX(WINDOW_WIDTH + 16), CenterY(WINDOW_HEIGHT + 39), WINDOW_WIDTH + 16, WINDOW_HEIGHT + 39, NULL, NULL, hInst, NULL);
	global_hInst = hInst;

	while (GetMessage(&MainMessage, NULL, NULL, NULL)) {
		TranslateMessage(&MainMessage);
		DispatchMessage(&MainMessage);
	}
#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif // DEBUG
	return 0;
}

LRESULT CALLBACK main_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_CREATE: 
	{
		main_edit_wnd = CreateWindow(L"edit", NULL, 
									WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | WS_CLIPCHILDREN | WS_CLIPSIBLINGS,
									10, 10, 
									MAIN_EDIT_SIZE, MAIN_EDIT_SIZE, 
									hWnd, 
									(HMENU)MAIN_EDIT_ID, 
									NULL, NULL);

		main_scroll = new ScrollBar(MAIN_EDIT_SIZE + 10, 10, 
									17, MAIN_EDIT_SIZE,
									hWnd, 
									MAIN_SCROLLBAR_ID);

		txt_info_bar = new TextInfoBar(10, MAIN_EDIT_SIZE + 10, hWnd);

		DragAcceptFiles(main_edit_wnd, TRUE);
		SetOpenFileNameParams(hWnd);

		original_edit_proc = (WNDPROC)SetWindowLongPtr(main_edit_wnd, GWLP_WNDPROC, (LONG_PTR)custom_main_edit_proc);

		if (startFromFile)
			loadDialog(hWnd);
	}
		break;

	case WM_VSCROLL:
	{
		if ((HWND)lp == main_scroll->getHandle()) {
			main_scroll->scroll(wp, main_edit_wnd);
		}
	}
		break;

	case WM_COMMAND:
		switch (LOWORD(wp)) {

		}
		switch (HIWORD(wp)) {
		case EN_SETFOCUS:
			break;
		}
		break;

	case WM_DESTROY:
		delete main_scroll;
		delete txt_info_bar;
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

LRESULT CALLBACK save_dialog(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	
	case WM_CREATE:
		//EnableWindow(ghWnd, false);
		CreateWindow(L"static", L"Enter key for encrypt:", WS_CHILD | WS_VISIBLE, 10, 10, 280, 20, hWnd, NULL, NULL, NULL);
		
		password_edit_prev_wnd = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 10, 30, 280, 20, hWnd, (HMENU)PASSWORD_PREV_EDIT_ID, NULL, NULL);
		password_edit_wnd = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 10, 60, 280, 20, hWnd, (HMENU)PASSWORD_EDIT_ID, NULL, NULL);
		
		CreateWindow(L"button", L"cancel", WS_CHILD | WS_VISIBLE, 150, 100, 60, 20, hWnd, (HMENU)CANCEL_BTN_ID, NULL, NULL);
		action_btn_wnd = CreateWindow(L"button", L"save", WS_CHILD | WS_VISIBLE , 230, 100, 60, 20, hWnd, (HMENU)SAVE_BTN_ID, NULL, NULL);
		
		CreateWindow(L"static", L"Max symbols in password = 32 bytes", WS_CHILD | WS_VISIBLE, 10, 130, 280, 20, hWnd, NULL, NULL, NULL);
		

		original_edit_proc = (WNDPROC)SetWindowLongPtr(password_edit_prev_wnd, GWLP_WNDPROC, (LONG_PTR)custom_password_edit_proc);
		original_edit_proc = (WNDPROC)SetWindowLongPtr(password_edit_wnd, GWLP_WNDPROC, (LONG_PTR)custom_password_edit_proc);

		SetFocus(password_edit_prev_wnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wp)) {

		case SAVE_BTN_ID:
			if (GetSaveFileNameA(&userDirectory)) {
				if (std::string(path).find(".rij") == std::string::npos)
					for (short i = 0; true; i++) {
						if (path[i] != '\0')
							continue;
						path[i++] = '.';
						path[i++] = 'r';
						path[i++] = 'i';
						path[i++] = 'j';
						path[i] = '\0';
						break;
					}
				if (!save(path))
					MessageBox(hWnd, L"Passwords do not match", L"Error!", MB_OK | MB_ICONWARNING);
				else 
					DestroyWindow(hWnd);
			}
			break;

		case CANCEL_BTN_ID:
			DestroyWindow(hWnd);
			break;
		}

		switch (HIWORD(wp)) {
		case EN_SETFOCUS:
			current_password_edit_focus_id = LOWORD(wp);
			break;
		}

		break;

	case WM_DESTROY:
		SetFocus(global_hWnd);
		//EnableWindow(ghWnd, true);
		return 0;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

LRESULT CALLBACK load_dialog(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {
	
	case WM_CREATE:
		//EnableWindow(ghWnd, false);
		CreateWindow(L"static", L"Enter key for decrypt:", WS_CHILD | WS_VISIBLE, 10, 10, 280, 20, hWnd, NULL, NULL, NULL);

		password_edit_wnd = CreateWindow(L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 10, 30, 280, 20, hWnd, (HMENU)PASSWORD_EDIT_ID, NULL, NULL);

		CreateWindow(L"button", L"cancel", WS_CHILD | WS_VISIBLE, 150, 70, 60, 20, hWnd, (HMENU)CANCEL_BTN_ID, NULL, NULL);
		action_btn_wnd = CreateWindow(L"button", L"load", WS_CHILD | WS_VISIBLE , 230, 70, 60, 20, hWnd, (HMENU)LOAD_BTN_ID, NULL, NULL);

		CreateWindow(L"static", L"Max symbols in password = 32 bytes", WS_CHILD | WS_VISIBLE, 10, 100, 280, 20, hWnd, NULL, NULL, NULL);

		original_edit_proc = (WNDPROC)SetWindowLongPtr(password_edit_wnd, GWLP_WNDPROC, (LONG_PTR)custom_password_edit_proc);

		SetFocus(password_edit_wnd);
		break;

	case WM_COMMAND:
		switch (LOWORD(wp)) {

		case LOAD_BTN_ID:
			try {
				if (!load(path)) {
					MessageBoxA(hWnd, "File not found", "Error:", MB_OK | MB_ICONWARNING);
				}
				txt_info_bar->update(main_edit_wnd);
				txt_info_bar->updateCursorPosition(main_edit_wnd);
				main_scroll->update(txt_info_bar->getLineCount());
				txt_info_bar->show();
			}
			catch (CryptoPP::Exception e) {
				MessageBoxA(hWnd, e.what(), "Exception:", MB_OK);
			}
			DestroyWindow(hWnd);
			break;

		case CANCEL_BTN_ID:
			DestroyWindow(hWnd);
			break;
		
		}

		switch (HIWORD(wp)) {
		case EN_SETFOCUS:
			current_password_edit_focus_id = LOWORD(wp);
			break;
		}
		break;

	case WM_DESTROY:
		SetFocus(global_hWnd);
		//EnableWindow(ghWnd, true);
		break;

	default:
		return DefWindowProc(hWnd, msg, wp, lp);
	}
}

LRESULT CALLBACK custom_main_edit_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_LBUTTONUP:
	case WM_LBUTTONDOWN: {
		LRESULT result = CallWindowProc(original_edit_proc, hWnd, msg, wp, lp);
		txt_info_bar->updateCursorPosition(hWnd);
		txt_info_bar->show();
		return result; }

	case WM_KEYDOWN:
		txt_info_bar->updateCursorPosition(hWnd);
		switch (LOWORD(wp)) {

		case 16:
			hot_key.shift = true;
			break;

		case 17:
			hot_key.ctrl = true;
			break;

		case 37:
			if (hot_key.shift == true) {
				SendMessage(hWnd, EM_SETSEL, 
					txt_info_bar->getCursorRangeX(), 
					txt_info_bar->getCursorRangeY() - 1);
			}
			else {
				SendMessage(hWnd, EM_SETSEL, 
					txt_info_bar->getCursorRangeY() - 1, 
					txt_info_bar->getCursorRangeY() - 1);
			}
			break;

		/*case 38:
			break;*/

		case 39:
			if (hot_key.shift == true) {
				SendMessage(hWnd, EM_SETSEL,
					txt_info_bar->getCursorRangeY(),
					txt_info_bar->getCursorRangeX() + 1);
			}
			else {
				SendMessage(hWnd, EM_SETSEL,
					txt_info_bar->getCursorRangeX() + 1,
					txt_info_bar->getCursorRangeX() + 1);
			}
			break;

		/*case 40:
			break;*/

		case 46:		//	del	//
			//	TODO
			break;
		
		case 83:
			if (hot_key.ctrl == true) {
				saveDialog(hWnd);
				hot_key.ctrl = false;
			}
			break;
		
		case 65:		//	ctrl + a	//	Select all
			if (hot_key.ctrl == true) {
				SendMessage(hWnd, EM_SETSEL, 0, -1);
				hot_key.ctrl = false;
			}
			break;

#ifdef DEBUG
		default:
			int test = LOWORD(wp);
			//MessageBox(hWnd, std::to_wstring(test).c_str(), L"Key:", MB_OK);
			break;
#endif // DEBUG

		}
		break;

	case WM_KEYUP:
		txt_info_bar->updateCursorPosition(hWnd);
		if (txt_info_bar->update(hWnd)) {
			main_scroll->update(txt_info_bar->getLineCount(), true, txt_info_bar->getCursorPosY() + 1);
		}
		txt_info_bar->show();
		switch (LOWORD(wp)) {
		case 16:
			hot_key.shift = false;
			break;

		case 17:
			hot_key.ctrl = false;
			break;
		}
		break;

	case WM_DROPFILES:
		if (getDropedFilePath((HDROP)wp)) {
			loadDialog(hWnd);
		}
		else {
			MessageBox(hWnd, L"You can only open \"*.rij\" files", L"Open file error", MB_OK);
		}
		break;

	case WM_MOUSEWHEEL:
		if (GET_WHEEL_DELTA_WPARAM(wp) > 0) {
			main_scroll->setPos(main_scroll->getPos() - 1, true, main_edit_wnd);
		}
		if (GET_WHEEL_DELTA_WPARAM(wp) < 0) {
			main_scroll->setPos(main_scroll->getPos() + 1, true, main_edit_wnd);
		}
		break;

	default:
		return CallWindowProc(original_edit_proc, hWnd, msg, wp, lp);
		break;
	}
}

LRESULT CALLBACK custom_password_edit_proc(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp) {
	switch (msg) {

	case WM_KEYDOWN:
		switch (LOWORD(wp)) {

		case 8:
			if (hot_key.ctrl == true) {
				switch (current_password_edit_focus_id) {
				case PASSWORD_PREV_EDIT_ID:
					SetWindowText(password_edit_prev_wnd, L"");
					break;

				case PASSWORD_EDIT_ID:
					SetWindowText(password_edit_wnd, L"");
					break;
				}
			}
			break;
		
		case 13:
			switch (current_password_edit_focus_id) {
			case PASSWORD_PREV_EDIT_ID:
				SetFocus(password_edit_wnd);
				break;
			
			case PASSWORD_EDIT_ID:
				SendMessage(action_btn_wnd, BM_CLICK, 0, 0);
				break;
			}
			break;

		case 17:
			hot_key.ctrl = true;
			break;
		}
		break;

	case WM_KEYUP:
		switch (LOWORD(wp)) {

		case 17:
			hot_key.ctrl = false;
			break;
		}
		break;

	default:
		return CallWindowProc(original_edit_proc, hWnd, msg, wp, lp);
	}
}

void saveDialog(HWND hWnd) {
	save_dialog_wnd = CreateWindow(L"askKeyForSave", L"Enter Key:", WS_VISIBLE | WS_SYSMENU | WS_CAPTION, CenterX(300 + 16), CenterY(200 + 39), 300 + 16, 200 + 39, hWnd, NULL, global_hInst, save_dialog);
}

void loadDialog(HWND hWnd) {
	load_dialog_wnd = CreateWindow(L"askKeyForLoad", L"Enter Key:", WS_VISIBLE, CenterX(300 + 16), CenterY(200 + 39), 300 + 16, 200 + 39, hWnd, NULL, global_hInst, load_dialog);
}

bool save(char path[]) {
	int plain_text_lenght;
	char* buffer{};

	plain_text_lenght = GetWindowTextLength(main_edit_wnd) + 1;

	buffer = new char[plain_text_lenght];

	GetWindowTextA(main_edit_wnd, buffer, plain_text_lenght);

	std::string plain_text(buffer);
	std::string cipher_text;

	memset(buffer, 0x00, plain_text_lenght);
	delete[] buffer;
	buffer = NULL;

	CryptoPP::byte key[CryptoPP::AES::MAX_KEYLENGTH];
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];

	buffer = new char[CryptoPP::AES::MAX_KEYLENGTH];

	memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	memset(buffer, 0x00, CryptoPP::AES::MAX_KEYLENGTH);

	GetWindowTextA(password_edit_prev_wnd, buffer, CryptoPP::AES::MAX_KEYLENGTH);
	SetWindowText(password_edit_prev_wnd, L"");

	memcpy(key, buffer, CryptoPP::AES::MAX_KEYLENGTH);

	GetWindowTextA(password_edit_wnd, buffer, CryptoPP::AES::MAX_KEYLENGTH);
	SetWindowText(password_edit_wnd, L"");

	for (short i = 0; i < CryptoPP::AES::MAX_KEYLENGTH; i++) {
		if (key[i] != buffer[i]) {
			return false;
		}
	}

	delete[] buffer;
	buffer = NULL;

	CryptoPP::AES::Encryption aesEncrption(key, CryptoPP::AES::MAX_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption(aesEncrption, iv);
	CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(cipher_text));

	stfEncryptor.Put(reinterpret_cast<const unsigned char*>(plain_text.c_str()), plain_text.length());
	stfEncryptor.MessageEnd();

	plain_text.clear();

	std::fstream file;

	file.open(path, std::ios_base::binary | std::ios_base::out);
	file << cipher_text;
	file.close();

	cipher_text.clear();

	return true;
}

bool load(char path[]) {
	char* buffer{};
	std::string encrypted_text;
	std::string decrypted_text;

#ifdef DEBUG
	MessageBoxA(NULL, path, "wchar_t path[256]", MB_OK);
#endif // DEBUG

	if (!readFileTo(encrypted_text))
		return false;

	CryptoPP::byte key[CryptoPP::AES::MAX_KEYLENGTH];
	CryptoPP::byte iv[CryptoPP::AES::BLOCKSIZE];

	buffer = new char[CryptoPP::AES::MAX_KEYLENGTH];

	memset(key, 0x00, CryptoPP::AES::MAX_KEYLENGTH);
	memset(iv, 0x00, CryptoPP::AES::BLOCKSIZE);
	memset(buffer, 0x00, CryptoPP::AES::MAX_KEYLENGTH);

	GetWindowTextA(password_edit_wnd, buffer, CryptoPP::AES::MAX_KEYLENGTH);
	SetWindowText(password_edit_wnd, L"");

#ifdef DEBUG
	MessageBoxA(NULL, buffer, "password", MB_OK);
#endif // DEBUG 

	memcpy(key, buffer, CryptoPP::AES::MAX_KEYLENGTH);
	memset(buffer, 0x00, CryptoPP::AES::MAX_KEYLENGTH);

	delete[] buffer;
	buffer = NULL;

	CryptoPP::AES::Decryption aesDecrption(key, CryptoPP::AES::MAX_KEYLENGTH);
	CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption(aesDecrption, iv);
	CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink(decrypted_text));

	stfDecryptor.Put(reinterpret_cast<const unsigned char*>(encrypted_text.c_str()), encrypted_text.size());
	stfDecryptor.MessageEnd();

	SetWindowTextA(main_edit_wnd, decrypted_text.c_str());

#ifdef DEBUG
	SetWindowText(main_edit_wnd, L"Complete!");
#endif // DEBUG

	return true;
}

bool readFileTo(std::string& str) {
	std::fstream file;

	file.open(path, std::ios_base::binary | std::ios_base::in);

	if (!file.is_open())
		return false;

	str.clear();
	while (!file.eof())
		str.push_back(file.get());

	str.erase(--str.end());

	file.close();
	return true;
}

bool getDropedFilePath(HDROP hDrop) {
	DragQueryFileA(hDrop, 0, path, MAX_PATH);
	if (std::string(path).find(".rij") == std::string::npos)
		return false;
	return true;
}

void SetOpenFileNameParams(HWND hWnd) {
	ZeroMemory(&userDirectory, sizeof(userDirectory));
	userDirectory.lStructSize = sizeof(userDirectory);
	userDirectory.hwndOwner = hWnd;
	userDirectory.lpstrFile = path;
	userDirectory.nMaxFile = sizeof(path);
	userDirectory.lpstrFilter = "*.rij";
	userDirectory.lpstrFileTitle = NULL;
	userDirectory.lpstrInitialDir = NULL;
	userDirectory.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
}