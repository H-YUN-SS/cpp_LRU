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

    void erase(const K&key)
    {
        auto it=index_.find(key);
        if(it==index_.end())
        {
            return;
        }
        items_.erase(it->second);
        index_.erase(it);
    }

    void clear()
    {
        items_.clear();
        index_.clear();
    }
    
    size_t size() const
    {
        return items_.size();
    }

    size_t capacity() const
    {
        return cap_;
    }

    void resize(size_t newcap)
    {
        if(newcap==0)
        {
            throw std::invalid_argument("new capecity cant't ==0 ");
        }
        cap_=newcap;

        while(items_.size()>cap_)
        {
            auto &old_item=items_.back();
            index_.erase(old_item.first);
            items_.pop_back();
        }
    }

    void debug_print()const
    {
        std::cout<<"[MRU->LRU]";
        for(auto it=items_.begin();it!=items_.end();it++)
        {
            std::cout<<"("<<it->first<<":"<<it->second<<")";
        }
        std::cout<<"\n"<<'\n';
    }


private:
    size_t cap_;
    std::list<std::pair<K,V>>items_;
    std::unordered_map<K,typename std::list<std::pair<K,V>>::iterator>index_;
    
};



int main()
{
    LRUCache<int,std::string>cache(3);
    std::cout<<" ===插入三个元素=== "<<std::endl;
    cache.put(1,"A");
    cache.put(2,"B");
    cache.put(3,"C");
    cache.debug_print();
    
    std::cout<<"\n ===获取key=2 === "<<std::endl;
    std::string out;
    if(cache.get(2,out))
    {
        std::cout<<"get 2 -> "<<out << std::endl;
        cache.debug_print();
    }

    std::cout<<"=== 插入key=4 (触发淘汰) ==="<<std::endl;
    cache.put(4,"D");
    cache.debug_print();

    std::cout<<"=== 更新key=2 ==="<<std::endl;
    cache.put(2,"B2");
    cache.debug_print();

    std::cout<<"=== 删除key=3 ==="<<std::endl;
    cache.erase(3);
    cache.debug_print();

    std::cout<<"=== 调整容量从3到2 ==="<<std::endl;
    std::cout<<"当前大小: "<<cache.size()<<",容量： "<<cache.capacity()<<std::endl;
    cache.debug_print();
    cache.resize(2);
    std::cout<<"调整后大小: "<<cache.size()<<",容量： "<<cache.capacity()<<std::endl;
    cache.debug_print();

    std::cout<<"=== 调整容量从2到5 ==="<<std::endl;
    std::cout<<"当前大小: "<<cache.size()<<",容量： "<<cache.capacity()<<std::endl;
    cache.debug_print();
    cache.resize(5);
    std::cout<<"调整后大小: "<<cache.size()<<",容量： "<<cache.capacity()<<std::endl;
    cache.put(5, "E");
    cache.put(6, "F");
    cache.debug_print();
    

    std::cout << "\n=== 清空缓存 ===" << std::endl;
    cache.clear();
    std::cout << "清空后大小: " << cache.size() << std::endl;
    cache.debug_print();  
    return 0;
}