#include "PushButton.h"
#include <graphics.h>

PushButton::PushButton(const std::wstring& text, int x, int y, int w, int h)
	: BasicWidget(x, y, w, h), m_text(text) {
	m_msg = ExMessage{};
}

void PushButton::show()
{
	setfillcolor( cur_c);
	fillroundrect(m_x, m_y, m_x + m_w, m_y + m_h, 10, 10);

	settextcolor(RED);
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, L"微软雅黑");

	// 正确使用类成员变量 m_text
	int tx = m_x + (m_w - textwidth(m_text.c_str())) / 2;
	int ty = m_y + (m_h - textheight(m_text.c_str())) / 2;

	outtextxy(tx, ty, m_text.c_str());
}
bool PushButton::isin()const {
	return (m_msg.x >= m_x && m_msg.x < m_x + m_w &&
		m_msg.y >= m_y && m_msg.y <= m_y + m_h);
}

bool PushButton::isClicked() {
	
	return (m_msg.message == WM_LBUTTONDOWN && isin());

}

void PushButton::eventloop(const ExMessage& msg)
{
	m_msg = msg;
	if (!isin()) {
		cur_c = normal_c;

	}
	else {
		cur_c = hover_c;
	}
        
}

void PushButton::setBackground(COLORREF c)
{
	normal_c = c;
}

void PushButton::setHoverColor(COLORREF c)
{
	hover_c = c;
}


