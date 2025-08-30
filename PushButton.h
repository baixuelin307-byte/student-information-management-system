#pragma once
#include <string>
#include "BasicWidget.h"
#include <graphics.h> 
// EasyX 事件相关
#include <conio.h>
class PushButton : public BasicWidget {
public:
    PushButton(const std::wstring& text, int x, int y, int w, int h);
    void show() override;
    bool isin()const;
    bool isClicked();
   
    void eventloop(const ExMessage& msg);
    void setBackground(COLORREF c);
    void setHoverColor(COLORREF c);
private:
    std::wstring m_text;
    ExMessage m_msg;
private:
    COLORREF cur_c = RGB(232, 232, 236);
    COLORREF normal_c = RGB(232, 232, 236);
  
    COLORREF hover_c = BLUE;
};