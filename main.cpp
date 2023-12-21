// main.cpp

#include<iostream>
#include<string>

#include "functions.h"  // 包含头文件

using namespace std;

int main() {
    Forum* F = new Forum("模拟论坛", 5, false); // 创建论坛指针
    int forum_inited = 0;  // 初始化标志
    int choice = -1;  // 用户选择操作的变量
    now_post_id = 1;
    now_comment_id = 1;
    while (choice != 0) {
        showMenu(F, choice);  // 显示菜单

        // 处理输入非数字的情况
        if (!cin) {
            cout << "\n输入操作选择编号非数字！请重新输入\n";
            cin.clear();
            while (cin.get() != '\n');  // 清空输入缓冲区
            choice = -1;
            continue;
        }

        switch (choice) {
            case 0:
                cout << "论坛已结束运行" << endl;
                return 0;

            case 1:
                // 初始化论坛
                if(forum_inited)
                {
                    cout << "论坛已经初始化过了哦~" << endl;
                    break;
                }
                if (initForum(F)) {
                    cout << "\n初始化成功！\n";
                    forum_inited = 1;
                } else {
                    cout << "\n初始化失败！\n";
                }
                break;

            case 2:
                // 发帖
                if (!forum_inited) {
                    cout << "\n论坛尚未初始化TAT，请先初始化\n";
                } else {
                    // 创建帖子
                    if(createPost(F) == OK)
                        cout << "发帖成功！" << endl;
                    else
                        cout << "发帖失败！" << endl;
                }
                break;

            case 3:
                // 查看帖子
                if (!forum_inited) {
                    cout << "\n论坛尚未初始化TAT，请先初始化\n";
                } else {
                    if(showSecondMenu(F) == OK)break;
                }
                break;
            case 4:
                // 查找帖子
                if (!forum_inited) {
                    cout << "\n论坛尚未初始化TAT，请先初始化\n";
                } else {
                    if(searchPost(F) == OK)break;
                }
                break;
            case 5:
                // 通过帖子id删除帖子
                if (!forum_inited) {
                    cout << "\n论坛尚未初始化TAT，请先初始化\n";
                } else {
                    if(deletePost(F) == OK)break;
                }
                break;
            case 6:
                // 论坛设置
                if (!forum_inited) {
                    cout << "\n论坛尚未初始化TAT，请先初始化\n";
                } else {
                    if(forumSetting(F) == OK)break;
                }
                break;
            default:
                cout << "输入的选择无效，请重新输入！" << endl;
                break;
        }
        system("pause");
    }

    return 0;
}
