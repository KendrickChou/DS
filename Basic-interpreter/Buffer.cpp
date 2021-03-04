#include <fstream>
#include <iostream>
#include "Buffer.h"

//TODO: your code here
//implement the functions in ListBuffer
Buffer::ListBuffer::ListBuffer() {
    head = new node;
    rear = new node;

    rear->Id = "999999999999";

    head->next = rear;
    rear->pre = head;
}

Buffer::ListBuffer::~ListBuffer(){
    deleteList();
}

//add after LinePos
void Buffer::ListBuffer::add(int Line,string s){
    node *N = new node;
    N->line = s;
    node *OriginNode = move(Line);
    N->pre = OriginNode;
    N->next = OriginNode->next;
    OriginNode->next->pre = N;
    OriginNode->next = N;
    ++NumofLine;
}

// "1"base
Buffer::node* Buffer::ListBuffer::move(int i){
    node *tar = head;
    int I = i;
    while(i > 0){
        tar = tar->next;
        --i;
    }
    return tar;
}

void Buffer::ListBuffer::deleteLine(int i){
    node *dtar = move(i);
    node *P,*N;
    P = dtar->pre;
    N = dtar->next;
    P->next = N;
    N->pre = P;
    delete dtar;
    --NumofLine;
}

void Buffer::ListBuffer::deleteList(){
    int totalLine = NumofLine;
    for(int i = 1;i <= totalLine; ++i){
        deleteLine(1);
    }
    delete head;
    delete rear;
}

string Buffer::ListBuffer::outputLine(){
    node *N = head;
    string output;
    for(int i = 0;i < NumofLine;++i){
        N = N->next;
        output += N->line;
        output += '\n';
    }
    return output;
}

Buffer::Buffer() {
    currentLineNum = 0;
    Text = new ListBuffer;
}

Buffer::~Buffer() {
    Text->deleteList();
}

void Buffer::writeToFile(const string &filename) const {
    std::fstream file;
    int Bytes = 0;
    file.open(filename,std::ios::out);
    int end = this->Text->NumofLine;
    for(int i = 1;i <= end;++i){
        string contain = this->moveToLine(i);
        Bytes += contain.length();
        file << contain;
        file << '\n';
        Bytes += 1;
    }
    file.close();
    std::cout << Bytes << " byte(s) written" << std::endl;
    return;
}

//void Buffer::showLines(int from, int to) const {
//    for(int i = from; i <= to; ++i){
//        Text->showLine(i);
//    }
//}

//void Buffer::deleteLines(int from, int to){
//    this->move(from);
//    for(int i = from; i <= to; ++i){
//        Text->deleteLine(from);
//        if(Text->NumofLine < currentLineNum){
//            currentLineNum =  Text->NumofLine;
//        }
//    }
//}

//void Buffer::insertLine(const string &text){
//    if(currentLineNum == 0){
//        this->Text->add(currentLineNum,text);
//        ++currentLineNum;
//    }
//    else{
//        --currentLineNum;
//        this->Text->add(currentLineNum,text);
//        ++currentLineNum;
//    }
//}

void Buffer::appendLine(const string &text){
     node *NewLine = new node (text);
     node *ClosestLine = move(NewLine->Id);

     if(ClosestLine->Id == NewLine->Id){
         ClosestLine->line = NewLine->line;
     }
     else{
         NewLine->pre = ClosestLine->pre;
         NewLine->next = ClosestLine;
         ClosestLine->pre->next = NewLine;
         ClosestLine->pre = NewLine;
     }

     ++currentLineNum;
     ++Text->NumofLine;

}

const string &Buffer::moveToLine(int idx) const {
    return this->Text->move(idx)->line;
}

//return a node, this node's Id is the closest to idx.(round up)
Buffer::node* Buffer::move(string idx){
    node *CurrentNode = Text->head;

    while(true){
        CurrentNode = CurrentNode->next;
        if(CurrentNode->Id >= idx){
            return CurrentNode;
        }
    }
}

void Buffer::deleteLine(std::string Id){
    node *dLine = move(Id);

    if(dLine->Id == Id){
        dLine->pre->next = dLine->next;
        dLine->next->pre = dLine->pre;
        delete dLine;
        --currentLineNum;
        --Text->NumofLine;
        return;
    }
}

void Buffer::clearLine()
{
    int totalLine = Text->NumofLine;
    for(int i = 1;i <= totalLine; ++i){
        Text->deleteLine(1);
        --currentLineNum;
    }
    Text->NumofLine = 0;
    currentLineNum = 0;
}

string Buffer::outputLine(){
    return Text->outputLine();
}
