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

// 函数声明

// 初始化论坛
Status initForum(Forum *F);

// 发帖
Status createPost(Forum *F);

// 输出论坛中含有的帖子列表，需要包含帖子id、帖子标题、发帖时间
Status showPosts(Forum *F, int now_page);

// 根据帖子id删除帖子
Status deletePost(Forum *F);

//查找帖子：输入关键词，检索所有含有关键词的帖子
Status searchPost(Forum *F);

//论坛设置，包括每页帖子显示数量，每页评论显示数量，查找时查找帖子内容或仅查找标题等等设置项
Status forumSetting(Forum *F);

// 主菜单
Status showMenu(Forum *F, int &choice);

//二级菜单（在显示帖子列表后调用，需要实现返回上级菜单、翻页、根据显示帖子内容、按发帖时间升/降序排列、按帖子标题字典序升/降序排列这几个功能）
Status showSecondMenu(Forum *F);
//三级菜单（在显示某一个帖子后显示，需要实现返回上一级菜单、评论帖子、评论帖子的某一个评论、按评论时间升/降序显示评论、根据评论id查看某一个评论之下的所有评论这些功能）
Status showThirdMenu(Post *P, Forum *F);
//四级菜单（在选择查看某一个评论后显示，需要实现返回上一级菜单，评论当前评论，根据回复id评论回复这些功能）
Status showFourthMenu(Comment *C, Forum *F);

// 函数定义

Status showMenu(Forum *F, int &choice) {
    system("cls");
    std::cout << F->name + "操作主菜单：\n";
    std::cout << "1.初始化论坛 2.创建帖子 3.查看帖子 4.查找帖子 5.删除帖子 6.论坛设置 0.退出论坛\n";
    std::cout << "\n请输入选择的操作编号：";
    std::cin >> choice;
    return OK;
}

// 论坛初始化
Status initForum(Forum *F) {
    std::string forum_name;
    int post_num_per_page;
    std::cout << "请输入论坛名称：";
    std::cin >> forum_name;
    std::cout << "请输入每页显示的帖子数：";
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
    std::cout << "请输入帖子标题：";
    std::cin >> title;
    std::cout << "\n请输入帖子内容\n";
    std::cin >> content;
    Post p(title, content, now_post_id);
    now_post_id++; // 帖子id自动+1
    F->createPost(p);
    return OK;
}

// 查看第now_page页的帖子
Status showPosts(Forum *F, int now_page) {
    int total_page = F->posts.size() / F->post_num_per_page + (F->posts.size() % F->post_num_per_page ? 1 : 0);
    std::cout << "\n帖子列表：\n";
    for (int i = (now_page - 1) * F->post_num_per_page;i < std::min(now_page * F->post_num_per_page, int(F->posts.size()));i++) {
        Post post = F->posts[i];
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%Y年%m月%d日 %X", localtime(&post.tme));
        std::cout << "帖子ID：" << post.id << " | 标题：" << post.title << " | 发帖时间：" << time_str << std::endl;
    }
    std::cout << "第" << now_page << "页 / " << "共" << total_page << "页" << std::endl;
    return OK;
}

Status deletePost(Forum *F) {
    int post_id;
    std::cout << "\n请输入要删除的帖子ID：";
    std::cin >> post_id;
    if (F->deletePost(post_id)) {
        std::cout << "帖子删除成功！\n";
    } else {
        std::cout << "未找到指定ID的帖子，删除失败。\n";
    }

    return OK;
}

Status searchPost(Forum *F) {
    std::string keyword;
    std::cout << "\n请输入关键词：";
    std::cin >> keyword;

    std::cout << "\n搜索结果：\n";
    for (const auto &post: F->posts) {
        // 根据论坛设置决定是否查找帖子内容
        if (F->search_post_content) {
            if (post.title.find(keyword) != std::string::npos || post.content.find(keyword) != std::string::npos) {
                char time_str[100];
                strftime(time_str, sizeof(time_str), "%Y年%m月%d日 %X", localtime(&post.tme));
                std::cout << "帖子ID：" << post.id << " | 标题：" << post.title << " | 发帖时间：" << time_str
                          << std::endl;
            }
        } else {
            if (post.title.find(keyword) != std::string::npos) {
                char time_str[100];
                strftime(time_str, sizeof(time_str), "%Y年%m月%d日 %X", localtime(&post.tme));
                std::cout << "帖子ID：" << post.id << " | 标题：" << post.title << " | 发帖时间：" << time_str
                          << std::endl;
            }
        }
    }

    return OK;
}

Status forumSetting(Forum *F) {
    system("cls");

    std::cout << "\n论坛设置：\n";
    std::cout << "1. 每页帖子显示数量\n";
    std::cout << "2. 查找时是否查找帖子内容\n";
    std::cout << "0. 返回上级菜单\n";

    int setting_choice;
    std::cout << "请输入选择的设置项编号：";
    std::cin >> setting_choice;

    switch (setting_choice) {
        case 1:
            std::cout << "请输入每页帖子显示数量：";
            std::cin >> F->post_num_per_page;
            break;
        case 2:
            std::cout << "是否查找帖子内容？(1:是, 0:否)：";
            std::cin >> F->search_post_content;
            break;
        case 0:
            return OK;
        default:
            break;
    }

    std::cout << "设置成功！\n";
    return OK;
}

