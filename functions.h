// functions.h

#ifndef ZHSJ_FUNCTIONS_H
#define ZHSJ_FUNCTIONS_H

#include "global_definitions.h"
#include "structs.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <limits>

int now_post_id = 1;
int now_comment_id = 1;

// ��������

// ��ʼ����̳
Status initForum(Forum *F);

// ����
Status createPost(Forum *F);

// �����̳�к��е������б���Ҫ��������id�����ӱ��⡢����ʱ��
Status showPosts(Forum *F, int now_page);

// ��������idɾ������
Status deletePost(Forum *F);

//�������ӣ�����ؼ��ʣ��������к��йؼ��ʵ�����
Status searchPost(Forum *F);

//��̳���ã�����ÿҳ������ʾ������ÿҳ������ʾ����������ʱ�����������ݻ�����ұ���ȵ�������
Status forumSetting(Forum *F);

// ���˵�
Status showMenu(Forum *F, int &choice);

//�����˵�������ʾ�����б����ã���Ҫʵ�ַ����ϼ��˵�����ҳ��������ʾ�������ݡ�������ʱ����/�������С������ӱ����ֵ�����/���������⼸�����ܣ�
Status showSecondMenu(Forum *F);
//�����˵�������ʾĳһ�����Ӻ���ʾ����Ҫʵ�ַ�����һ���˵����������ӡ��������ӵ�ĳһ�����ۡ�������ʱ����/������ʾ���ۡ���������id�鿴ĳһ������֮�µ�����������Щ���ܣ�
Status showThirdMenu(Post *P, Forum *F);
//�ļ��˵�����ѡ��鿴ĳһ�����ۺ���ʾ����Ҫʵ�ַ�����һ���˵������۵�ǰ���ۣ����ݻظ�id���ۻظ���Щ���ܣ�
Status showFourthMenu(Comment *C, Forum *F);

// ��������

Status showMenu(Forum *F, int &choice) {
    system("cls");
    std::cout << F->name + "�������˵���\n";
    std::cout << "1.��ʼ����̳ 2.�������� 3.�鿴���� 4.�������� 5.ɾ������ 6.��̳���� 0.�˳���̳\n";
    std::cout << "\n������ѡ��Ĳ�����ţ�";
    std::cin >> choice;
    return OK;
}

// ��̳��ʼ��
Status initForum(Forum *F) {
    std::string forum_name;
    int post_num_per_page;
    std::cout << "��������̳���ƣ�";
    std::cin >> forum_name;
    std::cout << "������ÿҳ��ʾ����������";
    std::cin >> post_num_per_page;
    if (!std::cin) {
        return ERROR;
    }
    F->name = forum_name;
    F->post_num_per_page = post_num_per_page;
    return OK;
}


Status createPost(Forum *F) {
    std::string title, content;
    std::cout << "���������ӱ��⣺";
    std::cin >> title;
    std::cout << "\n��������������\n";
    std::cin >> content;
    Post p(title, content, now_post_id);
    now_post_id++; // ����id�Զ�+1
    F->createPost(p);
    return OK;
}

// �鿴��now_pageҳ������
Status showPosts(Forum *F, int now_page) {
    int total_page = F->posts.size() / F->post_num_per_page + (F->posts.size() % F->post_num_per_page ? 1 : 0);
    std::cout << "\n�����б�\n";
    for (int i = (now_page - 1) * F->post_num_per_page;i < std::min(now_page * F->post_num_per_page, int(F->posts.size()));i++) {
        Post post = F->posts[i];
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%Y��%m��%d�� %X", localtime(&post.tme));
        std::cout << "����ID��" << post.id << " | ���⣺" << post.title << " | ��������" << post.likes << " | ����ʱ�䣺" << time_str << std::endl;
    }
    std::cout << "��" << now_page << "ҳ / " << "��" << total_page << "ҳ" << std::endl;
    return OK;
}

Status deletePost(Forum *F) {
    int post_id;
    std::cout << "\n������Ҫɾ��������ID��";
    std::cin >> post_id;
    if (F->deletePost(post_id)) {
        std::cout << "����ɾ���ɹ���\n";
    } else {
        std::cout << "δ�ҵ�ָ��ID�����ӣ�ɾ��ʧ�ܡ�\n";
    }

    return OK;
}

