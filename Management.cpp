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
	settextstyle(24, 0, _T("΢���ź�"));
	

	
	menu_btns.push_back(new PushButton(L"�鿴ѧ��", 200, 100, 200, 40));
	menu_btns.push_back(new PushButton(L"���ѧ��", 200, 150, 200, 40));
	menu_btns.push_back(new PushButton(L"ɾ��ѧ��", 200, 200, 200, 40));
	menu_btns.push_back(new PushButton(L"�޸�ѧ��", 200, 250, 200, 40));
	menu_btns.push_back(new PushButton(L"����ѧ��", 200, 300, 200, 40));
	menu_btns.push_back(new PushButton(L"�˳�ϵͳ", 200, 350, 200, 40));

	
    for (int i = 0;i < menu_btns.size();i++) {
		menu_btns[i]->setFixedSize(250, 50);
		int bx = (window::width() - menu_btns[i]->width()) / 2;
		int vspace = (window::height() - menu_btns.size() * menu_btns[i]->height()) / 2;
		int by = vspace +  i * menu_btns[i]->height();
		menu_btns[i]->move(bx, by);
	}

	//�鿴������
	m_showTable = std::make_unique<Table>();//newһ���±��
	m_showTable->setRowcount(20);//��������������
	m_showTable->setHeader(m_header);//��Ӧ֮ǰ���ļ��Ķ��ı�ͷ�Ϳ���
	m_showTable->show();//��ʾ������
	for (auto& val : vec_stu) {
		m_showTable->insert(val.formatInfo());
	}
	//���ѧ����Ϣ�İ�ť��ʾ

	m_addBtn = std::make_unique<PushButton>(L"add", 600, 300, 80, 30);

	m_addEdit.reset(new LineEdit(200,260,400,40));
	m_delBtn = std::make_unique<PushButton>(L"delete", 600, 300, 80, 30);

	m_delEdit.reset(new LineEdit(200, 260, 400, 40));
	m_delTable.reset(new Table);


	m_modifyBtn = std::make_unique<PushButton>(L"modify", 600, 400, 80, 30);
	m_modifyEdit = std::make_unique<LineEdit>(200, 380, 400, 40);

	m_addIdEd = std::make_unique<LineEdit>(200, 140, 260, 40); m_addIdEd->setTitle(L"ѧ��");
	m_addNameEd = std::make_unique<LineEdit>(200, 190, 260, 40); m_addNameEd->setTitle(L"����");
	m_addGradeEd = std::make_unique<LineEdit>(200, 240, 260, 40); m_addGradeEd->setTitle(L"�༶");
	m_addMathEd = std::make_unique<LineEdit>(500, 140, 260, 40); m_addMathEd->setTitle(L"��ѧ");
	m_addChnEd = std::make_unique<LineEdit>(500, 190, 260, 40); m_addChnEd->setTitle(L"����");
	m_addEngEd = std::make_unique<LineEdit>(500, 240, 260, 40); m_addEngEd->setTitle(L"Ӣ��");



}

void Management::evevtLoop()
{
	m_showTable->show();//��ʾ������
	m_showTable->eventloop(m_msg);


	m_addBtn->eventloop(m_msg);
	m_addEdit->eventloop(m_msg);

	m_delBtn->eventloop(m_msg);
	m_delEdit->eventloop(m_msg);
   m_modifyBtn->eventloop(m_msg);
	m_modifyEdit->eventloop(m_msg);




}

