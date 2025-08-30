#pragma once


#include"BasicWidget.h"

#include<string>
#include <graphics.h> 







class LineEdit:public BasicWidget

{





public:
	LineEdit(int x = 0, int y = 0, int w = 150, int h = 30);


	bool isClicked() const;
	void show()override;
	void eventloop(const ExMessage& msg);
	inline void setTitle(const std::wstring& title) {
		m_title = title;
	}
	const std::wstring& text() const { return m_text; }
	inline void setText(const std::wstring& text) {
		m_text = text;
	}

	// 清空输入框
	inline void clear() {
		m_text.clear();
	}


	




private:
	std::wstring m_text;
	ExMessage m_msg;
	std::wstring m_title;

	bool  just_clicked_ = false;  // 本帧是否刚点击

	bool focused_ = false;














};