Status searchPost(Forum *F) {
    std::string keyword;
    std::cout << "������ؼ��ʣ�";
    std::cin >> keyword;

    std::cout << "���������";
    for (const auto &post: F->posts) {
        // ������̳���þ����Ƿ������������
        if (F->search_post_content) {
            if (post.title.find(keyword) != std::string::npos || post.content.find(keyword) != std::string::npos) {
                char time_str[100];
                strftime(time_str, sizeof(time_str), "%Y��%m��%d�� %X", localtime(&post.tme));
                std::cout << "����ID��" << post.id << " | ���⣺" << post.title << " | ����ʱ�䣺" << time_str
                          << std::endl;
            }
        } else {
            if (post.title.find(keyword) != std::string::npos) {
                char time_str[100];
                strftime(time_str, sizeof(time_str), "%Y��%m��%d�� %X", localtime(&post.tme));
                std::cout << "����ID��" << post.id << " | ���⣺" << post.title << " | ����ʱ�䣺" << time_str
                          << std::endl;
            }
        }
    }

    return OK;
}

Status forumSetting(Forum *F) {
    while(true) {
        system("cls");

        std::cout << "\n��̳���ã�\n";
        std::cout << "1. ÿҳ������ʾ����\n";
        std::cout << "2. ����ʱ�Ƿ������������\n";
        std::cout << "3. ��̳������\n";
        std::cout << "0. �����ϼ��˵�\n";

        int setting_choice;
        std::cout << "������ѡ����������ţ�";
        std::cin >> setting_choice;

        switch (setting_choice) {
            case 1:
                std::cout << "������ÿҳ������ʾ������";
                int post_num_per_page;
                std::cin >> post_num_per_page;
                if (!std::cin)return ERROR;
                F->post_num_per_page = post_num_per_page;
                std::cout << "�����Ѽ�¼��" << std::endl;
                break;
            case 2:
                std::cout << "�Ƿ�����������ݣ�(1:��, 0:��)��";
                bool search_post_content;
                std::cin >> search_post_content;
                if (!std::cin)return ERROR;
                F->search_post_content = search_post_content;
                std::cout << "�����Ѽ�¼��" << std::endl;
                break;
            case 3:
                std::cout << "��̳ԭ����" << F->name << std::endl;
                std::cout << "�����������ƣ�";
                std::cin >> F->name;
                break;
            case 0:
                return OK;
            default:
                break;
        }
        system("pause");
    }
}

Status showSecondMenu(Forum *F) {
    system("cls");

    int second_choice;
    int now_page = 1;
    while (true) {
        showPosts(F, now_page);
        std::cout << "\n�����˵���\n";
        std::cout << "1. �����ϼ��˵�\n";
        std::cout << "2. ��ҳ\n";
        std::cout << "3. �鿴/�������Ӿ�������\n";
        std::cout << "4. ������ʱ����������\n";
        std::cout << "5. ������ʱ�併������\n";
        std::cout << "6. �����ӱ����ֵ�����������\n";
        std::cout << "7. �����ӱ����ֵ���������\n";
        std::cout << "8. �����ӵ�������������\n";
        std::cout << "9. �����ӵ�������������\n";
        std::cout << "������ѡ��Ĳ�����ţ�";
        std::cin >> second_choice;

        switch (second_choice) {
            case 1:
                return OK;
            case 2:
                int choice;
                int total_page;
                total_page = F->posts.size() / F->post_num_per_page + (F->posts.size() % F->post_num_per_page ? 1 : 0);

                std::cout << "������������\n";
                std::cout << "1. ��һҳ\n";
                std::cout << "2. ��һҳ\n";
                std::cout << "3. ��ת��ָ��ҳ\n";
                std::cout << "����. �����ϼ��˵�\n";
                std::cin >> choice;
                switch (choice) {
                    case 1:
                        if (now_page - 1 <= 0) {
                            std::cout << "�Ѿ�����ҳ����" << std::endl;
                            break;
                        }
                        --now_page;
                        break;
                    case 2:
                        if (now_page + 1 > total_page) {
                            std::cout << "�Ѿ���βҳ����" << std::endl;
                            break;
                        }
                        ++now_page;
                        break;
                    case 3:
                        std::cout << "��������תҳ����";
                        int to_page;
                        while (true) {
                            std::cin >> to_page;
                            if (to_page < 1 || to_page > total_page) {
                                std::cout << "�����ҳ�벻�������������룡" << std::endl;
                            } else {
                                now_page = to_page;
                                showPosts(F, to_page);
                                break;
                            }
                        }
                        break;
                    default:
                        break;
                }
                break;
            case 3:
                int post_id;
                bool flag; // �Ƿ�ɹ��ҵ�����
                flag = false;
                std::cout << "��������ϣ���鿴������id��";
                std::cin >> post_id;
                // ����id��������
                for (auto &post: F->posts) {
                    if (post.id == post_id) {
                        flag = true;
                        if (showThirdMenu(&post, F) == OK)
                            break; // ���ӳɹ����
                        break;
                    }
                }
                if (!flag)
                    std::cout << "δ�ҵ�ָ�����ӵ�id" << std::endl;
                break;
            case 4:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.tme < b.tme; });
                std::cout << "�����Ѱ�����ʱ���������С�\n";
                break;
            case 5:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.tme > b.tme; });
                std::cout << "�����Ѱ�����ʱ�併�����С�\n";
                break;
            case 6:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.title < b.title; });
                std::cout << "�����Ѱ����ӱ����ֵ����������С�\n";
                break;
            case 7:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.title > b.title; });
                std::cout << "�����Ѱ����ӱ����ֵ��������С�\n";
                break;
            case 8:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.likes < b.likes; });
                std::cout << "�����Ѱ��������������С�\n";
                break;
            case 9:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.likes > b.likes; });
                std::cout << "�����Ѱ��������������С�\n";
                break;
            default:
                std::cout << "��Ч��ѡ�����������롣\n";
                break;
        }
        system("pause");

    }
}