void Management::run() {
	int op = menu();              // �Ƚ���˵�
	BeginBatchDraw();             // �� ֻ��ѭ�������һ��

	while (true) {
		// --- �¼� ---
		if (window::hasMsg()) {
			m_msg = window::getMsg();
			if (m_msg.message == WM_KEYDOWN && m_msg.vkcode == VK_ESCAPE) {
				op = Menu;
			}
			if (op == Management::Display && m_showTable) {
				m_showTable->eventloop(m_msg);  // �ͷ�����
			}
		}
		if (_kbhit() && _getch() == 27) {
			op = menu();
		}
		
		// --- ÿ֡������/����������Ҫ�� show() ֮���� putimage��---
		putimage(0, 0, &m_bk);

		// --- ��״̬���� ---
		switch (op) {
		case Menu: {
			// ֻ�ڲ˵�״̬���˵�����Ҫ�����
			drawMenu();
			// ��ѯ��ťѡ��
			int sel = menu();
			if (sel != Menu) op = sel;
			break;
		}
		case Management::Display: {
			// ȷ�������Ѵ�������ֻ����һ��
			if (!m_showTable) {
				m_showTable = std::make_unique<Table>();
				m_showTable->setRowcount(20);
				m_showTable->setHeader(m_header);
				m_showTable->move(100, 100);
				// ����Ҫ��m_showTable->setData(vec_stu);
			}
			m_showTable->show();       // �� �� Display ҳ��ÿ֡����
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

		
		// --- ˢ��һ֡ ---
		FlushBatchDraw();
		
		Sleep(16);
	}
	EndBatchDraw();
}
void Management::drawMenu() {
	// ֻ���Ʋ˵���ť
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
	fstream write(fileName, ios::out | ios::trunc);//���ļ�or ������
	if (!write.is_open()) {
		cerr << fileName << "file open fail" << endl;//��ӡ������Ϣ

		return;
	}
	//��д��ͷ
	m_header = "ѧ��\t����\t�༶\t��ѧ\t����\tӢ��";
	write.write(m_header.c_str(), m_header.size());//ǰ���ǽ������ֺ����Ǵ�С
	write.write("\r\n", 2);
	
	//����д������
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
	solidrectangle(0, 0, window::width(), window::height()); // �����˫����ǵ��� EndBatchDraw �ٻ�
	// �������
	int px = 180, py = 120, pw = 560, ph = 360;
	setfillcolor(RGB(245, 245, 245));
	setlinecolor(BLACK);
	fillroundrect(px, py, px + pw, py + ph, 12, 12);

	// ����
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(28, 0, L"΢���ź�");
	outtextxy(px + 20, py + 10, L"�༭ѧ����Ϣ");

	// ��������
	// λ�þ����������Ŀ��ͻ
	LineEdit nameEd(px + 140, py + 60, pw - 180, 34);
	LineEdit gradeEd(px + 140, py + 110, pw - 180, 34);
	LineEdit mathEd(px + 140, py + 160, pw - 180, 34);
	LineEdit chnEd(px + 140, py + 210, pw - 180, 34);
	LineEdit engEd(px + 140, py + 260, pw - 180, 34);

	// Ԥ������
	nameEd.setText(str2wstr(s.name));
	gradeEd.setText(str2wstr(s.grade));
	mathEd.setText(std::to_wstring(s.math));
	chnEd.setText(std::to_wstring(s.chinese));
	engEd.setText(std::to_wstring(s.english));

	// ��ǩ
	settextstyle(22, 0, L"΢���ź�");
	outtextxy(px + 30, py + 64, L"����");
	outtextxy(px + 30, py + 114, L"�༶");
	outtextxy(px + 30, py + 164, L"��ѧ");
	outtextxy(px + 30, py + 214, L"����");
	outtextxy(px + 30, py + 264, L"Ӣ��");

	// ��ť
	PushButton okBtn(L"����", px + pw - 220, py + ph - 60, 90, 34);
	PushButton cancelBtn(L"ȡ��", px + pw - 110, py + ph - 60, 90, 34);

	// ��ѭ�������������������
	while (true) {
		// ���ؼ�
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
					outtextxy(px + 20, py + ph - 95, L"�ɼ�����Ϊ����");
					break; // ����ѭ���ȴ�����
				}
				// ��ѡ�ķ�ΧУ��
				auto okR = [](int v) { return v >= 0 && v <= 150; };
				if (!okR(math) || !okR(ch) || !okR(en)) {
					outtextxy(px + 20, py + ph - 95, L"�ɼ���Χ 0~150");
					break;
				}
				// ��д
				s.name = wstr2str(nameEd.text());
				s.grade = wstr2str(gradeEd.text());
				s.math = std::stoi(mathEd.text());     // std::stoi ֧�� wstring
				s.chinese = std::stoi(chnEd.text());
				s.english = std::stoi(engEd.text());
				return true;
			}
			if (cancelBtn.isClicked()) {
				return false;
			}
			// ESC �˳�
			if (msg.message == WM_KEYDOWN && msg.vkcode == VK_ESCAPE) {
				return false;
			}
		}

		// �������˫���壬�� FlushBatchDraw()���������
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

	// ���ڴ����ѧ���������²��뵽���
	for (const auto& s : vec_stu) {
		// ��� formatInfo() ���� std::string
		m_showTable->insert(s.formatInfo());

		// �������ǿ��ַ��棺m_showTable->insert(s.formatInfoW());
		// ����ת����m_showTable->insert(wstr2str(s.formatInfoW()));
	}









}

