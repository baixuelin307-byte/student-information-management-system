#pragma once
#include "BasicWidget.h"
#include"configure.h"
#include <vector>
#include <string>
#include"pushButton.h"
class Table :
    public BasicWidget
{
    //����ʼ�������кͿ�
public:
    Table(int row = 0, int col = 0);


      //�����к���
        void setRowcount(int row);


        void setColcount(int col);
        void setHeader(const std::string& header);//����table�ı�ͷ������һ��header�����޸ģ�

        ~Table();
        bool m_btnInited = false;   // ��ť�Ƿ��Ѵ���
        void drawButton();

        void insert(const std::string& data);

    //��ʾ���
        void show();

        void update();//����һ��ҳ�棬��ҳ����չʾ�ڶ�ҳ��ѧ����Ϣ��

        void drawTableGrid();
        void drawTabledata();
        void eventloop(const ExMessage&msg);//������һҳ��һҳ�İ�ť�ȵ�ȥ�л�ҳ��
       static std::vector<std::string>split(std::string str, char separator = '\t');//����ǰ����ݶ����������ķ�����\tȥ����ÿһ������

       


private:
    int m_row;

    int m_col;
    //���ӵĿ�Ⱥ͸߶�
    int m_gridw;
    int m_gridH;
    //���ֵĿ�Ⱥ͸߶�
    int m_tw;
    int m_th;
    std::string m_header;
    std::vector<std::string> m_datas;


    //ѧ����Ϣ��ҳ����
private:
    int m_curPage;//��ǰҳ
    int m_maxPage;//���ҳ
    int m_extraData;//���һҳʣ���������

    PushButton* last_page;
    PushButton* next_page;//����ҳ����ʱ��������Ҫд�İ�ť������pushbutton��
    PushButton* first_page;
    PushButton* end_page;
     
};

