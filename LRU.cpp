/**
 * @file LRU.cpp
 * @brief LRU (Least Recently Used) 缓存实现
 * @details 使用双向链表 + 哈希表实现高效的LRU缓存
 *          时间复杂度：get() O(1), put() O(1)
 *          空间复杂度：O(capacity)
 *
 * @author H_YUN_SS
 * @date 2026-03-19
 * @version 1.0
 *
 * @note 核心思想：
 *       - 双向链表维护访问顺序：头部=最新使用，尾部=最久未使用
 *       - 哈希表实现快速查找：key -> 链表节点迭代器
 *       - 两者结合实现O(1)时间复杂度的查找和更新
 *
 * @warning 使用注意事项：
 *       - 容量必须大于0，否则抛出 std::invalid_argument
 *       - 键类型K必须支持哈希（std::hash<K>）
 *       - 值类型V必须支持移动语义以提高性能
 *       - 非线程安全，多线程环境下需要外部加锁
 */

#include <stdexcept>
#include <iostream>
#include <string>
#include <utility>
#include <unordered_map>
#include <list>

/**
 * @class LRUCache
 * @brief 泛型LRU缓存类
 * @tparam K 键类型（必须支持std::hash）
 * @tparam V 值类型
 *
 * @details 实现了一个基于最近最少使用算法的缓存，当缓存满时自动淘汰最久未使用的数据
 *
 * @example
 * LRUCache<int, std::string> cache(3);  // 创建容量为3的缓存
 * cache.put(1, "value1");               // 插入键值对
 * std::string out;
 * if (cache.get(1, out)) {              // 获取值
 *     std::cout << out << std::endl;
 * }
 */
template <typename K, typename V>
class LRUCache
{
public:
    /**
     * @brief 构造函数
     * @param capacity 缓存的最大容量
     * @throws std::invalid_argument 当capacity为0时抛出异常
     * @note 使用explicit防止隐式类型转换
     */
    explicit LRUCache(size_t capacity) : cap_(capacity)
    {
        if (cap_ == 0)
        {
            throw std::invalid_argument("capacity can't be 0");
        }
    }

    /**
     * @brief 获取指定键的值
     * @param key 要查找的键
     * @param out 输出参数，存储找到的值
     * @return true=命中缓存，false=未命中
     * @note 命中时会将该键对应的节点移到链表头部（标记为最近使用）
     * @time_complexity O(1)
     */
    bool get(const K& key, V& out)
    {
        auto it = index_.find(key);
        if (it == index_.end())
        {
            return false;
        }
        items_.splice(items_.begin(), items_, it->second);
        out = it->second->second;
        return true;
    }

    /**
     * @brief 插入或更新键值对
     * @param key 键
     * @param value 值（使用移动语义传递）
     * @note 两种情况：
     *       1. 键已存在：更新值并移到链表头部
     *       2. 键不存在：插入新键，若缓存满则淘汰最久未使用的键
     * @time_complexity O(1)
     */
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

    /**
     * @brief 删除指定键的缓存项
     * @param key 要删除的键
     * @note 如果键不存在，则不做任何操作
     * @time_complexity O(1)
     */
    void erase(const K& key)
    {
        auto it = index_.find(key);
        if (it == index_.end())
        {
            return;
        }
        items_.erase(it->second);
        index_.erase(it);
    }

    /**
     * @brief 清空缓存中的所有元素
     * @note 清空后缓存大小为0，但容量不变
     * @time_complexity O(n)，n为当前元素数量
     */
    void clear()
    {
        items_.clear();
        index_.clear();
    }

    /**
     * @brief 获取当前缓存中的元素数量
     * @return 当前元素数量
     * @time_complexity O(1)
     */
    size_t size() const
    {
        return items_.size();
    }

    /**
     * @brief 获取缓存的最大容量
     * @return 缓存容量
     * @time_complexity O(1)
     */
    size_t capacity() const
    {
        return cap_;
    }

    /**
     * @brief 调整缓存容量
     * @param newcap 新的容量（必须大于0）
     * @throws std::invalid_argument 当newcap为0时抛出异常
     * @note 如果新容量小于当前元素数量，会淘汰最久未使用的元素
     * @time_complexity O(k)，k为需要淘汰的元素数量
     */
    void resize(size_t newcap)
    {
        if (newcap == 0)
        {
            throw std::invalid_argument("new capacity can't be 0");
        }
        cap_ = newcap;

        while (items_.size() > cap_)
        {
            auto& old_item = items_.back();
            index_.erase(old_item.first);
            items_.pop_back();
        }
    }

