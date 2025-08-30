#include "Management.h"
#include<iostream>
#include<conio.h>
#include"window.h"
#include<fstream>
#include<sstream>
#include <graphics.h>
#include<vector>
#include <memory>
#include"utils.h"
using namespace std;
//set active class
Management::Management()
{
	loadimage(&m_bk, _T("IMAGE\\bk.png"), window::width(), window::height());
	// main page button initate
	readFile("./image/student.txt");
	settextstyle(24, 0, _T("微软雅黑"));
	

	
	menu_btns.push_back(new PushButton(L"查看学生", 200, 100, 200, 40));
	menu_btns.push_back(new PushButton(L"添加学生", 200, 150, 200, 40));
	menu_btns.push_back(new PushButton(L"删除学生", 200, 200, 200, 40));
	menu_btns.push_back(new PushButton(L"修改学生", 200, 250, 200, 40));
	menu_btns.push_back(new PushButton(L"查找学生", 200, 300, 200, 40));
	menu_btns.push_back(new PushButton(L"退出系统", 200, 350, 200, 40));

	
    for (int i = 0;i < menu_btns.size();i++) {
		menu_btns[i]->setFixedSize(250, 50);
		int bx = (window::width() - menu_btns[i]->width()) / 2;
		int vspace = (window::height() - menu_btns.size() * menu_btns[i]->height()) / 2;
		int by = vspace +  i * menu_btns[i]->height();
		menu_btns[i]->move(bx, by);
	}

	//查看这个表格
	m_showTable = std::make_unique<Table>();//new一个新表格
	m_showTable->setRowcount(20);//表格的行数和列数
	m_showTable->setHeader(m_header);//对应之前的文件阅读的表头就可以
	m_showTable->show();//显示这个表格；
	for (auto& val : vec_stu) {
		m_showTable->insert(val.formatInfo());
	}
	//添加学生信息的按钮显示

	m_addBtn = std::make_unique<PushButton>(L"add", 600, 300, 80, 30);

	m_addEdit.reset(new LineEdit(200,260,400,40));
	m_delBtn = std::make_unique<PushButton>(L"delete", 600, 300, 80, 30);

	m_delEdit.reset(new LineEdit(200, 260, 400, 40));
	m_delTable.reset(new Table);


	m_modifyBtn = std::make_unique<PushButton>(L"modify", 600, 400, 80, 30);
	m_modifyEdit = std::make_unique<LineEdit>(200, 380, 400, 40);

	m_addIdEd = std::make_unique<LineEdit>(200, 140, 260, 40); m_addIdEd->setTitle(L"学号");
	m_addNameEd = std::make_unique<LineEdit>(200, 190, 260, 40); m_addNameEd->setTitle(L"姓名");
	m_addGradeEd = std::make_unique<LineEdit>(200, 240, 260, 40); m_addGradeEd->setTitle(L"班级");
	m_addMathEd = std::make_unique<LineEdit>(500, 140, 260, 40); m_addMathEd->setTitle(L"数学");
	m_addChnEd = std::make_unique<LineEdit>(500, 190, 260, 40); m_addChnEd->setTitle(L"语文");
	m_addEngEd = std::make_unique<LineEdit>(500, 240, 260, 40); m_addEngEd->setTitle(L"英语");



}

void Management::evevtLoop()
{
	m_showTable->show();//显示这个表格；
	m_showTable->eventloop(m_msg);


	m_addBtn->eventloop(m_msg);
	m_addEdit->eventloop(m_msg);

	m_delBtn->eventloop(m_msg);
	m_delEdit->eventloop(m_msg);
   m_modifyBtn->eventloop(m_msg);
	m_modifyEdit->eventloop(m_msg);




}

