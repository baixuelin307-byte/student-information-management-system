#pragma once
#include <iostream>
#include <vector>
#include "Table.h"
#include "Management.h"
using namespace std;

// ============= 可选：是否先做 split 测试 =============
#define RUN_SPLIT_TEST 1

// 小测试：验证 Table::split 是否正常
static int test_split() {
    // 建议明确传分隔符 '\t'（制表符）
    auto res = Table::split("3140620002\t陈佳琪\t计算机1401\t78\t95\t83", '\t');

    cout << "[split result] count = " << res.size() << '\n';
    for (const auto& v : res) {
        cout << "  [" << v << "]\n";
    }
    cout << endl;
    return 0;
}

int main() {
    // 如果你想先看 split 的输出，把开关置 1
#if RUN_SPLIT_TEST
    test_split();
#endif

    // ======== 图形窗口部分 ========
    initgraph(960, 640, EX_SHOWCONSOLE);         // 初始化图形窗口，并带控制台
    SetWindowTextA(GetHWnd(), "学生管理系统");     // 设置标题

    cout << "图形窗口已打开，按任意键退出" << endl;

    Management m;
    m.drawBackground();
    m.menu();

    // 如果你需要先停在菜单界面看一下 Console 输出，可以保留这句：
    getchar();  // 阻止窗口立即关闭

    m.run();

    closegraph(); // 正确关闭
    return 0;
}