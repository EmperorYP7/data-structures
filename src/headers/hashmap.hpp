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
    typedef pair_type& pair_ref;
private:
    pair_ptr m_head = nullptr;
    Allocator m_allocator;
    size_t m_size = 0;
    size_t m_capacity = 0;
    PredicateFunc m_func;
private:
    void reallocate(size_t p_new_size)
    {
        pair_ptr new_data = m_allocator.allocate(p_new_size);
        size_t size = (m_size > p_new_size) ? p_new_size : m_size;
        for (size_t i = 0; i < size; ++i)
            new_data[i] = m_head[i];
        delete[] m_head;
        m_head = new_data;
        m_capacity = p_new_size;
    }

    constexpr bool is_equal(const KeyType& a, const KeyType& b)
    {
        return m_func(a, b);
    }

public:
    hash_bucket()
        : m_size(0)
    {
        reallocate(2);
    }
    hash_bucket(const KeyType &p_key, const ValueType &p_value)
        : m_size(0)
    {
        reallocate(2);
        m_head->first = p_key;
        m_head->second = p_value;
        m_size = 1;
    }
    ~hash_bucket()
    {
        delete[] m_head;
        m_head = nullptr;
    }
    pair_ref push(const KeyType& p_key, const ValueType& p_value)
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            if(is_equal(m_head[i].first, p_key)) {
                m_head[i].second = p_value;
                return m_head[i];
            }
        }
        if (m_size + 1 > m_capacity)
            m_capacity <<= 1, reallocate(m_capacity);
        m_head[m_size].first = p_key;
        m_head[m_size].second = p_value;
        return m_head[m_size++];
    }
    pair_ref push(const KeyType& p_key)
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            if(is_equal(m_head[i].first, p_key))
                return m_head[i];
        }
        if (m_size + 1 > m_capacity)
            m_capacity <<= 1, reallocate(m_capacity);
        m_head[m_size].first = p_key;
        return m_head[m_size++];
    }
    pair_ref find_or_create(const KeyType& p_key)
    {
        for (size_t i = 0; i < m_size; ++i)
        {
            if(is_equal(m_head[i].first, p_key))
                return m_head[i];
        }
        return push(p_key);
    }
    constexpr size_t nodes_in_bucket()
    {
        return m_size;
    }

    pair_ref get_node(size_t index)
    {
        return m_head[index];
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
    typedef hash_bucket<KeyType, ValueType, HashFunc, PredicateFunc, Allocator> bucket_type;
    typedef ValueType& value_ref;
    typedef bucket_type* bucket_ptr;
    typedef bucket_type& bucket_ref;
    typedef std::allocator<bucket_type> bucket_alloc;
    typedef std::pair<const KeyType, ValueType> pair_type;
private:
    size_t m_bucket_size = 0;
    size_t m_node_size = 0;
    bucket_alloc m_bucket_allocator;
    bucket_ptr m_bucket_head = nullptr;
private:
    void reallocate(size_t p_new_size)
    {
        bucket_ptr new_ptr = m_bucket_allocator.allocate(p_new_size);
        size_t size = (m_bucket_size > p_new_size) ? p_new_size : m_bucket_size;
        m_bucket_size = p_new_size;
        for (size_t i = 0; i < size; ++i)
        {
            for (size_t j = 0; j < m_bucket_head[i].nodes_in_bucket(); ++j)
            {
                auto pair = m_bucket_head[i].get_node(j);
                size_t index = hash_func(pair.first);
                new_ptr[index].push(pair.first, pair.second);
            }
            m_bucket_head[i].~hash_bucket();
        }
        delete[] m_bucket_head;
        m_bucket_head = new_ptr;
    }
    size_t hash_func(const KeyType& p_key) {
        return HashFunc{}(p_key) % m_bucket_size;
    }
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
        for (size_t i = 0; i < m_bucket_size; ++i)
            m_bucket_head[i].~hash_bucket();
        delete[] m_bucket_head;
        m_bucket_head = nullptr;
    }
    value_ref get(const KeyType& p_key)
    {
        auto index = hash_func(p_key);
        auto pair = m_bucket_head[index].find_or_create(p_key);
        return pair.second;
    }
    value_ref operator[](const KeyType& __k)
    {
        return get(__k);
    }

    value_ref operator[](KeyType&& __k)
    { 
        return get(std::move(__k)); 
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
    ~hashmap()
    {
        h_table.~hashtable();
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
