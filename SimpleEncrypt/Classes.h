#pragma once

class TextInfoBar {
public:

	TextInfoBar(int x, int y, HWND parent_wnd) {
		this->m_parent_wnd = parent_wnd;

		this->m_bar_wnd = CreateWindow(L"static", NULL, WS_VISIBLE | WS_CHILD, x, y, MAIN_EDIT_SIZE, 20, parent_wnd, NULL, NULL, NULL);

		CreateWindow(L"static", L"symbols:", WS_VISIBLE | WS_CHILD, 0, 2, 65, 16, m_bar_wnd, NULL, NULL, NULL);
		this->m_symbols_count_wnd = CreateWindow(L"static", L"0", WS_VISIBLE | WS_CHILD, 65, 2, 40, 16, m_bar_wnd, NULL, NULL, NULL);

		CreateWindow(L"static", L"rows:", WS_VISIBLE | WS_CHILD, 120, 2, 40, 16, m_bar_wnd, NULL, NULL, NULL);
		this->m_row_count_wnd = CreateWindow(L"static", L"1", WS_VISIBLE | WS_CHILD, 160, 2, 40, 16, m_bar_wnd, NULL, NULL, NULL);

		CreateWindow(L"static", L"cursor:", WS_VISIBLE | WS_CHILD, 212, 2, 50, 16, m_bar_wnd, NULL, NULL, NULL);
		this->m_cursor_position_wnd = CreateWindow(L"static", L"0:0", WS_VISIBLE | WS_CHILD, 262, 2, 65, 16, m_bar_wnd, NULL, NULL, NULL);

		CreateWindow(L"static", L"selected:", WS_VISIBLE | WS_CHILD, 330, 2, 64, 16, m_bar_wnd, NULL, NULL, NULL);
		this->m_selected_symbols_wnd = CreateWindow(L"static", L"0", WS_VISIBLE | WS_CHILD, 395, 2, 40, 16, m_bar_wnd, NULL, NULL, NULL);

	}

	//	===== Text methods =====

	//	return true when rowCount changed
	bool update(HWND source_wnd) {
		bool status = true;
		std::string str;
		int prev_row_count = m_row_count;
		char* temp;

		this->m_symbols_count = GetWindowTextLengthA(source_wnd) + 1;
		temp = new char[this->m_symbols_count];
		GetWindowTextA(source_wnd, temp, this->m_symbols_count);

		this->m_row_count = 1;
		for (int i = 0; i < this->m_symbols_count; i++) {
			if (temp[i] == '\n') {
				m_row_count++;
			}
		}

		if (prev_row_count == m_row_count)
			status = false;

		m_line_count = SendMessage(source_wnd, EM_GETLINECOUNT, NULL, NULL);

		delete[] temp; temp = NULL;

		return status;
	}

	//	Show values on info bar
	void show() {
		SetWindowTextA(m_symbols_count_wnd, std::to_string(m_symbols_count - 1).c_str());
		SetWindowTextA(m_row_count_wnd, std::to_string(m_row_count).c_str());
		SetWindowTextA(m_cursor_position_wnd, (std::to_string(m_curs_pos.X) + ":" + std::to_string(m_curs_pos.Y)).c_str());
		SetWindowTextA(m_selected_symbols_wnd, std::to_string(m_selected_symbols).c_str());
	}

	int getSymbolsCount() {
		return m_symbols_count;
	}
	
	int getRowCount() {
		return m_row_count;
	}

	int getLineCount() {
		return m_line_count;
	}

	//	===== Cursor methods =====

	void updateCursorPosition(HWND source_wnd) {
		LRESULT result = SendMessage(source_wnd, EM_GETSEL, NULL, NULL);
		m_curs_range.Y = LOWORD(result);
		m_curs_range.X = HIWORD(result);

		m_curs_pos.Y = SendMessage(source_wnd, EM_LINEFROMCHAR, -1, NULL);
		m_curs_pos.X = m_curs_range.X - SendMessage(source_wnd, EM_LINEINDEX, -1, NULL);
		
		m_selected_symbols = m_curs_range.X - m_curs_range.Y;
		if (m_selected_symbols < 0)
			m_selected_symbols *= (-1);
	}

	int getCursorRangeX() {
		return m_curs_range.X;
	}

	int getCursorRangeY() {
		return m_curs_range.Y;
	}

	int getCursorPosX() {
		return m_curs_pos.X;
	}