void Management::run() {
	int op = menu();              // 先进入菜单
	BeginBatchDraw();             // ★ 只在循环外调用一次

	while (true) {
		// --- 事件 ---
		if (window::hasMsg()) {
			m_msg = window::getMsg();
			if (m_msg.message == WM_KEYDOWN && m_msg.vkcode == VK_ESCAPE) {
				op = Menu;
			}
			if (op == Management::Display && m_showTable) {
				m_showTable->eventloop(m_msg);  // 就放这里
			}
		}
		if (_kbhit() && _getch() == 27) {
			op = menu();
		}
		
		// --- 每帧先清屏/画背景（不要在 show() 之后再 putimage）---
		putimage(0, 0, &m_bk);

		// --- 按状态绘制 ---
		switch (op) {
		case Menu: {
			// 只在菜单状态画菜单（不要画表格）
			drawMenu();
			// 轮询按钮选择
			int sel = menu();
			if (sel != Menu) op = sel;
			break;
		}
		case Management::Display: {
			// 确保对象已创建，且只创建一次
			if (!m_showTable) {
				m_showTable = std::make_unique<Table>();
				m_showTable->setRowcount(20);
				m_showTable->setHeader(m_header);
				m_showTable->move(100, 100);
				// 如需要：m_showTable->setData(vec_stu);
			}
			m_showTable->show();       // ★ 在 Display 页面每帧调用
			break;
		}
		case Management::Insert:  add();    break;
		case Management::Remove:  erase();  break;
		case Management::Modify:  modify(); break;
		case Management::Search:  search(); break;
		default:
			savefile("./image/test.txt");
			exit(0);
		}

		
		// --- 刷新一帧 ---
		FlushBatchDraw();
		
		Sleep(16);
	}
	EndBatchDraw();
}
void Management::drawMenu() {
	// 只绘制菜单按钮
	for (auto* b : menu_btns) {
		b->show();
	}
}

void Management::readFile(const std::string& fileName)
{
	fstream read(fileName, ios::in);
	if (!read.is_open()) {
		return;
	}

	char buf[1024] = { 0 };
	read.getline(buf, 1024);
	m_header = buf;

	while (!read.eof())
	{
		char data[1024] = { 0 };
		read.getline(data, 1024);
		if (strlen(data) == 0) {
			break;
		};

		student stu;
		stringstream ss(data);
		ss >> stu.number >> stu.name >> stu.grade
			>> stu.math >> stu.chinese >> stu.english;
		vec_stu.push_back(stu);
	}



	
	read.close();
}

void Management::savefile(const std::string& fileName)
{
	fstream write(fileName, ios::out | ios::trunc);//打开文件or 创建；
	if (!write.is_open()) {
		cerr << fileName << "file open fail" << endl;//打印错误信息

		return;
	}
	//先写表头
	m_header = "学号\t姓名\t班级\t数学\t语文\t英语";
	write.write(m_header.c_str(), m_header.size());//前面是接受文字后面是大小
	write.write("\r\n", 2);
	
	//继续写入数据
	for (auto& val : vec_stu) {
		std::string info = val.formatInfo();
		write.write(info.c_str(), info.size());
		cout << val.formatInfo()<<endl;
	}

	write.close();
}

bool Management::editStudentDialog(student& s)
{
	setfillcolor(RGB(0, 0, 0));
	solidrectangle(0, 0, window::width(), window::height()); // 如果有双缓冲记得先 EndBatchDraw 再画
	// 做个面板
	int px = 180, py = 120, pw = 560, ph = 360;
	setfillcolor(RGB(245, 245, 245));
	setlinecolor(BLACK);
	fillroundrect(px, py, px + pw, py + ph, 12, 12);

	// 标题
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(28, 0, L"微软雅黑");
	outtextxy(px + 20, py + 10, L"编辑学生信息");

	// 五个输入框
	// 位置尽量别和你项目冲突
	LineEdit nameEd(px + 140, py + 60, pw - 180, 34);
	LineEdit gradeEd(px + 140, py + 110, pw - 180, 34);
	LineEdit mathEd(px + 140, py + 160, pw - 180, 34);
	LineEdit chnEd(px + 140, py + 210, pw - 180, 34);
	LineEdit engEd(px + 140, py + 260, pw - 180, 34);

	// 预填内容
	nameEd.setText(str2wstr(s.name));
	gradeEd.setText(str2wstr(s.grade));
	mathEd.setText(std::to_wstring(s.math));
	chnEd.setText(std::to_wstring(s.chinese));
	engEd.setText(std::to_wstring(s.english));

	// 标签
	settextstyle(22, 0, L"微软雅黑");
	outtextxy(px + 30, py + 64, L"姓名");
	outtextxy(px + 30, py + 114, L"班级");
	outtextxy(px + 30, py + 164, L"数学");
	outtextxy(px + 30, py + 214, L"语文");
	outtextxy(px + 30, py + 264, L"英语");

	// 按钮
	PushButton okBtn(L"保存", px + pw - 220, py + ph - 60, 90, 34);
	PushButton cancelBtn(L"取消", px + pw - 110, py + ph - 60, 90, 34);

	// 主循环（仅处理弹窗内组件）
	while (true) {
		// 画控件
		nameEd.show();  gradeEd.show();  mathEd.show(); chnEd.show(); engEd.show();
		okBtn.show();   cancelBtn.show();

		ExMessage msg;
		while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
			nameEd.eventloop(msg);
			gradeEd.eventloop(msg);
			mathEd.eventloop(msg);
			chnEd.eventloop(msg);
			engEd.eventloop(msg);
			okBtn.eventloop(msg);
			cancelBtn.eventloop(msg);

			if (okBtn.isClicked()) {
				int math, ch, en;
				if (!parseIntWS(mathEd.text(), math) ||
					!parseIntWS(chnEd.text(), ch) ||
					!parseIntWS(engEd.text(), en)) {
					outtextxy(px + 20, py + ph - 95, L"成绩必须为整数");
					break; // 继续循环等待修正
				}
				// 可选的范围校验
				auto okR = [](int v) { return v >= 0 && v <= 150; };
				if (!okR(math) || !okR(ch) || !okR(en)) {
					outtextxy(px + 20, py + ph - 95, L"成绩范围 0~150");
					break;
				}
				// 回写
				s.name = wstr2str(nameEd.text());
				s.grade = wstr2str(gradeEd.text());
				s.math = std::stoi(mathEd.text());     // std::stoi 支持 wstring
				s.chinese = std::stoi(chnEd.text());
				s.english = std::stoi(engEd.text());
				return true;
			}
			if (cancelBtn.isClicked()) {
				return false;
			}
			// ESC 退出
			if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
				return false;
			}
		}

		// 如果你用双缓冲，请 FlushBatchDraw()；否则忽略
		FlushBatchDraw();
		Sleep(10);
	}

    return false;
}

