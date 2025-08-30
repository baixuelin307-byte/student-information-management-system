#include "Table.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#undef min
#undef max
#include <windows.h>   // MultiByteToWideChar
#include <graphics.h>  // EasyX textwidth/textheight/outtextxy
#include "PushButton.h"
// 如果 PushButton 有接收标题的构造函数
Table::Table(int row, int col)
    : BasicWidget(0, 0, 0, 0)
    , m_row(row), m_col(col)
   
{
}


// 把 ANSI(本地代码页) 字符串转成宽字符串（工程是 Unicode 时用）
static std::wstring AnsiToWide(const std::string& s) {
    if (s.empty()) return std::wstring();
    int n = MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, nullptr, 0);
    std::wstring ws(n ? n - 1 : 0, L'\0');
    if (n > 1) MultiByteToWideChar(CP_ACP, 0, s.c_str(), -1, &ws[0], n);
    return ws;
}













// 按“任意空白”切分（空格/Tab/多个空格都可），返回各单元格
static std::vector<std::string> splitByWhitespace(const std::string& line) {
    std::istringstream iss(line);
    std::string tok;
    std::vector<std::string> cells;
    while (iss >> tok) cells.push_back(tok);
    return cells;
}





void Table::setRowcount(int row) {


    m_row = row;
    update();
}
void Table::setColcount(int col) { m_col = col; }

void Table::setHeader(const std::string& header)
{
    m_header = header;

    // 1) 统计列数（按空白切分）
    auto cells = splitByWhitespace(m_header);
    m_col = static_cast<int>(cells.size());
    std::cout << "header: " << m_header << "\ncols = " << m_col << std::endl;

    // 2) 测量文字宽高（存到成员变量）
    std::wstring wHeader = AnsiToWide(m_header);





    m_tw = textheight(_T("计算机1401"));
    m_th = textwidth(wHeader.c_str());




    m_gridw = textwidth(wHeader.c_str());      // 每格宽度
    m_gridH = textheight(_T("计算机1401"))+10;    // 每格高度


    //总宽度和总高度
    m_w = m_gridw * m_col;
    m_h = m_gridH * m_row;











    std::cout << "text width = " << m_gridw << ", text height = " << m_gridH << std::endl;
    const int marginX = 1;   // 左右边距
    const int marginY = 1;  // 顶部/底部边距
    m_x = marginX;
    m_y = marginY;

}

Table::~Table()
{
    delete last_page;
     delete next_page;
    delete first_page;
  delete  end_page;//删除内存
}

void Table::drawButton() {
    const int w = getwidth();
    const int h = getheight();
    const int btnW = 100, btnH = 32, gap = 10, margin = 20;

    if (!m_btnInited) {              // ★ 只做一次
        const int y = h - btnH - margin;
        int x = w - margin - btnW;

        end_page = new PushButton(L"end page", x, y, btnW, btnH);
        x -= (btnW + gap);
        first_page = new PushButton(L"first page", x, y, btnW, btnH);
        x -= (btnW + gap);
        next_page = new PushButton(L"next page", x, y, btnW, btnH);
        x -= (btnW + gap);
        last_page = new PushButton(L"last page", x, y, btnW, btnH);

        m_btnInited = true;
    }

    // 每帧只画
    if (last_page)  last_page->show();
    if (next_page)  next_page->show();
    if (first_page) first_page->show();
    if (end_page)   end_page->show();
}












void Table::insert(const std::string& data)
{
    m_datas.push_back(data);
    
    update();// insert data after update this
}

void Table::show()
{
    drawTableGrid();
    drawTabledata();
   
}



void Table::drawTableGrid()
{
    setlinecolor(BLACK);
    // 画横线
    for (size_t i = 0; i < m_row + 1; i++)   // 去掉分号
    {


        line(m_x, m_y + i * m_gridH, m_x + m_col * m_gridw, m_y + i * m_gridH);
    }

    //画竖线

    for (size_t i = 0; i < m_col + 1; i++)
    {
        // 原来的主竖线
        line(m_x + i * m_gridw, m_y, m_x + i * m_gridw, m_y + m_row * m_gridH);

        // 如果需要在每格里多加竖线
        if (i < m_col) {
            int subDiv = 2; // 每格细分成 subDiv 份
            int step = m_gridw / subDiv;
            for (int s = 1; s < subDiv; ++s) {
                int xSub = m_x + i * m_gridw + s * step;
                line(xSub, m_y, xSub, m_y + m_row * m_gridH);
            }
        }
    }


    if (m_maxPage > 0) {
        drawButton();//如果页面大于1，就显示按钮
    }
    else {

    }




















}








void Table::drawTabledata() {
    const size_t n = m_datas.size();
    const size_t r = std::max<size_t>(1, m_row);
    if (n == 0) return;

    // 计算本页的 [start, end)
    const size_t start = std::min(n, static_cast<size_t>(m_curPage) * r);
    const size_t end = std::min(n, start + r);
    if (start >= end) return;  // 空页直接返回

    for (size_t i = start; i < end; ++i) {          // 关键：i < end 且 ++i
        const auto line_data = split(m_datas[i], '\t');

        // 页内行号用于计算 y，避免翻页后 y 位置飘
        const int rowOnPage = static_cast<int>(i - start);

        for (size_t k = 0; k < line_data.size(); ++k) {
            std::wstring ws(line_data[k].begin(), line_data[k].end());
            int tx = m_x + static_cast<int>(k) * m_gridw + 40;
            int ty = m_y + rowOnPage * m_gridH + 5;
            outtextxy(tx, ty, ws.c_str());
        }
    }
}

void Table::eventloop(const ExMessage& msg)
{
    if (last_page) last_page->eventloop(msg);
    if(next_page) next_page->eventloop(msg);
     if(first_page)  first_page->eventloop(msg);
      if(end_page)   end_page->eventloop(msg);

      if (last_page && last_page->isClicked()) {
          if (m_curPage != 0) {
              m_curPage--;
          }
      }

      if (next_page && next_page->isClicked()) {
          if (m_curPage != m_maxPage) {
              m_curPage++;
          }
      }

      if (first_page && first_page->isClicked()) {
          m_curPage = 0;
      }

      if (end_page && end_page->isClicked()) {
          m_curPage = m_maxPage;
      }











}









std::vector<std::string> Table::split(std::string str, char separator)
{
    std::vector<std::string> res;
    std::size_t start = 0;
    while (true) {
        std::size_t pos = str.find(separator, start);
        if (pos == std::string::npos) {
            res.emplace_back(str.substr(start));  // 最后一段
            break;
        }
        res.emplace_back(str.substr(start, pos - start));
        start = pos + 1;  // 跳过分隔符
    }
    return res;
}


void Table::update() {
    const size_t n = m_datas.size();
    const size_t r = std::max<size_t>(1, m_row);   // 防 m_row==0

    // 计算“总页数”
    const size_t pageCount = (n == 0) ? 0 : ((n + r - 1) / r);

    m_extraData = (n == 0) ? 0 : (n % r);

    // 如果你原来的 m_maxPage 表示“最后一页下标”，就这样赋值：
    if (pageCount == 0) {
        m_maxPage = 0;      // 无页
        m_curPage = 0;
        return;
    }
    m_maxPage = pageCount - 1;  // 最后一页下标（0 基）

    if (m_curPage > m_maxPage)  // 夹紧当前页
        m_curPage = m_maxPage;
}