	int getCursorPosY() {
		return m_curs_pos.Y;
	}

private:
	int m_symbols_count = 1;
	int m_row_count = 1;
	int m_line_count = 0;
	int m_selected_symbols = 0;

	COORD m_curs_pos = { 0 };
	COORD m_curs_range = { 0 };

	HWND m_parent_wnd;
	HWND m_bar_wnd;
	HWND m_symbols_count_wnd;
	HWND m_row_count_wnd;
	HWND m_cursor_position_wnd;
	HWND m_selected_symbols_wnd;
};


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////


class ScrollBar {
public:
	ScrollBar(int x, int y, int width, int height, HWND parent_wnd, int wnd_id) {
		this->m_scroll_id = wnd_id;
		this->m_handle_wnd = CreateWindow(L"scrollbar", NULL, WS_CHILD | WS_VISIBLE | SBS_VERT, x, y, width, height, parent_wnd, (HMENU)wnd_id, NULL, NULL);

		m_scroll_info.cbSize = sizeof(SCROLLINFO);
		m_scroll_info.fMask = SIF_ALL;
		m_scroll_info.nMin = 0;
		m_scroll_info.nMax = 0;
		m_scroll_info.nPage = SCROLL_PAGE_HEIGHT;
		m_scroll_info.nPos = 0;
		m_scroll_info.nTrackPos = 0;

		SetScrollInfo(m_handle_wnd, SB_CTL, &m_scroll_info, TRUE);
	}

	void update(int lines_count, bool redraw = false, int cursorPositinY = 0) {
		GetScrollInfo(m_handle_wnd, SB_CTL, &m_scroll_info);

		if (lines_count > MAX_ROWS_ON_PAGE) {
			m_scroll_info.nMax = lines_count - MAX_ROWS_ON_PAGE;
		}
		else {
			m_scroll_info.nMax = 0;
		}

		if (redraw) {
			if (lines_count == cursorPositinY)
				m_scroll_info.nTrackPos = m_scroll_info.nPos = cursorPositinY;
		}
		else {
			m_scroll_info.nPos = m_scroll_info.nTrackPos = m_scroll_info.nMin;
		}

		SetScrollInfo(m_handle_wnd, SB_CTL, &m_scroll_info, TRUE);
	}

	void scroll(WPARAM& wp, HWND scrolled_window) {
		m_scroll_info.nTrackPos = m_scroll_info.nPos;

		switch (LOWORD(wp)) {
		case SB_LINEUP:
			m_scroll_info.nPos -= 5;
			break;

		case SB_LINEDOWN:
			m_scroll_info.nPos += 5;
			break;

		case SB_THUMBPOSITION:
		case SB_THUMBTRACK:
			m_scroll_info.nPos = HIWORD(wp);
			break;
		}

		if (m_scroll_info.nPos > m_scroll_info.nMax)
			m_scroll_info.nPos = m_scroll_info.nMax;

		if (m_scroll_info.nPos < m_scroll_info.nMin)
			m_scroll_info.nPos = m_scroll_info.nMin;

		SetScrollPos(m_handle_wnd, SB_CTL, m_scroll_info.nPos, TRUE);	
		SendMessage(scrolled_window, EM_LINESCROLL, NULL, m_scroll_info.nPos - m_scroll_info.nTrackPos);
	}

	void setPos(int position, bool scroll = false, HWND scrolled_window = NULL) {
		m_scroll_info.nTrackPos = m_scroll_info.nPos;

		m_scroll_info.nPos = position;

		if (m_scroll_info.nPos > m_scroll_info.nMax)
			m_scroll_info.nTrackPos = m_scroll_info.nPos = m_scroll_info.nMax;

		if (m_scroll_info.nPos < m_scroll_info.nMin)
			m_scroll_info.nTrackPos = m_scroll_info.nPos = m_scroll_info.nMin;

		SetScrollPos(m_handle_wnd, SB_CTL, m_scroll_info.nPos, TRUE);
		if (scroll) {
			SendMessage(scrolled_window, EM_LINESCROLL, NULL, m_scroll_info.nPos - m_scroll_info.nTrackPos);
		}
	}

	int getPos() {
		return m_scroll_info.nPos;
	}

	HWND getHandle() {
		return m_handle_wnd;
	}


private:

	HWND m_handle_wnd;
	int m_scroll_id;

	SCROLLINFO m_scroll_info = { 0 };
};