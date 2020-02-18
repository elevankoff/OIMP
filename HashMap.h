template<class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
public:
    class HashNode {
    public:
        size_t it;
        KeyType first;
        ValueType second;
        bool deleted = false;
        HashNode(const KeyType& key, const ValueType& value) : first(key), second(value) {}
    };

    class iterator : public std::list<HashNode>::iterator {
    public:
        iterator(typename std::list<HashNode>::iterator other) : std::list<HashNode>::iterator(other) {}
        iterator() {}
    };

    class const_iterator : public std::list<HashNode>::const_iterator {
    public:
        const_iterator(typename std::list<HashNode>::const_iterator other) : std::list<HashNode>::const_iterator(other) {}
        const_iterator() {}
    };

private:
    static const size_t capacity = 2'000'000;
    std::list<HashNode> list;
    std::vector<typename std::list<HashNode>::iterator> v;
    std::vector<bool> deleted;
    Hash hasher;

public:

    int tableSize = 0;

    HashMap(const Hash& hasher = Hash()) : v(capacity), hasher(hasher), deleted(capacity) {
        std::fill(v.begin(), v.end(), list.end());
    }

    template <class Iterator>
    HashMap(Iterator begin, const Iterator end, const Hash& hasher = Hash()) : v(capacity), hasher(hasher), deleted(capacity) {
        std::fill(v.begin(), v.end(), list.end());
        while (begin != end) {
            insert(*begin);
            begin++;
        }
    }

    HashMap(HashMap::iterator begin, const HashMap::iterator end, const Hash& hasher = Hash()) : v(capacity), hasher(hasher), deleted(capacity) {
        std::fill(v.begin(), v.end(), list.end());
        while (begin != end) {
            insert(std::make_pair((*begin).first, (*begin).second));
            begin++;
        }
    }

    HashMap(const std::initializer_list<std::pair<KeyType, ValueType>> it, const Hash& hasher = Hash()) : HashMap(it.begin(), it.end(), hasher) {}

    int size() const {
        return tableSize;
    }

    bool empty() const {
        return tableSize == 0;
    }

    void insert(const std::pair<KeyType, ValueType>& x) {
        size_t hash = hasher(x.first) % capacity;
        while ((v[hash] != list.end() && (*v[hash]).first != x.first) || deleted[hash] == true) hash = (hash + 1) % capacity;
        if (v[hash] == list.end()) {
            list.push_back(HashNode(x.first, x.second));
            list.back().deleted = false;
            list.back().it = hash;
            v[hash] = prev(list.end());
            tableSize++;
        }
    }

    void erase(const KeyType& key) {
        size_t hash = hasher(key) % capacity;
        while ((v[hash] != list.end() && (*v[hash]).first != key) || deleted[hash] == true) hash = (hash + 1) % capacity;
        if (v[hash] != list.end()) {
            list.erase(v[hash]);
            v[hash] = list.end();
            deleted[hash] = true;
            tableSize--;
        }
    }

    Hash hash_function() const {
        return hasher;
    }

    typename HashMap::const_iterator begin() const {
        return list.begin();
    }

    typename HashMap::iterator begin() {
        return list.begin();
    }

    const typename HashMap::const_iterator end() const {
        return list.end();
    }

    typename HashMap::iterator end() {
        return list.end();
    }

    typename HashMap::const_iterator find(const KeyType& key) const {
        size_t hash = hasher(key) % capacity;
        while ((v[hash] != list.end() && (*v[hash]).first != key) || deleted[hash] == true) hash = (hash + 1) % capacity;
        if (v[hash] != list.end()) return v[hash];
        return end();
    }

    typename HashMap::iterator find(const KeyType& key) {
        size_t hash = hasher(key) % capacity;
        while ((v[hash] != list.end() && (*v[hash]).first != key) || deleted[hash] == true) hash = (hash + 1) % capacity;
        if (v[hash] != list.end()) return v[hash];
        return end();
    }

    ValueType& operator [] (const KeyType& key) {
        size_t hash = hasher(key) % capacity;
        while ((v[hash] != list.end() && (*v[hash]).first != key) || deleted[hash] == true) hash = (hash + 1) % capacity;
        if (v[hash] != list.end()) return (*v[hash]).second;
        insert(std::make_pair(key, ValueType()));
        return (*find(key)).second;
    }

    const ValueType& at(const KeyType& key) const {
        size_t hash = hasher(key) % capacity;
        while ((v[hash] != list.end() && (*v[hash]).first != key) || deleted[hash] == true) hash = (hash + 1) % capacity;
        if (v[hash] != list.end()) return (*v[hash]).second;
        throw std::out_of_range("");
    }

    void clear() {
        auto first = begin(), last = end();
        while (first != last) {
            v[(*first).it] = list.end(), first++;
        }
        list.clear();
        tableSize = 0;
    }
};
