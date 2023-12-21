// main.cpp

#include<iostream>
#include<string>

#include "functions.h"  // ����ͷ�ļ�

using namespace std;

int main() {
    Forum* F = new Forum("ģ����̳", 5, false); // ������ָ̳��
    int forum_inited = 0;  // ��ʼ����־
    int choice = -1;  // �û�ѡ������ı���
    now_post_id = 1;
    now_comment_id = 1;
    while (choice != 0) {
        showMenu(F, choice);  // ��ʾ�˵�

        // ������������ֵ����
        if (!cin) {
            cout << "\n�������ѡ���ŷ����֣�����������\n";
            cin.clear();
            while (cin.get() != '\n');  // ������뻺����
            choice = -1;
            continue;
        }

        switch (choice) {
            case 0:
                cout << "��̳�ѽ�������" << endl;
                return 0;

            case 1:
                // ��ʼ����̳
                if(forum_inited)
                {
                    cout << "��̳�Ѿ���ʼ������Ŷ~" << endl;
                    break;
                }
                if (initForum(F)) {
                    cout << "\n��ʼ���ɹ���\n";
                    forum_inited = 1;
                } else {
                    cout << "\n��ʼ��ʧ�ܣ�\n";
                }
                system("pause");
                break;

            case 2:
                // ����
                if (!forum_inited) {
                    cout << "\n��̳��δ��ʼ��TAT�����ȳ�ʼ��\n";
                } else {
                    // ��������
                    if(createPost(F) == OK)
                        cout << "�����ɹ���" << endl;
                    else
                        cout << "����ʧ�ܣ�" << endl;
                }
                system("pause");
                break;

            case 3:
                // �鿴����
                if (!forum_inited) {
                    cout << "\n��̳��δ��ʼ��TAT�����ȳ�ʼ��\n";
                } else {
                    if(showSecondMenu(F) == OK)break;
                }
                system("pause");
                break;
            case 4:
                // ��������
                if (!forum_inited) {
                    cout << "\n��̳��δ��ʼ��TAT�����ȳ�ʼ��\n";
                } else {
                    if(searchPost(F) == OK)break;
                }
                system("pause");
                break;
            case 5:
                // ͨ������idɾ������
                if (!forum_inited) {
                    cout << "\n��̳��δ��ʼ��TAT�����ȳ�ʼ��\n";
                } else {
                    if(deletePost(F) == OK)break;
                }
                system("pause");
                break;
            case 6:
                // ��̳����
                if (!forum_inited) {
                    cout << "\n��̳��δ��ʼ��TAT�����ȳ�ʼ��\n";
                } else {
                    if(forumSetting(F) == OK)break;
                }
                system("pause");
                break;
            default:
                cout << "�����ѡ����Ч�����������룡" << endl;
                break;
        }
    }

    return 0;
}