void Management::onAddClicked()
{
	student s;
	if (!addStudentDialog(s)) {                      // ��֮ǰд�ĵ� 6 �� InputBox �ĺ���
		outtextxy(m_addBtn->x(), m_addBtn->y() + 60, L"��ȡ��");
		return;
	}

	// 2. ѧ���ظ�У�飨��Ҫ��
	auto itDup = std::find_if(vec_stu.begin(), vec_stu.end(),
		[&](const student& t) { return t.number == s.number; });
	if (itDup != vec_stu.end()) {
		outtextxy(m_addBtn->x(), m_addBtn->y() + 60, L"ѧ���Ѵ���");
		return;
	}

	// 3. ���/ˢ��/���棨��Ҫ��
	vec_stu.push_back(std::move(s));
	rebuildTable();
	savefile("./image/student.txt");

	outtextxy(m_addBtn->x(), m_addBtn->y() + 60, L"��ӳɹ���");
}

bool Management::addStudentDialog(student& out)
{

	wchar_t id[32], name[64], grade[64], math[16], chn[16], eng[16];
	bool ok =
		InputBox(id, 32, L"����ѧ��", L"���ѧ��") &&
		InputBox(name, 64, L"��������", L"���ѧ��") &&
		InputBox(grade, 64, L"����༶", L"���ѧ��") &&
		InputBox(math, 16, L"��ѧ�ɼ�", L"���ѧ��") &&
		InputBox(chn, 16, L"���ĳɼ�", L"���ѧ��") &&
		InputBox(eng, 16, L"Ӣ��ɼ�", L"���ѧ��");

	if (!ok) return false;  // ���뱻ȡ����ֱ�ӷ���ʧ��

	try {
		unsigned int uid = std::stoul(id);
		int imath = std::stoi(math);
		int ichn = std::stoi(chn);
		int ieng = std::stoi(eng);

		out.number = uid;
		out.math = imath;
		out.chinese = ichn;
		out.english = ieng;

		// ��� student �� name/grade �� std::string
		out.name = w2u8(name);
		out.grade = w2u8(grade);

		// ����� std::wstring ��ֱ�ӣ�
		// out.name  = name;
		// out.grade = grade;

		return true;  // ? �ɹ�
	}
	catch (...) {
		return false; // ? ת��ʧ��
	}














	
}









































// ��ʾѧ��ҳ��
void Management::display() {
	setbkmode(TRANSPARENT);    
	
	settextcolor(BLACK);  // ����������ɫ
	settextstyle(30, 0, L"΢���ź�");  // ����������ʽ

	outtextxy(0, 0, _T("display"));
	
}




