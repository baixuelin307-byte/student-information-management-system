#include "LineEdit.h"
#include <graphics.h>
#include <string>

LineEdit::LineEdit(int x, int y, int w, int h)
    : BasicWidget(x, y, w, h), m_text(L""), m_title(L"input content please ") {
}

// �����ⲿ��ѯ���Ƿ��������� eventloop ����
bool LineEdit::isClicked() const {
    return (m_msg.message == WM_LBUTTONUP &&
        m_msg.x >= m_x && m_msg.x <= m_x + m_w &&
        m_msg.y >= m_y && m_msg.y <= m_y + m_h);
}

// ���ؼ� + ���С��յ������һ�������
void LineEdit::show() {
    // ���� + �߿�
    setfillcolor(WHITE);
    setlinecolor(BLACK);
    fillrectangle(m_x, m_y, m_x + m_w, m_y + m_h);

    // �ı�
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(20, 0, L"΢���ź�");
    outtextxy(m_x + 5, m_y + (m_h - textheight(L"A")) / 2, m_text.c_str());

    // ֻ�ڱ�֡���յ����ʱ��һ�Σ�eventloop ����λ��
    if (just_clicked_) {
        just_clicked_ = false;              // �������������

        wchar_t buf[128] = L"";
        EndBatchDraw();                     // ���ⱻ����������ס
        bool ok = InputBox(buf, 128,
            m_title.empty() ? L"in put please" : m_title.c_str(),
            L"in put");
        BeginBatchDraw();

        if (ok) {
            m_text = buf;                   // ��д
        }
    }
}

// �� ���뱣�� msg����������ʱ�� just_clicked_ ��λ
void LineEdit::eventloop(const ExMessage& msg) {
    m_msg = msg;

    // �ð��¸����״�����Ҳ�ɻ��� WM_LBUTTONUP
    if (msg.message == WM_LBUTTONDOWN &&
        msg.x >= m_x && msg.x <= m_x + m_w &&
        msg.y >= m_y && msg.y <= m_y + m_h) {
        just_clicked_ = true;
    }

    // ����ѡ���۽�����ռ�������
    if (msg.message == WM_CHAR) {
        if (msg.ch == VK_BACK) {
            if (!m_text.empty()) m_text.pop_back();
        }
        else if (msg.ch >= 32) {
            m_text.push_back(static_cast<wchar_t>(msg.ch));
        }
    }
}