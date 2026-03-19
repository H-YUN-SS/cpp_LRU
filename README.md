# LRU Cache Implementation (C++)

一个高效、泛型的 LRU（Least Recently Used）缓存实现，使用 C++11 编写。

## 项目简介

本项目实现了一个基于最近最少使用算法的缓存系统，当缓存满时自动淘汰最久未使用的数据。使用双向链表 + 哈希表的组合数据结构，实现了 O(1) 时间复杂度的查找和更新操作。

## 核心特性

- **泛型实现**：支持任意类型的键和值
- **高效性能**：所有操作均为 O(1) 时间复杂度
- **自动淘汰**：缓存满时自动淘汰最久未使用的数据
- **动态调整**：支持运行时调整缓存容量
- **完整接口**：提供 get、put、erase、clear、resize 等完整操作

## 技术实现

### 数据结构设计

- **双向链表 (`std::list`)**：维护访问顺序
  - 头部：最新使用的元素（MRU）
  - 尾部：最久未使用的元素（LRU）

- **哈希表 (`std::unordered_map`)**：实现快速查找
  - 键：缓存的 key
  - 值：链表中对应节点的迭代器

### 时间复杂度

| 操作 | 时间复杂度 | 说明 |
|------|-----------|------|
| `get(key)` | O(1) | 哈希表查找 + 链表移动 |
| `put(key, value)` | O(1) | 哈希表查找 + 链表插入/删除 |
| `erase(key)` | O(1) | 哈希表查找 + 链表删除 |
| `clear()` | O(n) | 清空所有元素 |
| `resize(newcap)` | O(k) | k 为需要淘汰的元素数量 |

## 编译和运行

### 编译

```bash
g++ -std=c++11 LRU.cpp -o lru_cache
```

### 运行

```bash
./lru_cache
```

## 使用示例

```cpp
#include "LRU.cpp"

int main() {
    // 创建容量为3的LRU缓存
    LRUCache<int, std::string> cache(3);

    // 插入元素
    cache.put(1, "A");
    cache.put(2, "B");
    cache.put(3, "C");

    // 获取元素
    std::string out;
    if (cache.get(2, out)) {
        std::cout << "Found: " << out << std::endl;  // 输出: Found: B
    }

    // 缓存满时插入新元素，自动淘汰最久未使用的
    cache.put(4, "D");  // key=1 被淘汰

    // 更新已存在的键
    cache.put(2, "B2");  // 更新 key=2 的值并移到头部

    // 删除元素
    cache.erase(3);

    // 调整容量
    cache.resize(5);

    // 清空缓存
    cache.clear();

    return 0;
}
```

## API 文档

### 构造函数

```cpp
explicit LRUCache(size_t capacity);
```

创建一个指定容量的 LRU 缓存。

**参数：**
- `capacity`：缓存的最大容量（必须大于0）

**异常：**
- `std::invalid_argument`：当 capacity 为 0 时抛出

### 成员函数

#### `bool get(const K& key, V& out)`

获取指定键的值。

**参数：**
- `key`：要查找的键
- `out`：输出参数，存储找到的值

**返回值：**
- `true`：命中缓存
- `false`：未命中

**注意：** 命中时会将该键对应的节点移到链表头部。

#### `void put(const K& key, V value)`

插入或更新键值对。

**参数：**
- `key`：键
- `value`：值（使用移动语义传递）

**行为：**
- 键已存在：更新值并移到链表头部
- 键不存在：插入新键，若缓存满则淘汰最久未使用的键

#### `void erase(const K& key)`

删除指定键的缓存项。

**参数：**
- `key`：要删除的键

**注意：** 如果键不存在，则不做任何操作。

#### `void clear()`

清空缓存中的所有元素。

**注意：** 清空后缓存大小为0，但容量不变。

#### `size_t size() const`

获取当前缓存中的元素数量。

**返回值：** 当前元素数量

#### `size_t capacity() const`

获取缓存的最大容量。

**返回值：** 缓存容量

#### `void resize(size_t newcap)`

调整缓存容量。

**参数：**
- `newcap`：新的容量（必须大于0）

**异常：**
- `std::invalid_argument`：当 newcap 为 0 时抛出

**注意：** 如果新容量小于当前元素数量，会淘汰最久未使用的元素。

#### `void debug_print() const`

调试打印：按「最新使用→最久使用」顺序打印缓存内容。

**输出格式：** `[MRU->LRU] (key1:value1) (key2:value2) ...`

**注意：** 仅用于调试，生产环境建议移除或使用日志系统。

## 使用注意事项

1. **容量限制**：容量必须大于0，否则抛出异常
2. **键类型要求**：键类型 K 必须支持哈希（`std::hash<K>`）
3. **值类型要求**：值类型 V 建议支持移动语义以提高性能
4. **线程安全**：当前实现非线程安全，多线程环境下需要外部加锁
5. **内存管理**：使用智能指针或 RAII 技术管理资源

## 项目结构

```
cpp_LRU/
├── LRU.cpp          # LRU缓存实现（包含模板类定义和测试代码）
└── README.md        # 项目文档
```

## 性能优化建议

1. **使用移动语义**：传递值时使用 `std::move` 减少拷贝
2. **预分配内存**：对于已知大小的缓存，可以预分配哈希表容量
3. **避免频繁 resize**：尽量在初始化时确定合适的容量
4. **使用合适的键类型**：选择哈希冲突少的键类型

## 扩展功能建议

- [ ] 添加迭代器支持
- [ ] 实现线程安全版本
- [ ] 添加统计功能（命中率、访问次数等）
- [ ] 支持持久化存储
- [ ] 添加过期时间功能
- [ ] 实现批量操作接口

## 测试覆盖

当前实现包含以下测试场景：

- ✅ 插入元素到未满缓存
- ✅ 获取已存在的元素
- ✅ 缓存满时自动淘汰
- ✅ 更新已存在的键
- ✅ 删除指定元素
- ✅ 清空所有元素
- ✅ 动态调整容量（缩小）
- ✅ 动态调整容量（扩大）

## 许可证

本项目仅供学习和参考使用。

## 作者

H_YUN_SS

## 更新日志

### v1.0 (2026-03-19)
- ✨ 初始版本
- ✨ 实现核心 LRU 缓存功能
- ✨ 支持泛型键值对
- ✨ 添加完整的 API 接口
- ✨ 包含测试代码和示例

## 参考资源

- [LRU Cache - Wikipedia](https://en.wikipedia.org/wiki/Cache_replacement_policies#Least_recently_used_(LRU))
- [std::list - cppreference](https://en.cppreference.com/w/cpp/container/list)
- [std::unordered_map - cppreference](https://en.cppreference.com/w/cpp/container/unordered_map)
