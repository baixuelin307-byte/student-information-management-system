#pragma once
#include <iostream>
#include <vector>
#include "Table.h"
#include "Management.h"
using namespace std;

// ============= ��ѡ���Ƿ����� split ���� =============
#define RUN_SPLIT_TEST 1

// С���ԣ���֤ Table::split �Ƿ�����
static int test_split() {
    // ������ȷ���ָ��� '\t'���Ʊ����
    auto res = Table::split("3140620002\t�¼���\t�����1401\t78\t95\t83", '\t');

    cout << "[split result] count = " << res.size() << '\n';
    for (const auto& v : res) {
        cout << "  [" << v << "]\n";
    }
    cout << endl;
    return 0;
}

int main() {
    // ��������ȿ� split ��������ѿ����� 1
#if RUN_SPLIT_TEST
    test_split();
#endif

    // ======== ͼ�δ��ڲ��� ========
    initgraph(960, 640, EX_SHOWCONSOLE);         // ��ʼ��ͼ�δ��ڣ���������̨
    SetWindowTextA(GetHWnd(), "ѧ������ϵͳ");     // ���ñ���

    cout << "ͼ�δ����Ѵ򿪣���������˳�" << endl;

    Management m;
    m.drawBackground();
    m.menu();

    // �������Ҫ��ͣ�ڲ˵����濴һ�� Console ��������Ա�����䣺
    getchar();  // ��ֹ���������ر�

    m.run();

    closegraph(); // ��ȷ�ر�
    return 0;
}