    /**
     * @brief 调试打印：按「最新使用→最久使用」顺序打印缓存内容
     * @note 输出格式：[MRU->LRU] (key1:value1) (key2:value2) ...
     *       MRU = Most Recently Used（最近使用）
     *       LRU = Least Recently Used（最久未使用）
     * @warning 仅用于调试，生产环境建议移除或使用日志系统
     */
    void debug_print() const
    {
        std::cout << "[MRU->LRU]";
        for (auto it = items_.begin(); it != items_.end(); it++)
        {
            std::cout << "(" << it->first << ":" << it->second << ")";
        }
        std::cout << "\n\n";
    }

private:
    size_t cap_;  ///< 缓存最大容量

    /**
     * @brief 双向链表，存储键值对
     * @details 顺序：头部=最新使用，尾部=最久未使用
     *          使用std::pair存储键值对
     */
    std::list<std::pair<K, V>> items_;

    /**
     * @brief 哈希表，实现快速查找
     * @details 键：缓存的key
     *          值：链表中对应节点的迭代器
     *          通过迭代器可以在O(1)时间内访问链表节点
     */
    std::unordered_map<K, typename std::list<std::pair<K, V>>::iterator> index_;
};

/**
 * @brief 主函数：测试LRUCache的核心功能
 * @details 演示LRU缓存的所有核心功能：
 *          - 插入元素（put）
 *          - 获取元素（get）
 *          - 缓存满时自动淘汰
 *          - 更新已存在的键
 *          - 删除元素（erase）
 *          - 调整容量（resize）
 *          - 清空缓存（clear）
 */
int main()
{
    // 1. 创建容量为3的LRU缓存（键=int，值=string）
    LRUCache<int, std::string> cache(3);

    // 2. 插入3个节点：1→A、2→B、3→C
    std::cout << "=== 插入三个元素 ===" << std::endl;
    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C");
    cache.debug_print();  // 预期输出：[MRU->LRU] (3:C)(2:B)(1:A)

    // 3. 获取key=2：命中，同时将2移到头部
    std::cout << "\n=== 获取key=2 ===" << std::endl;
    std::string out;
    if (cache.get(2, out))
    {
        std::cout << "get 2 -> " << out << std::endl;
        cache.debug_print();  // 预期输出：[MRU->LRU] (2:B)(3:C)(1:A)
    }

    // 4. 插入key=4：缓存满，淘汰最久未使用的key=1
    std::cout << "=== 插入key=4 (触发淘汰) ===" << std::endl;
    cache.put(4, "D");
    cache.debug_print();  // 预期输出：[MRU->LRU] (4:D)(2:B)(3:C)

    // 5. 更新key=2的值：将2的值改为B2，并移到头部
    std::cout << "=== 更新key=2 ===" << std::endl;
    cache.put(2, "B2");
    cache.debug_print();  // 预期输出：[MRU->LRU] (2:B2)(4:D)(3:C)

    // 6. 删除key=3：移除该节点
    std::cout << "=== 删除key=3 ===" << std::endl;
    cache.erase(3);
    cache.debug_print();  // 预期输出：[MRU->LRU] (2:B2)(4:D)

    // 7. 调整容量：从3缩小到2
    std::cout << "=== 调整容量从3到2 ===" << std::endl;
    std::cout << "当前大小: " << cache.size() << ", 容量: " << cache.capacity() << std::endl;
    cache.debug_print();
    cache.resize(2);
    std::cout << "调整后大小: " << cache.size() << ", 容量: " << cache.capacity() << std::endl;
    cache.debug_print();  // 预期输出：[MRU->LRU] (2:B2)(4:D)

    // 8. 调整容量：从2扩大到5
    std::cout << "=== 调整容量从2到5 ===" << std::endl;
    std::cout << "当前大小: " << cache.size() << ", 容量: " << cache.capacity() << std::endl;
    cache.debug_print();
    cache.resize(5);
    std::cout << "调整后大小: " << cache.size() << ", 容量: " << cache.capacity() << std::endl;
    cache.put(5, "E");
    cache.put(6, "F");
    cache.debug_print();  // 预期输出：[MRU->LRU] (6:F)(5:E)(2:B2)(4:D)

    // 9. 清空缓存
    std::cout << "\n=== 清空缓存 ===" << std::endl;
    cache.clear();
    std::cout << "清空后大小: " << cache.size() << std::endl;
    cache.debug_print();  // 预期输出：[MRU->LRU]

    return 0;
}
