#include<stdexcept>
#include<iostream>
#include<iostream>
#include<string>
#include<utility>
#include<unordered_map>
#include<list>


template <typename K,typename V>
class LRUcache
{
public:
    explicit LRUcache(size_t capacity) : cap_(capacity)
    {
        if (cap_==0)
        {
            throw std::invalid_argument("capacity can't <0");
        }
        
    }
private:
    size_t cap_;
    std::list<std::pair<K,V>>items;
}