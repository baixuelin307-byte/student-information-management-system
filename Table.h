#pragma once
#include "BasicWidget.h"
#include"configure.h"
#include <vector>
#include <string>
#include"pushButton.h"
class Table :
    public BasicWidget
{
    //表格初始化设置行和宽；
public:
    Table(int row = 0, int col = 0);


      //设置行和列
        void setRowcount(int row);


        void setColcount(int col);
        void setHeader(const std::string& header);//建立table的表头，设置一个header不能修改；

        ~Table();
        bool m_btnInited = false;   // 按钮是否已创建
        void drawButton();

        void insert(const std::string& data);

    //显示表格
        void show();

        void update();//更新一下页面，分页处理，展示第二页的学生信息；

        void drawTableGrid();
        void drawTabledata();
        void eventloop(const ExMessage&msg);//根据上一页下一页的按钮等等去切换页面
       static std::vector<std::string>split(std::string str, char separator = '\t');//这个是吧数据读出来单独的返回用\t去分离每一段数据

       


private:
    int m_row;

    int m_col;
    //格子的宽度和高度
    int m_gridw;
    int m_gridH;
    //文字的宽度和高度
    int m_tw;
    int m_th;
    std::string m_header;
    std::vector<std::string> m_datas;


    //学生信息分页管理
private:
    int m_curPage;//当前页
    int m_maxPage;//最大页
    int m_extraData;//最后一页剩余多少数据

    PushButton* last_page;
    PushButton* next_page;//做分页处理时，我们需要写的按钮，基于pushbutton；
    PushButton* first_page;
    PushButton* end_page;
     
};

