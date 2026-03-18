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
    
    void put(const K& key, V value)
    {
        auto it = index_.find(key);
        if (it != index_.end())
        {
            it->second->second = std::move(value);
            items_.splice(items_.begin(), items_, it->second);
            return;
        }
        if (items_.size() == cap_)
        {
            auto& old_item = items_.back();
            index_.erase(old_item.first);
            items_.pop_back();
        }
        items_.emplace_front(key, std::move(value));
        index_[items_.front().first] = items_.begin();
    }
private:
    size_t cap_;
    std::list<std::pair<K,V>>items_;
    std::unordered_map<K,typename std::list<std::pair<K,V>>::iterator>index_;
    
};



