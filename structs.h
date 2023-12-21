// structs.h

#ifndef ZHSJ_STRUCTS_H
#define ZHSJ_STRUCTS_H

#include <vector>
#include <iomanip>

// ���ۺͻظ�
struct Comment
{
    Comment(const std::string &comment_content, int now_id){
        content = comment_content;
        tme = time(nullptr);
        id = now_id;
        likes = 0;
    }

    //һ�����������ݡ��ظ���ʱ�����
    std::string content; // ��������
    std::vector<Comment> replies;    //���۵����� ʹ�ñ䳤����vector�������ڷ��������ڴ�ռ䣬����Ҫʱ��ʹ�á�
    time_t tme; // ����ʱ��
    int id; // ����Ψһid
    int likes; // ������

    // �ظ�����
    Status replyComment(const Comment& c){
        replies.push_back(c);
        return OK;
    }
    // ɾ���ظ�
    Status deleteReply(const int &reply_id) {
        for (auto it = replies.begin(); it != replies.end(); ++it) {
            if (it->id == reply_id) {
                replies.erase(it);
                return OK; // �ظ�ɾ���ɹ�
            }
        }

        // �ݹ�ɾ���ظ��Ļظ�
        for (auto &reply : replies) {
            if (reply.deleteReply(reply_id) == OK)
                return OK; // �ظ�ɾ���ɹ�
        }

        return ERROR; // δ�ҵ�ָ��id�Ļظ�����ظ�
    }
    // ��ʾ��������
    Status showDetailedComment(int depth = 0) {
        // ��ʾ���۵�������������Ȳ��ȷ��������
        std::cout << std::setw(4 * depth) << ""; // ���������������
        char time_str[100];
        strftime(time_str, 100, "%Y��%m��%d�� %X", localtime(&tme)); // ת��ʱ��
        std::cout << (depth == 0 ? "��ǰ����" : "�ظ�") << "ID��" << id << " | ���ݣ�" << content << " | ����ʱ�䣺" << time_str << " | ��������" << likes << std::endl;

        // �ݹ���ʾ�ظ�
        for (auto &reply : replies) {
            reply.showDetailedComment(depth + 1);
        }

        return OK;
    }
    Status likeComment() {
        ++likes;
        return OK;
    }
    Status deLikeComment() {
        if(likes <= 0){
            return ERROR;
        }
        --likes;
        return OK;
    }
};

// ����
struct Post
{
    int id; // ����id
    std::string title; // ���ӱ���
    std::string content;
    time_t tme;
    std::vector<Comment> comments;// �������
    int likes; // ������
    // ���캯�������ô˺�������һƪPost
    // �÷�ʾ����Post myPost("Title of Post", "Content of Post", now_post_id);
    Post(const std::string& postTitle, const std::string& postContent, int now_id)
    {
        // �ڹ��캯���жԽṹ���Ա���г�ʼ��
        title = postTitle;
        content = postContent;
        tme = time(nullptr); // ʹ�õ�ǰʱ���ʼ��time��Ա
        id = now_id;
        likes = 0;
    }

    // ��ʾ������Ϣ
    Status displayPost() const {
        char time_str[100];
        strftime(time_str, sizeof(time_str), "%Y��%m��%d�� %X", localtime(&tme));
        std::cout << "����id��" << id << std::endl;
        std::cout << "���ӱ���: " << title << std::endl;
        std::cout << "��������: " << content << std::endl;
        std::cout << "���ӵ�������" << likes << std::endl;
        std::cout << "���ӷ���ʱ��: " << time_str << std::endl;

        if (!comments.empty()) {
            for (Comment comment : comments) {
                std::cout << "----------------------\n";
                comment.showDetailedComment();
            }
        }
        return OK;
    }

    // ��������
    Status commentPost(Comment& c) {
        comments.push_back(c);
        return OK;
    }

    Status deleteComment(const int &comment_id) {
        for (auto it = comments.begin(); it != comments.end(); ++it) {
            if (it->id == comment_id) {
                comments.erase(it);
                return OK; // ����ɾ���ɹ�
            }
        }

        // �ݹ�ɾ���ظ�
        for (auto &comment : comments) {
            if (comment.deleteReply(comment_id) == OK)
                return OK; // �ظ�ɾ���ɹ�
        }

        return ERROR; // δ�ҵ�ָ��id�����ۼ���ظ�
    }
    Status likeComment() {
        ++likes;
        return OK;
    }
    Status deLikeComment() {
        if(likes <= 0){
            return ERROR;
        }
        --likes;
        return OK;
    }
};

// ��̳
struct Forum
{

    // ��̳����
    int post_num_per_page; // �ֱ����ÿҳ��ʾ��������������������ʾ��������Ȳ㼶
    bool search_post_content; // ����ʱ�Ƿ������������

    std::vector<Post> posts;
    std::string name;
    Forum(const std::string& forum_name, const int Post_num_per_page, bool Search_post_content)
    {
        post_num_per_page = Post_num_per_page;
        search_post_content = Search_post_content;
        name = forum_name;
    }
    Status deletePost(const int &post_id) {
        for (auto it = posts.begin(); it != posts.end(); ++it) {
            if (it->id == post_id) {
                posts.erase(it);
                return OK; // ����ɾ���ɹ�
            }
        }
        return ERROR; // δ�ҵ�ָ��id������
    }
    Status createPost(Post& p){
        posts.push_back(p);
        return OK;
    }
};

#endif //�ۺ����_STRUCTS_H
