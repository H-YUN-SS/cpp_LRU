#include<stdexcept>
#include<iostream>
#include<iostream>
#include<string>
#include<utility>
#include<unordered_map>
#include<list>


template <typename K,typename V>
class LRUCache
{
public:
    explicit LRUCache(size_t capacity) : cap_(capacity)
    {
        if (cap_==0)
        {
            throw std::invalid_argument("capacity can't <0");
        }
        
    }
    bool get(const K& key ,V& out)
    {
        auto it=index_.find(key);
        if(it==index_.end())
        {
            return false;
        }
        items_.splice(items_.begin(),items_,it->second);
        out=it->second->second;
        return true;
    }
private:
    size_t cap_;
    std::list<std::pair<K,V>>items_;
    std::unordered_map<K,typename std::list<std::pair<K,V>>::iterator>index_;
    
};