Status showThirdMenu(Post *P, Forum *F) {
    system("cls");

    int third_choice;
    while (true) {
        P->displayPost();
        std::cout << "\n���Ӳ����˵���\n";
        std::cout << "1. ������һ���˵�\n";
        std::cout << "2. ��������\n";
        std::cout << "3. �鿴/�ظ����ӵ�ĳһ������\n";
        std::cout << "4. ������ʱ��������ʾ����\n";
        std::cout << "5. ������ʱ�併����ʾ����\n";
        std::cout << "6. ��������������ʾ����\n";
        std::cout << "7. ��������������ʾ����\n";
        std::cout << "8. ɾ�������µ�����\n";
        std::cout << "9. ���޸�����\n";
        std::cout << "10. ȡ�����޸�����\n";
        std::cout << "11. �༭����\n";

        std::cout << "������ѡ��Ĳ�����ţ�";
        std::cin >> third_choice;

        switch (third_choice) {
            case 1:
                return OK;
            case 2: {
                Comment c("", now_comment_id++);
                std::cout << "�������������ݣ�";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, c.content);
                P->commentPost(c);
                std::cout << "���۳ɹ���\n";
                break;
            }
            case 3:
                if (P->comments.empty()) {
                    std::cout << "������������\n";
                } else {
                    int comment_id;
                    std::cout << "��������ϣ���ظ�������id��";
                    std::cin >> comment_id;

                    bool found = false;
                    for (auto &comment : P->comments) {
                        if (comment.id == comment_id) {
                            found = true;
                            if (showFourthMenu(&comment, F) == OK)
                                break;
                            break;
                        }
                    }

                    if (!found)
                        std::cout << "δ�ҵ�ָ�����۵�id" << std::endl;
                }
                break;
            case 4:
                // ������ʱ��������ʾ���۹��ܵ�ʵ��
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.tme < b.tme; });
                std::cout << "�����Ѱ�ʱ���������С�\n";
                break;
            case 5:
                // ������ʱ�併����ʾ���۹��ܵ�ʵ��
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.tme > b.tme; });
                std::cout << "�����Ѱ�ʱ�併�����С�\n";
                break;
            case 6:
                // ������ʱ�併����ʾ���۹��ܵ�ʵ��
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.likes < b.likes; });
                std::cout << "�����Ѱ��������������С�\n";
                break;
            case 7:
                // ������ʱ�併����ʾ���۹��ܵ�ʵ��
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.likes > b.likes; });
                std::cout << "�����Ѱ��������������С�\n";
                break;
            case 8:
                if (P->comments.empty()) {
                    std::cout << "������������\n";
                } else {
                    int comment_id;
                    std::cout << "��������ϣ��ɾ��������id��";
                    std::cin >> comment_id;

                    if (P->deleteComment(comment_id) == OK) {
                        std::cout << "����ɾ���ɹ���\n";
                    } else {
                        std::cout << "δ�ҵ�ָ�����۵�id��ɾ��ʧ�ܡ�\n";
                    }
                }
                break;
            case 9:
                if(P->likeComment()){
                    std::cout << "���޳ɹ���" << std::endl;
                }
                else{
                    std::cout << "����ʧ�ܡ���" << std::endl;
                }
                break;
            case 10:
                if(P->deLikeComment()){
                    std::cout << "ȡ�����޳ɹ���" << std::endl;
                }
                else{
                    std::cout << "ȡ������ʧ�ܣ���������Ϊ��û�е�����~" << std::endl;
                }
                break;
            case 11:
                // �༭����
                std::cout << "����༭���ӵģ�" << std::endl;
                std::cout << "1. ����" << std::endl;
                std::cout << "2. ����" << std::endl;
                int choice;
                while(true)
                {
                    std::cin >> choice;
                    bool chose_successfully = false;
                    switch (choice) {
                        case 1:
                            std::cout << "����ԭ����Ϊ��" << P->title << std::endl;
                            std::cout << "���������ӵ��±��⣺" << std::endl;
                            std::cin >> P->title;
                            std::cout << "�޸ĳɹ���" << std::endl;
                            chose_successfully = true;
                            break;
                        case 2:
                            std::cout << "����ԭ����Ϊ��" << P->content << std::endl;
                            std::cout << "���������ӵ������ݣ�" << std::endl;
                            std::cin >> P->content;
                            std::cout << "�޸ĳɹ���" << std::endl;
                            chose_successfully = true;
                            break;
                        default:
                            std::cout << "��Ч��ѡ����������룡" << std::endl;
                            break;
                    }
                    if(chose_successfully)break;
                }
                break;

            default:
                std::cout << "��Ч��ѡ�����������롣\n";
                break;
        }
        system("pause");
    }
}

