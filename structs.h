// structs.h

#ifndef ZHSJ_STRUCTS_H
#define ZHSJ_STRUCTS_H

#include <vector>
#include <iomanip>

// 评论和回复
struct Comment
{
    Comment(const std::string &comment_content, int now_id){
        content = comment_content;
        tme = time(nullptr);
        id = now_id;
    }

    //一个评论由内容、回复和时间组成
    std::string content; // 评论内容
    std::vector<Comment> replies;    //评论的评论 使用变长数组vector可以免于分配过多的内存空间，在需要时再使用。
    time_t tme; // 评论时间
    int id; // 评论唯一id

    // 回复评论
    Status replyComment(const Comment& c){
        replies.push_back(c);
        return OK;
    }
    // 删除回复
    Status deleteReply(const int &reply_id) {
        for (auto it = replies.begin(); it != replies.end(); ++it) {
            if (it->id == reply_id) {
                replies.erase(it);
                return OK; // 回复删除成功
            }
        }

        // 递归删除回复的回复
        for (auto &reply : replies) {
            if (reply.deleteReply(reply_id) == OK)
                return OK; // 回复删除成功
        }

        return ERROR; // 未找到指定id的回复及其回复
    }
    // 显示评论详情
    Status showDetailedComment(int depth = 0) {
        // 显示评论的缩进，根据深度层次确定缩进量
        std::cout << std::setw(4 * depth) << "";
        char time_str[100];
        strftime(time_str, 100, "%Y年%m月%d日 %X", localtime(&tme));
        std::cout << (depth == 0 ? "当前评论" : "回复") << "ID：" << id << " | 内容：" << content << " | 评论时间：" << time_str << std::endl;

        // 递归显示回复
        for (auto &reply : replies) {
            reply.showDetailedComment(depth + 1);
        }

        return OK;
    }
};

// 帖子
struct Post
{
    int id; // 帖子id
    std::string title; // 帖子标题
    std::string content;
    time_t tme;
    std::vector<Comment> comments;// 多个评论
    // 构造函数，利用此函数生成一篇Post
    // 用法示例：Post myPost("Title of Post", "Content of Post", now_post_id);
    Post(const std::string& postTitle, const std::string& postContent, int now_id)
    {
        // 在构造函数中对结构体成员进行初始化
        title = postTitle;
        content = postContent;
        tme = time(nullptr); // 使用当前时间初始化time成员
        id = now_id;
    }

    // 显示帖子信息
    Status displayPost() const {
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%Y年%m月%d日 %X", localtime(&tme));
        std::cout << "帖子id：" << id << std::endl;
        std::cout << "帖子标题: " << title << std::endl;
        std::cout << "帖子内容: " << content << std::endl;
        std::cout << "帖子发布时间: " << time_str << std::endl;

        if (!comments.empty()) {
            for (Comment comment : comments) {
                std::cout << "----------------------\n";
                comment.showDetailedComment();
            }
        }
        return OK;
    }

    // 评论帖子
    Status commentPost(Comment& c) {
        comments.push_back(c);
        return OK;
    }

    Status deleteComment(const int &comment_id) {
        for (auto it = comments.begin(); it != comments.end(); ++it) {
            if (it->id == comment_id) {
                comments.erase(it);
                return OK; // 评论删除成功
            }
        }

        // 递归删除回复
        for (auto &comment : comments) {
            if (comment.deleteReply(comment_id) == OK)
                return OK; // 回复删除成功
        }

        return ERROR; // 未找到指定id的评论及其回复
    }
};

// 论坛
struct Forum
{

    // 论坛设置
    int post_num_per_page; // 分别代表每页显示的帖子数、评论数和显示的评论深度层级
    bool search_post_content; // 查找时是否查找帖子内容

    std::vector<Post> posts;
    std::string name;
    Forum(const std::string& forum_name, const int Post_num_per_page, const int Comment_num_per_page, bool Search_post_content)
    {
        post_num_per_page = Post_num_per_page;
        search_post_content = Search_post_content;
        name = forum_name;
    }
    Status deletePost(const int &post_id) {
        for (auto it = posts.begin(); it != posts.end(); ++it) {
            if (it->id == post_id) {
                posts.erase(it);
                return OK; // 帖子删除成功
            }
        }
        return ERROR; // 未找到指定id的帖子
    }
    Status createPost(Post& p){
        posts.push_back(p);
        return OK;
    }
};

#endif //综合设计_STRUCTS_H