void Management::add() {
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, L"΢���ź�");
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
			onAddClicked();   // ��� ����ִ�С���ӡ�����
			
		}
	}



}
// ����ҳ������
void Management::erase() {
	setbkmode(TRANSPARENT);
	settextcolor(BLACK);
	settextstyle(30, 0, L"΢���ź�");
	outtextxy(0, 0, _T("erase"));

	m_delBtn->show();
	m_delEdit->show();

	ExMessage msg;
	while (peekmessage(&msg, EM_MOUSE | EM_KEY)) {
		m_delBtn->eventloop(msg);
		m_delEdit->eventloop(msg);

		if (m_delBtn->isClicked()) {                     // ֻ�ڵ��ʱ����һ��
			const std::wstring& key = m_delEdit->text(); // ѧ��
			std::wcout << L"delete\n" << key << std::endl;

			if (key.empty()) break;                      // �վͲ�����

			// 1) ����
			auto it = std::find_if(vec_stu.begin(), vec_stu.end(),
				[&](const student& stu) {
					return std::to_wstring(stu.number) == key;
				});

			if (it == vec_stu.end()) {
				// 2) δ�ҵ� ���� ֻ��ʾ����Ҫ���� erase
				std::wstring msgW = L"sorry, i did not find student id " + key + L" information";
				outtextxy(m_delEdit->x(), m_delEdit->y() + 50, msgW.c_str());
				break;
			}

			// 3) ����ѡ��ɾ��ǰ�Ȱѽ�Ҫɾ��������ʾ����
			{
				std::string line = it->formatInfo();
				std::wstring wline(line.begin(), line.end());   // ��ת������ UTF-8 ��������
				outtextxy(m_delEdit->x(), m_delEdit->y() + 50, wline.c_str());
			}

			// 4) ����ɾ�� ���� ֮��Ҫ���� it
			vec_stu.erase(it);

			// 5) ���¹���/ˢ�±��
			if (!m_delTable) m_delTable = std::make_unique<Table>();
			m_delTable->setRowcount(20);
			m_delTable->setHeader(m_header);
			// ��� Table ����սӿڣ������һ���ٲ���
			// m_delTable->clear();

			for (const auto& s : vec_stu) {
				m_delTable->insert(s.formatInfo());    // formatInfo() ���� string���ӿڲ���
			}

			// 6) ������ļ�
			savefile("./image/student.txt");

			// 7) ��ʾ��ɾ��
			std::wstring ok = L"��ɾ��ѧ��Ϊ " + key + L" ��ѧ����¼��";
			outtextxy(m_delEdit->x(), m_delEdit->y() + 80, ok.c_str());

			break;  // ���ε��������ɣ�����������Ϣѭ��
		}
	}
}

	



	





void Management::modify() {
	setbkmode(TRANSPARENT);
	
	settextcolor(BLACK);
	settextstyle(30, 0, L"΢���ź�");
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
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 50, L"������ѧ��");
				continue;
			}

			auto it = std::find_if(vec_stu.begin(), vec_stu.end(),
				[&](const student& s) { return std::to_wstring(s.number) == key; });

			if (it == vec_stu.end()) {
				std::wstring msg2 = L"δ�ҵ�ѧ�� " + key;
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 50, msg2.c_str());
				continue;
			}

			// �����Ի���༭
			if (editStudentDialog(*it)) {
				// �ɹ����� -> �ؽ����д�ļ�
				rebuildTable();                            // �����л�ǰ������д
				savefile("./image/student.txt");
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 80, L"�ѱ����޸�");
			}
			else {
				outtextxy(m_modifyEdit->x(), m_modifyEdit->y() + 80, L"ȡ���޸�");
			}
		}
	}






































}


void Management::search() {
	setbkmode(TRANSPARENT);
	
	settextcolor(BLACK);
	settextstyle(30, 0, L"΢���ź�");
	outtextxy(0, 0, _T("search"));
	cout << "SEARCH" << endl;
}

int Management::menu() {
	for (int i = 0; i < menu_btns.size(); i++) {
		menu_btns[i]->show();
		menu_btns[i]->eventloop(m_msg);  // �ȴ����¼�
		if (menu_btns[i]->isClicked()) {  // �ж��Ƿ�����
			return i;  // ���ص����ť������
		}
	}
	return Menu;  // ���û�е���κΰ�ť������ Menu
}


void Management::drawBackground()
{
	putimage(0, 0, &m_bk);
}