Status showSecondMenu(Forum *F) {
    system("cls");

    int second_choice;
    int now_page = 1;
    while (true) {
        showPosts(F, now_page);
        std::cout << "\n看帖菜单：\n";
        std::cout << "1. 返回上级菜单\n";
        std::cout << "2. 翻页\n";
        std::cout << "3. 查看/评论帖子具体内容\n";
        std::cout << "4. 按发帖时间升序排列\n";
        std::cout << "5. 按发帖时间降序排列\n";
        std::cout << "6. 按帖子标题字典序升序排列\n";
        std::cout << "7. 按帖子标题字典序降序排列\n";
        std::cout << "8. 按帖子点赞数升序排列\n";
        std::cout << "9. 按帖子点赞数降序排列\n";
        std::cout << "请输入选择的操作编号：";
        std::cin >> second_choice;

        switch (second_choice) {
            case 1:
                return OK;
            case 2:
                int choice;
                int total_page;
                total_page = F->posts.size() / F->post_num_per_page + (F->posts.size() % F->post_num_per_page ? 1 : 0);

                std::cout << "请输入操作编号\n";
                std::cout << "1. 上一页\n";
                std::cout << "2. 下一页\n";
                std::cout << "3. 跳转至指定页\n";
                std::cout << "其他. 返回上级菜单\n";
                std::cin >> choice;
                switch (choice) {
                    case 1:
                        if (now_page - 1 <= 0) {
                            std::cout << "已经是首页啦！" << std::endl;
                            break;
                        }
                        --now_page;
                        break;
                    case 2:
                        if (now_page + 1 > total_page) {
                            std::cout << "已经是尾页啦！" << std::endl;
                            break;
                        }
                        ++now_page;
                        break;
                    case 3:
                        std::cout << "请输入跳转页数：";
                        int to_page;
                        while (true) {
                            std::cin >> to_page;
                            if (to_page < 1 || to_page > total_page) {
                                std::cout << "输入的页码不合理，请重新输入！" << std::endl;
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
                bool flag; // 是否成功找到帖子
                flag = false;
                std::cout << "请输入您希望查看的帖子id：";
                std::cin >> post_id;
                // 根据id查找帖子
                for (auto &post: F->posts) {
                    if (post.id == post_id) {
                        flag = true;
                        if (showThirdMenu(&post, F) == OK)
                            break; // 帖子成功输出
                        break;
                    }
                }
                if (!flag)
                    std::cout << "未找到指定帖子的id" << std::endl;
                break;
            case 4:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.tme < b.tme; });
                std::cout << "帖子已按发帖时间升序排列。\n";
                break;
            case 5:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.tme > b.tme; });
                std::cout << "帖子已按发帖时间降序排列。\n";
                break;
            case 6:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.title < b.title; });
                std::cout << "帖子已按帖子标题字典序升序排列。\n";
                break;
            case 7:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.title > b.title; });
                std::cout << "帖子已按帖子标题字典序降序排列。\n";
                break;
            case 8:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.likes < b.likes; });
                std::cout << "帖子已按帖子标题字典序降序排列。\n";
                break;
            case 9:
                std::sort(F->posts.begin(), F->posts.end(),
                          [](const Post &a, const Post &b) { return a.likes > b.likes; });
                std::cout << "帖子已按帖子标题字典序降序排列。\n";
                break;
            default:
                std::cout << "无效的选择，请重新输入。\n";
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
        std::cout << "\n帖子操作菜单：\n";
        std::cout << "1. 返回上一级菜单\n";
        std::cout << "2. 评论帖子\n";
        std::cout << "3. 查看/回复帖子的某一个评论\n";
        std::cout << "4. 按评论时间升序显示评论\n";
        std::cout << "5. 按评论时间降序显示评论\n";
        std::cout << "6. 按评论时间降序显示评论\n";
        std::cout << "7. 按评论时间降序显示评论\n";
        std::cout << "8. 删除帖子下的评论\n";
        std::cout << "9. 点赞该帖子\n";
        std::cout << "10. 取消点赞该帖子\n";
        std::cout << "11. 编辑帖子\n";

        std::cout << "请输入选择的操作编号：";
        std::cin >> third_choice;

        switch (third_choice) {
            case 1:
                return OK;
            case 2: {
                Comment c("", now_comment_id++);
                std::cout << "请输入评论内容：";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, c.content);
                P->commentPost(c);
                std::cout << "评论成功！\n";
                break;
            }
            case 3:
                if (P->comments.empty()) {
                    std::cout << "该帖子无评论\n";
                } else {
                    int comment_id;
                    std::cout << "请输入您希望回复的评论id：";
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
                        std::cout << "未找到指定评论的id" << std::endl;
                }
                break;
            case 4:
                // 按评论时间升序显示评论功能的实现
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.tme < b.tme; });
                std::cout << "评论已按时间升序排列。\n";
                break;
            case 5:
                // 按评论时间降序显示评论功能的实现
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.tme > b.tme; });
                std::cout << "评论已按时间降序排列。\n";
                break;
            case 6:
                // 按评论时间降序显示评论功能的实现
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.likes < b.likes; });
                std::cout << "评论已按点赞数升序排列。\n";
                break;
            case 7:
                // 按评论时间降序显示评论功能的实现
                std::sort(P->comments.begin(), P->comments.end(),
                          [](const Comment &a, const Comment &b) { return a.likes > b.likes; });
                std::cout << "评论已按点赞数降序排列。\n";
                break;
            case 8:
                if (P->comments.empty()) {
                    std::cout << "该帖子无评论\n";
                } else {
                    int comment_id;
                    std::cout << "请输入您希望删除的评论id：";
                    std::cin >> comment_id;

                    if (P->deleteComment(comment_id) == OK) {
                        std::cout << "评论删除成功！\n";
                    } else {
                        std::cout << "未找到指定评论的id，删除失败。\n";
                    }
                }
                break;
            case 9:
                if(P->likeComment()){
                    std::cout << "点赞成功！" << std::endl;
                }
                else{
                    std::cout << "点赞失败……" << std::endl;
                }
                break;
            case 10:
                if(P->deLikeComment()){
                    std::cout << "取消点赞成功！" << std::endl;
                }
                else{
                    std::cout << "取消点赞失败，可能是因为还没有点赞呢~" << std::endl;
                }
                break;
            case 11:
                // 编辑帖子
                std::cout << "您想编辑帖子的？" << std::endl;
                std::cout << "1. 标题" << std::endl;
                std::cout << "2. 内容" << std::endl;
                int choice;
                while(true)
                {
                    std::cin >> choice;
                    bool chose_successfully = false;
                    switch (choice) {
                        case 1:
                            std::cout << "帖子原标题为：" << P->title << std::endl;
                            std::cout << "请输入帖子的新标题：" << std::endl;
                            std::cin >> P->title;
                            std::cout << "修改成功！" << std::endl;
                            chose_successfully = true;
                            break;
                        case 2:
                            std::cout << "帖子原内容为：" << P->content << std::endl;
                            std::cout << "请输入帖子的新内容：" << std::endl;
                            std::cin >> P->content;
                            std::cout << "修改成功！" << std::endl;
                            chose_successfully = true;
                            break;
                        default:
                            std::cout << "无效的选项，请重新输入！" << std::endl;
                            break;
                    }
                    if(chose_successfully)break;
                }

            default:
                std::cout << "无效的选择，请重新输入。\n";
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
        std::cout << "\n评论操作菜单：\n";
        std::cout << "1. 返回上一级菜单\n";
        std::cout << "2. 回复当前评论\n";
        std::cout << "3. 回复当前评论下的其他评论\n";
        std::cout << "4. 删除评论下的回复\n";
        std::cout << "5. 点赞该评论\n";
        std::cout << "6. 取消点赞该评论\n";
        std::cout << "请输入选择的操作编号：";
        std::cin >> fourth_choice;

        switch (fourth_choice) {
            case 1:
                return OK;
            case 2:{
                // 评论当前评论功能的实现
                Comment reply("", now_comment_id++);
                std::cout << "请输入回复内容：";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::getline(std::cin, reply.content);
                C->replyComment(reply);
                std::cout << "回复成功！\n";
                break;}
            case 3:
                // 根据回复id评论回复功能的实现
                if (C->replies.empty()) {
                    std::cout << "该评论无回复\n";
                } else {
                    int reply_id;
                    std::cout << "请输入您希望查看的回复id：";
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
                        std::cout << "未找到指定回复的id" << std::endl;
                }
                break;
            case 4:
                if (C->replies.empty()) {
                    std::cout << "该评论无回复\n";
                } else {
                    int reply_id;
                    std::cout << "请输入您希望删除的回复id：";
                    std::cin >> reply_id;

                    if (C->deleteReply(reply_id) == OK) {
                        std::cout << "回复删除成功！\n";
                    } else {
                        std::cout << "未找到指定回复的id，删除失败。\n";
                    }
                }
                break;
            case 5:
                if(C->likeComment()){
                    std::cout << "点赞成功！" << std::endl;
                }
                else{
                    std::cout << "点赞失败……" << std::endl;
                }
                break;
            case 6:
                if(C->deLikeComment()){
                    std::cout << "取消点赞成功！" << std::endl;
                }
                else{
                    std::cout << "取消点赞失败，可能是因为还没有点赞呢~" << std::endl;
                }
            default:
                std::cout << "无效的选择，请重新输入。\n";
                break;
        }
        system("pause");
    }
}

#endif //综合设计_FUNCTIONS_H
