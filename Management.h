#pragma once
#include"window.h"
#include <graphics.h>
#include <stack>
#include <vector>
#include"student.h"
#include"Table.h"
#include<memory>
#include"LineEdit.h"
class Management
{


    enum Operator
    {
        Display,
        Insert,
        Remove,
        Modify,
        Search,
        Menu = 66

    };








public:
    void run();
    void display();
    void add();
    void erase();
    void modify();
    void search();
    int menu();
    void drawBackground();
    Management();
    void evevtLoop();
    void drawMenu();
    void readFile(const std::string& fileName);
    void savefile(const std::string& fileName);
    
    bool editStudentDialog(student& s);
    void rebuildTable();
    void onAddClicked();
    bool addStudentDialog(student& out);
private:
    IMAGE m_bk;
    ExMessage m_msg;
    std::vector<PushButton*>menu_btns;
    std::stack<int> pageHistory;



private:
    std::string m_header;
    std::vector<student>vec_stu;
private:
    
    std::unique_ptr<Table> m_showTable;
    //添加学生信息的页面按钮
    std::unique_ptr<PushButton>m_addBtn;

    std::unique_ptr<LineEdit> m_addEdit;


    std::unique_ptr<LineEdit> m_addIdEd;
    std::unique_ptr<LineEdit> m_addNameEd;
    std::unique_ptr<LineEdit> m_addGradeEd;
    std::unique_ptr<LineEdit> m_addMathEd;
    std::unique_ptr<LineEdit> m_addChnEd;
    std::unique_ptr<LineEdit> m_addEngEd;


   std::unique_ptr<PushButton>m_delBtn;
    std::unique_ptr<LineEdit>m_delEdit;//这是输入框
    std::unique_ptr<Table>m_delTable;//
    std::unique_ptr<LineEdit>m_modifyEdit;

    std::unique_ptr<PushButton>m_modifyBtn;
   
};