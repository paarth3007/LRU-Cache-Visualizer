#include<iostream>
#include<unordered_map>
#include<chrono>

using namespace std;
//Node for doubly linked list
class Node{
    public:
    int key,value;
    Node*prev;
    Node*next;

    std::chrono::time_point<std::chrono::steady_clock> expiry;

    Node(int k,int v,int ttl)
     :key(k),value(v),prev(nullptr),next(nullptr){
        expiry=std::chrono::steady_clock::now()+std::chrono::seconds(ttl);
     }

};
class LRUCache{
    private:
    int capacity;
    unordered_map<int,Node*>cache;
    Node*head;
    Node*tail;

    void remove(Node*node){
        node->prev->next=node->next;
        node->next->prev=node->prev;
    }
    //Insert node right after head(MRU)

    void insert(Node*node){
        node->next=head->next;
        node->prev=head;

        head->next->prev=node;
        head->next=node;
    }
    //check expiration
    bool isExpired(Node*node){
        return std::chrono::steady_clock::now()>node->expiry;
    }
    //cleanup expired nodes(lazy cleanup)
    void cleanupExpired(){
        Node*curr=tail->prev;

        while(curr!=head){
            if(!isExpired(curr))break;

            Node*prevNode=curr->prev;
            remove(curr);
            cache.erase(curr->key);
            delete curr;
            curr=prevNode;
        }
    }

    public: 
    LRUCache(int cap):capacity(cap){
        head=new Node(0,0,0);
        tail=new Node(0,0,0);

        head->next=tail;
        tail->prev=head;
    }
    int get(int key){
        if(cache.find(key)==cache.end())return -1;

        Node*node=cache[key];
        if(isExpired(node)){
            remove(node);
            cache.erase(key);
            delete node;
            return -1;
        }

        //Move to front(MRU)
        remove(node);
        insert(node);

        return node->value;
    }

    void put(int key,int value,int ttl){
        //Remove exisiting key if present

        if(cache.find(key)!=cache.end()){
            Node*existing=cache[key];
            remove(existing);
            delete existing;
            cache.erase(key);
        }

        cleanupExpired();
        Node*node=new Node(key,value,ttl);
        insert(node);
        cache[key]=node;

        //evict lru if cap exceeded
        if(cache.size()>capacity){
            Node*lru=tail->prev;

            remove(lru);
            cache.erase(lru->key);
            delete lru;

        }

    }
    void display(){
        Node*curr=head->next;

        cout<<"Cache: ";
        while(curr!=tail){
            cout<<"("<<curr->key<<","<<curr->value<<")";
            curr=curr->next;
        }
        cout<<endl;




    }

            ~LRUCache(){
        Node*curr=head;
        while(curr){
        Node*next=curr->next;
        delete curr;
        curr=next;
    }
}
};
int main(){
    
     int capacity;

    cout << "Enter cache capacity: ";
    cin >> capacity;


    LRUCache cache(capacity);
    cache.put(1,10,5);
    cache.put(4,35,5);

    cache.display();

    cout<<"Get 1: "<<cache.get(1)<<endl;

    cache.display();
    cache.put(5,2,12); //evicts LRU

    cache.display();

    return 0;

}
