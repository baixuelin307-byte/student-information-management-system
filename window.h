#pragma once
#include"configure.h"
#include <graphics.h>
#include<vector>
#include"PushButton.h"

class window {
public:
	window(int w, int h, int flag);
	void setWindowTilte(const std::string& tilte);
		int exec();
		static int width();
		static int height();
		static void clear();
		static void beginDraw();
		static void flushDraw();
		static void endDraw();


		inline static bool hasMsg() { return ::peekmessage(&m_msg, EM_MOUSE, EM_KEY); }
		inline static  EASYXMSG& getMsg() { return m_msg; }//判断消息的




private:
	HWND m_handle;//窗口句柄
	static ExMessage m_msg;//消息获取
	
};
