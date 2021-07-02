// Copyright 2021 Yash Pandey(YP) - EmperorYP7

#include <functional>
#include <list>

namespace custom
{

template<typename KeyType, 
    typename ValueType, 
    typename HashFunc = std::hash<KeyType>,
    typename PredicateFunc = std::equal_to<KeyType>,
    typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>
>
class hash_bucket {
private:
    typedef std::pair<const KeyType, ValueType> pair_type;
    typedef pair_type* pair_ptr;
private:
    pair_ptr m_head = nullptr;
    Allocator m_allocate;
    size_t m_size = 0;
    PredicateFunc m_func;
private:
    void reallocate(size_t n)
    {
        pair_ptr new_ptr = m_allocate.allocate(n);
        size_t size = (m_size > n) ? n : m_size;
        for (size_t i = 0; i < size; ++i)
            new_ptr[i] = m_head[i];
        delete[] m_head;
        m_capacity = size;
        m_head = new_ptr;
    }

    constexpr bool is_equal(const KeyType& a, const KeyType& b)
    {
        return m_func(a, b);
    }

public:
    hash_bucket()
        : m_capacity(2)
    {
        reallocate(m_capacity);
    }
    hash_bucket(const KeyType &p_key, const ValueType &p_value)
        : m_capacity(2)
    {
        reallocate(m_capacity);
        m_head->first = p_key;
        m_head->second = p_value;
        m_size = 1;
    }
    ~hash_bucket()
    {
        delete[] m_head;
        m_head = nullptr;
        delete this;
        this = nullptr;
    }
    pair_ptr push(const KeyType& p_key, const ValueType& p_value)
    {
        if(m_size + 1 > m_capacity)
            m_capacity <<= 1, reallocate(m_capacity);
        m_head[m_size].first = p_key;
        m_head[m_size].second = p_value;
        ++m_size;
        return &m_head[m_size - 1];
    }
    pair_type& find(const KeyType& p_key)
    {
        for (size_t i = 0; i < m_size; ++i) {
            if(is_equal(m_head[i].first, p_key))
                return m_head[i];
        }
        return nullptr;
    }
    constexpr size_t nodes_in_bucket()
    {
        return m_size;
    }
};

template<typename KeyType, 
    typename ValueType, 
    typename HashFunc = std::hash<KeyType>,
    typename PredicateFunc = std::equal_to<KeyType>,
    typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>
>
class hashtable {
private:
    typedef hash_bucket<KeyType, ValueType, HashFunc, PredicateFunc, Allocator>* bucket_ptr;
    typedef std::allocator<hash_bucket<KeyType, ValueType, HashFunc, PredicateFunc, Allocator>> bucket_alloc;
    typedef std::pair<const KeyType, ValueType> pair_type;
private:
    size_t m_bucket_size = 0;
    size_t m_node_size = 0;
    bucket_alloc m_bucket_allocator;
    bucket_ptr m_bucket_head = nullptr;
public:
    hashtable()
        : m_bucket_size(2)
    {
        reallocate(2);
    }
    hashtable(hashtable &p_h)
        : m_bucket_size(p_h.m_bucket_size), m_node_size(p_h.m_node_size), m_bucket_head(p_h.m_bucket_head)
    {}
    hashtable(hashtable&& p_h) {
        m_bucket_size = p_h.m_bucket_size;
        m_bucket_head = p_h.m_bucket_head;
        p_h = nullptr;
    }
    ~hashtable()
    {
        delete[] m_bucket_head;
        m_bucket_head = nullptr;
    }
    pair_ptr add(const KeyType& p_key, const ValueType& p_value) {
        auto index = hash_func(p_key);
        &m_bucket_head[p_key];
    }
    ValueType& get(const KeyType& p_key)
    {
        auto index = hash_func(p_key);
        m_bucket_head[index].m_capacity;
    }
    ValueType& operator[](const KeyType& __k)
    {
        return get(__k);
    }

    ValueType& operator[](KeyType&& __k)
    { 
        return h_table[std::move(__k)]; 
    }

private:
    void reallocate(size_t p_new_size)
    {
        bucket_ptr new_ptr = m_bucket_allocator.allocate(p_new_size);
        size_t size = (m_bucket_size > p_new_size) ? p_new_size : m_bucket_size;
        for (size_t i = 0; i < size; ++i)
            new_ptr[i] = m_bucket_head[i];
        delete[] m_bucket_head;
        m_bucket_head = new_ptr;
    }
    size_t hash_func(const KeyType& p_key) {
        return HashFunc()(p_key) % m_bucket_size;
    }
};

template<typename KeyType, 
    typename ValueType, 
    typename HashFunc = std::hash<KeyType>,
    typename PredicateFunc = std::equal_to<KeyType>,
    typename Allocator = std::allocator<std::pair<const KeyType, ValueType>>
>
class hashmap {
private:
    hashtable<KeyType, ValueType, HashFunc, PredicateFunc> h_table;
public:
    hashmap() = default;
    hashmap(hashmap &t) {
        h_table = t->h_table;
    }
    hashmap(hashmap&& t) {
        h_table = t.h_table;
        t.h_table.~hashtable();
    }
    ValueType& operator[](const KeyType& __k)
    { 
        return h_table[__k]; 
    }

    ValueType&
    operator[](KeyType&& __k)
    { 
        return h_table[std::move(__k)]; 
    }
};

} // custom namespace
