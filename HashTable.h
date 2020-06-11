#ifndef HASHTESTUNIT_CHAINHASHING_H
#define HASHTESTUNIT_CHAINHASHING_H
#define INITIAL_SIZE 11

#include <stdio.h>

template<class T>
class NodeHash{
    T data_;
    int id_;
    NodeHash *next_;
public:
    NodeHash(T data,int id) : data_(data),id_(id), next_(NULL){}
    void setData(T& data){
        this->data_=data;
    }
    void setId(int id){
        this->id_=id;
    }
    NodeHash* getNext(){
        return this->next_;
    }
    T& getData(){
        return this->data_;
    }
    void setNext(NodeHash *next){
        this->next_=next;
    }
    int getId(){
        return this->id_;
    }

};

template <class T>
class HashTable{
    NodeHash<T> ** arr_;
    int size_;
    int capacity_;
    int hash(int id){
        return id % capacity_;
    }



    void startErase(NodeHash<T> **arr) {
        for (int i = 0; i < capacity_; i++) {
            NodeHash<T> *curr = arr[i];
            while (curr) {
                NodeHash<T> *prev = curr;
                curr = curr->getNext();
                delete prev;
            }
            arr[i] = NULL;
        }
    }

    void insert2new(NodeHash<T> **target, bool del) {
        int top = del ? capacity_ * 2 : capacity_ / 2;
        for (int i = 0; i < top; i++) {
            NodeHash<T> *curr = arr_[i];
            while (curr) {
                insertToArr(curr->getData(), curr->getId(), target);
                curr = curr->getNext();
            }
        }
    }

    void insertToArr(T data, int id, NodeHash<T> **arr) {
        int i = hash(id);
        if (!arr[i]) {
            arr[i] = new NodeHash<T>(data, id);
            return;
        }
        NodeHash<T> *curr = arr[i];
        while (curr->getNext()) {
            curr = curr->getNext();
        }
        curr->setNext(new NodeHash<T>(data, id));
    }

    void updateHashSize() {
        NodeHash<T> **newArr = new NodeHash<T> *[capacity_ * 2];
        for (int i = 0; i < 2 * capacity_; i++)
            newArr[i] = NULL;
        capacity_ *= 2;
        insert2new(newArr, false);
        capacity_ /= 2;
        startErase(arr_);
        delete[] arr_;
        arr_ = newArr;
        capacity_ *= 2;
    }

    void updateHashSizeAfterDelete() {

        NodeHash<T> **newArr = new NodeHash<T> *[capacity_/ 2];
        for (int i = 0; i < capacity_ / 2; i++)
            newArr[i] = NULL;
        capacity_ /= 2;
        insert2new(newArr, true);
        capacity_ *= 2;
        startErase(arr_);
        delete[] arr_;
        //updaets hash value and pointer
        arr_ = newArr;
        capacity_ /= 2;
    }
//    void resizeTable(){
//        HashTable new_table(capacity_ * 2);
//        for(int i = 0;i < capacity_;i++){
//            NodeHash<T> *curr = arr_[i];
//            while(curr){
//                new_table.insert(curr->getData(),curr->getId());
//                curr = curr->getNext();
//            }
//        }
//        eraseTable(this->arr_);
//        delete[] this->arr_;
//        this->arr_ = new_table.arr_;
//        this->capacity_ *= 2;
//    }

//    void eraseTable(NodeHash<T> **erase){
//        for (int i = 0; i < capacity_; i++) {
//            NodeHash<T> *curr = arr_[i];
//            while (curr) {
//                NodeHash<T> *prev = curr;
//                curr = curr->getNext();
//                delete prev;
//            }
//            arr_[i] = NULL;
//        }
//    }

//    void shrinkTable(){
//        HashTable new_table(capacity_/2);
//        for(int i = 0;i < capacity_;i++){
//            NodeHash<T> *curr = arr_[i];
//            while(curr){
//                new_table.insert(curr->getData(),curr->getId());
//                curr = curr->getNext();
//            }
//        }
//        eraseTable(this->arr_);
//        delete[] arr_;
//        this->arr_ = new_table.arr_;
//        this->capacity_ /= 2;
//    }

public:

    HashTable(){
        arr_ = new NodeHash<T> *[INITIAL_SIZE];
        for(int i =0;i < INITIAL_SIZE;i++)
            arr_[i]=NULL;
        capacity_ = INITIAL_SIZE;
        size_ = 0;
    }

    explicit HashTable(int size) {
        arr_ = new NodeHash<T> *[size];
        for (int i = 0; i < size; i++)
            arr_[i] = NULL;
        size_ = 0;
        capacity_ = size;
    }

    ~HashTable(){
        startErase(this->arr_);
        delete arr_;
    }

    NodeHash<T> *get(int id){
        int modu = hash(id);
        if(!this->arr_[modu])
            return NULL;
        NodeHash<T> *first = arr_[modu];
        while(first->getNext()!=NULL){
            if(first->getId()==id)
                return first;
            first=first->getNext();
        }
        if(first->getId()==id)
            return first;
        return NULL;
    }

    void insert(T data,int id){
        if(get(id))
            return;
        int modu = hash(id);
        if(!this->arr_[modu]){
            arr_[modu] = new NodeHash<T>(data,id);
        }else {
            NodeHash<T> *first = arr_[modu];
            while (first->getNext()!=NULL){
                first = first->getNext();
            }
            NodeHash<T> *new_first = new NodeHash<T>(data,id);
            first->setNext(new_first);
        }
        size_++;
        if(size_/capacity_== 2) updateHashSize();
    }

    void remove(int id){
        NodeHash<T> *first= get(id);
        if(!first)
            return;
        int modu = hash(id);
        if(first == arr_[modu]){
            arr_[modu] = first->getNext();
            delete first;
            size_--;
            if(size_ == 0) return;
            if(capacity_/size_==4) updateHashSizeAfterDelete();
            return;
        }
        NodeHash<T> *before = arr_[modu];
        while(before!=NULL){
            if(before->getNext()==first){
                if(first->getNext()!=NULL)
                    before->setNext(first->getNext());
                else
                    before->setNext(NULL);
                delete first;
            }
            before = before->getNext();
        }
        size_--;
        if(capacity_/size_==3) updateHashSizeAfterDelete();
    }

    int getCapacity(){
        return this->capacity_;
    }

    int getSize(){
        return this->size_;
    }

    NodeHash<T> **getArr(){
        return this->arr_;
    }



};






#endif //HASHTESTUNIT_CHAINHASHING_H
