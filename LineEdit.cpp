#include "LineEdit.h"
#include <graphics.h>
#include <string>

LineEdit::LineEdit(int x, int y, int w, int h)
    : BasicWidget(x, y, w, h), m_text(L""), m_title(L"input content please ") {
}

// 仅供外部查询；是否点击成立由 eventloop 决定
bool LineEdit::isClicked() const {
    return (m_msg.message == WM_LBUTTONUP &&
        m_msg.x >= m_x && m_msg.x <= m_x + m_w &&
        m_msg.y >= m_y && m_msg.y <= m_y + m_h);
}

// 画控件 + 如有“刚点击”则弹一次输入框
void LineEdit::show() {
    // 背景 + 边框
    setfillcolor(WHITE);
    setlinecolor(BLACK);
    fillrectangle(m_x, m_y, m_x + m_w, m_y + m_h);

    // 文本
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, L"微软雅黑");
    outtextxy(m_x + 5, m_y + (m_h - textheight(L"A")) / 2, m_text.c_str());

    // 只在本帧“刚点击”时弹一次（eventloop 里置位）
    if (just_clicked_) {
        just_clicked_ = false;              // 先清零避免连弹

        wchar_t buf[128] = L"";
        EndBatchDraw();                     // 避免被批量绘制遮住
        bool ok = InputBox(buf, 128,
            m_title.empty() ? L"in put please" : m_title.c_str(),
            L"in put");
        BeginBatchDraw();

        if (ok) {
            m_text = buf;                   // 回写
        }
    }
}

// ★ 必须保存 msg；并在命中时把 just_clicked_ 置位
void LineEdit::eventloop(const ExMessage& msg) {
    m_msg = msg;

    // 用按下更容易触发；也可换成 WM_LBUTTONUP
    if (msg.message == WM_LBUTTONDOWN &&
        msg.x >= m_x && msg.x <= m_x + m_w &&
        msg.y >= m_y && msg.y <= m_y + m_h) {
        just_clicked_ = true;
    }

    // （可选）聚焦后接收键盘输入
    if (msg.message == WM_CHAR) {
        if (msg.ch == VK_BACK) {
            if (!m_text.empty()) m_text.pop_back();
        }
        else if (msg.ch >= 32) {
            m_text.push_back(static_cast<wchar_t>(msg.ch));
        }
    }
}