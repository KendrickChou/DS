#pragma once

#include <string>

using std::string;

class Buffer {
private:
    int currentLineNum;
    // TODO: add a List here for storing the input lines
    struct node{
        string Id;
        string line;
        node *pre = nullptr,*next = nullptr;

        node(){};

        node(const string &text){
            int i = 0;
            while(text[i] != ' '){
                Id += text[i];
                ++i;
            }

            line = text;
        }
    };
    class ListBuffer{
    public:
        node *head;
        node *rear;
        int NumofLine = 0;
        ListBuffer();
        ~ListBuffer();
        void add(int Line,string s);
        node* move(int i);
        void deleteLine(int i);
        void deleteList();
        string outputLine();
    };

public:
    ListBuffer *Text;
    Buffer();
    ~Buffer();

    void writeToFile(const string &filename) const;

    const string &moveToLine(int idx) const;

    node* move(string idx);


    void appendLine(const string &text);
    void deleteLine(string Id);
    void clearLine();
    string outputLine();


    //    void deleteLines(int from, int to);
    //    void insertLine(const string &text);
    //    void showLines(int from, int to) const;

};
