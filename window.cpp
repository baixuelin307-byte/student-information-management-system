#include "window.h"
#include<iostream>
#include <graphics.h>

ExMessage window::m_msg;
window::window(int w, int h, int flag)
{
	m_handle = ::initgraph(w, h, flag);
	::setbkmode(TRANSPARENT);
}

void window::setWindowTilte(const std::string& title)
{
	::SetWindowTextA(m_handle, title.c_str());
}


int window::exec()
{
	return getchar();
}

int window::width()
{
	return getwidth();
}

int window::height()
{
	return getheight();
}

void window::clear()

{
	::cleardevice();
}

void window::beginDraw()
{
	::BeginBatchDraw();
}

void window::flushDraw()
{
	::FlushBatchDraw();
}

void window::endDraw()
{
	EndBatchDraw();
}