Status showFourthMenu(Comment *C, Forum *F) {
    int fourth_choice;
    while (true) {
        system("cls");
        C->showDetailedComment();
        std::cout << "\n���۲����˵���\n";
        std::cout << "1. ������һ���˵�\n";
        std::cout << "2. �ظ���ǰ����\n";
        std::cout << "3. �ظ���ǰ�����µ���������\n";
        std::cout << "4. ɾ�������µĻظ�\n";
        std::cout << "5. ���޸�����\n";
        std::cout << "6. ȡ�����޸�����\n";
        std::cout << "������ѡ��Ĳ�����ţ�";
        std::cin >> fourth_choice;

        switch (fourth_choice) {
            case 1:
                return OK;
            case 2:{
                // ���۵�ǰ���۹��ܵ�ʵ��
                Comment reply("", now_comment_id++);
                std::cout << "������ظ����ݣ�";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, reply.content);
                C->replyComment(reply);
                std::cout << "�ظ��ɹ���\n";
                break;}
            case 3:
                // ���ݻظ�id���ۻظ����ܵ�ʵ��
                if (C->replies.empty()) {
                    std::cout << "�������޻ظ�\n";
                } else {
                    int reply_id;
                    std::cout << "��������ϣ���鿴�Ļظ�id��";
                    std::cin >> reply_id;

                    bool found = false;
                    for (auto &reply : C->replies) {
                        if (reply.id == reply_id) {
                            found = true;
                            showFourthMenu(&reply, F);
                            break;
                        }
                    }

                    if (!found)
                        std::cout << "δ�ҵ�ָ���ظ���id" << std::endl;
                }
                break;
            case 4:
                if (C->replies.empty()) {
                    std::cout << "�������޻ظ�\n";
                } else {
                    int reply_id;
                    std::cout << "��������ϣ��ɾ���Ļظ�id��";
                    std::cin >> reply_id;

                    if (C->deleteReply(reply_id) == OK) {
                        std::cout << "�ظ�ɾ���ɹ���\n";
                    } else {
                        std::cout << "δ�ҵ�ָ���ظ���id��ɾ��ʧ�ܡ�\n";
                    }
                }
                break;
            case 5:
                if(C->likeComment()){
                    std::cout << "���޳ɹ���" << std::endl;
                }
                else{
                    std::cout << "����ʧ�ܡ���" << std::endl;
                }
                break;
            case 6:
                if(C->deLikeComment()){
                    std::cout << "ȡ�����޳ɹ���" << std::endl;
                }
                else {
                    std::cout << "ȡ������ʧ�ܣ���������Ϊ��û�е�����~" << std::endl;
                }
                break;
            default:
                std::cout << "��Ч��ѡ�����������롣\n";
                break;
        }
        system("pause");
    }
}

#endif //�ۺ����_FUNCTIONS_H
