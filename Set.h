#ifndef SET
#define SET

#include <vector>
#include <list>

template<class Key>
class Set {

    const float max_load_;
    int max_size_;
    int size_;

    std::vector<std::list<Key>> hash_table_;

    int Hash(Key key) {
        return std::hash<Key>{}(key) % max_size_;
    }

    float LoadFactor() {
        return ((float) size_) / ((float) max_size_);
    }

    void Resize() {
        max_size_ *= 2;
        std::vector<std::list<Key>> new_hash_table(max_size_, std::list<Key>());

        for (auto & chain : hash_table_)
        {
            while (!chain.empty())
            {
                Key key = chain.front();
                int new_hash_code = Hash(key);
                chain.pop_front();

                new_hash_table[new_hash_code].push_back(key);
            }
        }

        hash_table_ = new_hash_table;
    }

public:

    explicit Set(const float max_load = 0.75, int max_size = 10) : max_load_(max_load), max_size_(max_size) {
        hash_table_ = std::vector<std::list<Key>>(max_size, std::list<Key>());
        size_ = 0;
    }

    ~Set() {
        Clear();
    }

    bool Contains(Key key) {
        int hash_code = Hash(key);
        std::list<Key> & chain = hash_table_[hash_code];
        for (const auto & node_key : chain)
        {
            if (node_key == key)
            {
                return true;
            }
        }
        return false;
    }

    void Insert(Key key) {
        if (!Contains(key))
        {
            int hash_code = Hash(key);
            hash_table_[hash_code].push_back(key);
            size_++;

            if (LoadFactor() >= max_load_)
            {
                Resize();
            }
        }
    }

    void Remove(Key key) {
        if (Contains(key))
        {
            int hash_code = Hash(key);
            hash_table_[hash_code].remove(key);
            size_--;
        }
    }

    int Size() {
        return size_;
    }

    bool Empty() {
        return size_ == 0;
    }

    void Clear() {
        for (auto & chain : hash_table_)
        {
            chain.clear();
        }
        hash_table_.clear();
        size_ = 0;
    }
};


#endif