void Management::rebuildTable()
{
	
	
	m_showTable = std::make_unique<Table>();
	m_showTable->setRowcount(20);
	m_showTable->setHeader(m_header);

	// 把内存里的学生数据重新插入到表格
	for (const auto& s : vec_stu) {
		// 如果 formatInfo() 返回 std::string
		m_showTable->insert(s.formatInfo());

		// 如果你的是宽字符版：m_showTable->insert(s.formatInfoW());
		// 或者转换：m_showTable->insert(wstr2str(s.formatInfoW()));
	}









}

void Management::onAddClicked()
{
	student s;
	if (!addStudentDialog(s)) {                      // 你之前写的弹 6 个 InputBox 的函数
		outtextxy(m_addBtn->x(), m_addBtn->y() + 60, L"已取消");
		return;
	}

	// 2. 学号重复校验（必要）
	auto itDup = std::find_if(vec_stu.begin(), vec_stu.end(),
		[&](const student& t) { return t.number == s.number; });
	if (itDup != vec_stu.end()) {
		outtextxy(m_addBtn->x(), m_addBtn->y() + 60, L"学号已存在");
		return;
	}

	// 3. 入库/刷新/保存（必要）
	vec_stu.push_back(std::move(s));
	rebuildTable();
	savefile("./image/student.txt");

	outtextxy(m_addBtn->x(), m_addBtn->y() + 60, L"添加成功！");
}

bool Management::addStudentDialog(student& out)
{

	wchar_t id[32], name[64], grade[64], math[16], chn[16], eng[16];
	bool ok =
		InputBox(id, 32, L"输入学号", L"添加学生") &&
		InputBox(name, 64, L"输入姓名", L"添加学生") &&
		InputBox(grade, 64, L"输入班级", L"添加学生") &&
		InputBox(math, 16, L"数学成绩", L"添加学生") &&
		InputBox(chn, 16, L"语文成绩", L"添加学生") &&
		InputBox(eng, 16, L"英语成绩", L"添加学生");

	if (!ok) return false;  // 输入被取消，直接返回失败

	try {
		unsigned int uid = std::stoul(id);
		int imath = std::stoi(math);
		int ichn = std::stoi(chn);
		int ieng = std::stoi(eng);

		out.number = uid;
		out.math = imath;
		out.chinese = ichn;
		out.english = ieng;

		// 如果 student 的 name/grade 是 std::string
		out.name = w2u8(name);
		out.grade = w2u8(grade);

		// 如果是 std::wstring 就直接：
		// out.name  = name;
		// out.grade = grade;

		return true;  // ? 成功
	}
	catch (...) {
		return false; // ? 转换失败
	}














	
}









































// 显示学生页面
void Management::display() {
	setbkmode(TRANSPARENT);    
	
	settextcolor(BLACK);  // 设置字体颜色
	settextstyle(30, 0, L"微软雅黑");  // 设置字体样式

	outtextxy(0, 0, _T("display"));
	
}




void Management::add() {
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, L"微软雅黑");
	outtextxy(0, 0, _T("input student information:student id name grade"));

	m_addBtn->show();
	m_addEdit->show();
	

	ExMessage msg;
	
	while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
		m_addBtn->eventloop(msg);
		m_addIdEd->eventloop(msg);
		m_addNameEd->eventloop(msg);
		m_addGradeEd->eventloop(msg);
		m_addMathEd->eventloop(msg);
		m_addChnEd->eventloop(msg);
		m_addEngEd->eventloop(msg);

		if (m_addBtn->isClicked()) {
			onAddClicked();   // ★★ 真正执行“添加”动作
			
		}
	}



}
// 其他页面类似
void Management::erase() {
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, L"微软雅黑");
	outtextxy(0, 0, _T("erase"));

	m_delBtn->show();
	m_delEdit->show();

	ExMessage msg;
	while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
		m_delBtn->eventloop(msg);
		m_delEdit->eventloop(msg);

		if (m_delBtn->isClicked()) {                     // 只在点击时处理一次
			const std::wstring& key = m_delEdit->text(); // 学号
			std::wcout << L"delete\n" << key << std::endl;

			if (key.empty()) break;                      // 空就不处理

			// 1) 查找
			auto it = std::find_if(vec_stu.begin(), vec_stu.end(),
				[&](const student& stu) {
					return std::to_wstring(stu.number) == key;
				});

			if (it == vec_stu.end()) {
				// 2) 未找到 —— 只提示，不要调用 erase
				std::wstring msgW = L"sorry, i did not find student id " + key + L" information";
				outtextxy(m_delEdit->x(), m_delEdit->y() + 50, msgW.c_str());
				break;
			}

			// 3) （可选）删除前先把将要删除的行显示出来
			{
				std::string line = it->formatInfo();
				std::wstring wline(line.begin(), line.end());   // 简单转换，非 UTF-8 环境可用
				outtextxy(m_delEdit->x(), m_delEdit->y() + 50, wline.c_str());
			}

			// 4) 真正删除 —— 之后不要再用 it
			vec_stu.erase(it);

			// 5) 重新构建/刷新表格
			if (!m_delTable) m_delTable = std::make_unique<Table>();
			m_delTable->setRowcount(20);
			m_delTable->setHeader(m_header);
			// 如果 Table 有清空接口，先清空一遍再插入
			// m_delTable->clear();

			for (const auto& s : vec_stu) {
				m_delTable->insert(s.formatInfo());    // formatInfo() 返回 string，接口不变
			}

			// 6) 保存回文件
			savefile("./image/student.txt");

			// 7) 提示已删除
			std::wstring ok = L"已删除学号为 " + key + L" 的学生记录。";
			outtextxy(m_delEdit->x(), m_delEdit->y() + 80, ok.c_str());

			break;  // 本次点击处理完成，跳出本次消息循环
		}
	}
}

	



	





void Management::modify() {
	setbkmode(TRANSPARENT);
	
	settextcolor(BLACK);
	settextstyle(30, 0, L"微软雅黑");
	outtextxy(0, 0, _T("modify"));
	
	m_modifyBtn->show();
	m_modifyEdit->show();


	ExMessage msg;
	while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
		m_modifyBtn->eventloop(msg);
		m_modifyEdit->eventloop(msg);

		if (m_modifyBtn->isClicked()) {
			const std::wstring key = m_modifyEdit->text();
			if (key.empty()) {
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 50, L"请输入学号");
				continue;
			}

			auto it = std::find_if(vec_stu.begin(), vec_stu.end(),
				[&](const student& s) { return std::to_wstring(s.number) == key; });

			if (it == vec_stu.end()) {
				std::wstring msg2 = L"未找到学号 " + key;
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 50, msg2.c_str());
				continue;
			}

			// 弹出对话框编辑
			if (editStudentDialog(*it)) {
				// 成功保存 -> 重建表格并写文件
				rebuildTable();                            // 你已有或按前面那样写
				savefile("./image/student.txt");
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 80, L"已保存修改");
			}
			else {
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 80, L"取消修改");
			}
		}
	}






































}


void Management::search() {
	setbkmode(TRANSPARENT);
	
	settextcolor(BLACK);
	settextstyle(30, 0, L"微软雅黑");
	outtextxy(0, 0, _T("search"));
	cout << "SEARCH" << endl;
}

int Management::menu() {
	for (int i = 0; i < menu_btns.size(); i++) {
		menu_btns[i]->show();
		menu_btns[i]->eventloop(m_msg);  // 先处理事件
		if (menu_btns[i]->isClicked()) {  // 判断是否点击了
			return i;  // 返回点击按钮的索引
		}
	}
	return Menu;  // 如果没有点击任何按钮，返回 Menu
}


void Management::drawBackground()
{
	putimage(0, 0, &m_bk);
}

