#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>
#include <algorithm>
#include <stdexcept>

namespace sjtu {
    template<class T>
    class double_list {
        struct atom {
            T *data;
            atom *pre, *next;

            atom(atom *a1, atom *a2, T *t) : pre(a1), next(a2), data(t) {
            };
        };

        atom *begin_atom, *end_atom;
        size_t size;

        static bool is_end_atom(atom *ato) {
            return ato->next == nullptr && ato->data == nullptr;
        }

    public:
        /**
         * elements
         * add whatever you want
         */
        class iterator;
        class const_iterator;
        friend class iterator;
        // --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
         */
        double_list() : size(0) {
            begin_atom = end_atom = new atom(nullptr, nullptr, nullptr);
        }

        double_list(const double_list<T> &other) : size(0) {
            begin_atom = end_atom = new atom(nullptr, nullptr, nullptr);
            atom *current = other.begin_atom;
            while (!is_end_atom(current)) {
                insert_tail(*(current->data));
                current = current->next;
            }
        }

        ~double_list() {
            while (size != 0)delete_head();
            delete end_atom;
        }

        double_list &operator=(const double_list &other) {
            if (this == &other) return *this;
            clear();
            atom *current = other.begin_atom;
            while (!is_end_atom(current)) {
                insert_tail(*(current->data));
                current = current->next;
            }
            return *this;
        }

        class iterator {
        public:
            /**
             * elements
             * add whatever you want
             */
            double_list<T> *contain_it;
            atom *it;
            // --------------------------
            /**
             * the follows are constructors and destructors
             * you can also add some if needed.
             */
            iterator() : it(nullptr), contain_it(nullptr) {
            }

            iterator(atom *t, double_list<T> *c_t) : it(t), contain_it(c_t) {
            }

            iterator(const iterator &t) : it(t.it), contain_it(t.contain_it) {
            }

            ~iterator() = default;

            /**
             * iter++
             */
            iterator operator++(int) {
                iterator tmp = *this;
                if (is_end_atom(it)) throw std::invalid_argument("it++");
                if (it != nullptr) {
                    it = it->next;
                    return tmp;
                }
                throw std::invalid_argument("it++");
            }

            /**
             * ++iter
             */
            iterator &operator++() {
                if (is_end_atom(it)) throw std::invalid_argument("++it");
                if (it != nullptr) {
                    it = it->next;
                    return *this;
                }
                throw std::invalid_argument("++it");
            }

            /**
             * iter--
             */
            iterator operator--(int) {
                iterator tmp = *this;
                if (it != nullptr && it->pre == nullptr) throw std::invalid_argument("it--");
                if (it != nullptr) {
                    it = it->pre;
                    return tmp;
                }
                throw std::invalid_argument("it--");
            }

            /**
             * --iter
             */
            iterator &operator--() {
                if (it != nullptr && it->pre == nullptr) throw std::invalid_argument("--it");
                if (it != nullptr) {
                    it = it->pre;
                    return *this;
                }
                throw std::invalid_argument("--it");
            }

            /**
             * if the iter didn't point to a value
             * throw " invalid"
             */
            T &operator*() const {
                if (it != nullptr && !is_end_atom(it)) return *(it->data);
                throw std::invalid_argument("*it");
            }

            /**
             * other operation
             */
            T *operator->() const {
                if (it != nullptr && !is_end_atom(it)) return it->data;
                throw std::invalid_argument("->it");
            }

            bool operator==(const iterator &rhs) const {
                return rhs.it == it;
            }

            bool operator!=(const iterator &rhs) const {
                return rhs.it != it;
            }

            bool operator==(const const_iterator &rhs) const { return it == rhs.it; }
            bool operator!=(const const_iterator &rhs) const { return it != rhs.it; }

            [[nodiscard]] bool validity_check() const {
                return double_list<T>::validity_check(const_iterator(it, contain_it));
            }

            operator const_iterator() const {
                return const_iterator(*this);
            }
        };

        class const_iterator {
        public:
            atom *it;
            double_list<T> *contain_it;

            const_iterator() : it(nullptr), contain_it(nullptr) {
            }

            const_iterator(atom *t, double_list<T> *c_t) : it(t), contain_it(c_t) {
            }

            const_iterator(const iterator &t) : it(t.it), contain_it(t.contain_it) {
            }

            const_iterator(const const_iterator &t) : it(t.it), contain_it(t.contain_it) {
            }

            ~const_iterator() = default;

            const_iterator operator++(int) {
                const_iterator tmp = *this;
                if (is_end_atom(it)) throw std::invalid_argument("it++");
                if (it != nullptr) {
                    it = it->next;
                    return tmp;
                }
                throw std::invalid_argument("it++");
            }

            const_iterator &operator++() {
                if (is_end_atom(it)) throw std::invalid_argument("++it");
                if (it != nullptr) {
                    it = it->next;
                    return *this;
                }
                throw std::invalid_argument("++it");
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                if (it != nullptr && it->pre == nullptr) throw std::invalid_argument("it--");
                if (it != nullptr) {
                    it = it->pre;
                    return tmp;
                }
                throw std::invalid_argument("it--");
            }

            const_iterator &operator--() {
                if (it != nullptr && it->pre == nullptr) throw std::invalid_argument("--it");
                if (it != nullptr) {
                    it = it->pre;
                    return *this;
                }
                throw std::invalid_argument("--it");
            }

            const T &operator*() const {
                if (it != nullptr && !is_end_atom(it)) return *(it->data);
                throw std::invalid_argument("*cit");
            }

            const T *operator->() const {
                if (it != nullptr && !is_end_atom(it)) return it->data;
                throw std::invalid_argument("->cit");
            }

            bool operator==(const const_iterator &rhs) const {
                return rhs.it == it;
            }

            bool operator!=(const const_iterator &rhs) const {
                return rhs.it != it;
            }

            bool operator==(const iterator &rhs) const { return it == rhs.it; }
            bool operator!=(const iterator &rhs) const { return it != rhs.it; }

            [[nodiscard]] bool validity_check() const {
                return double_list<T>::validity_check(*this);
            }
        };

        /**
         * return an iterator to the beginning
         */
        iterator begin() {
            if (empty()) return end();
            return iterator(begin_atom, this);
        }

        /**
         * return an iterator to the ending
         * in fact, it returns the iterator point to nothing,
         * just after the last element.
         */
        iterator end() {
            return iterator(end_atom, this);
        }

        const_iterator begin() const {
            return const_iterator(begin_atom, const_cast<double_list<T> *>(this));
        }

        const_iterator end() const {
            return const_iterator(end_atom, const_cast<double_list<T> *>(this));
        }

        /**
         * if the iter didn't point to anything, do nothing,
         * otherwise, delete the element pointed by the iter
         * and return the iterator point at the same "index"
         * e.g.
         * 	if the origin iterator point at the 2nd element
         * 	the returned iterator also point at the
         *  2nd element of the list after the operation
         *  or nothing if the list after the operation
         *  don't contain 2nd elememt.
         */
        iterator erase(iterator pos) {
            if (pos.it == nullptr || is_end_atom(pos.it)) return end();
            atom *tmp = pos.it->next;
            if (pos.it->pre != nullptr) pos.it->pre->next = pos.it->next;
            else begin_atom = tmp;
            tmp->pre = pos.it->pre;
            delete pos.it->data;
            delete pos.it;
            --size;
            if (!is_end_atom(tmp)) return iterator(tmp, this);
            else return end();
        }

        /**
         * the following are operations of double list
         */
        void insert_head(const T &val) {
            atom *first = new atom(nullptr, begin_atom, new T(val));
            begin_atom->pre = first;
            if (begin_atom == end_atom)end_atom->pre = first;
            begin_atom = first;
            ++size;
        }

        void insert_tail(const T &val) {
            atom *last = new atom(end_atom->pre, end_atom, new T(val));
            if (last->pre != nullptr)last->pre->next = last;
            else begin_atom = last;
            end_atom->pre = last;
            ++size;
        }

        iterator insert(iterator pos, const T &val) {
            if (!validity_check(pos))throw std::invalid_argument("insert_double_list");
            if (is_end_atom(pos.it)) {
                insert_tail(val);
                iterator ret = end();
                --ret;
                return ret;
            }
            if (pos.it->pre == nullptr) {
                insert_head(val);
                return begin();
            }
            atom *t = new atom(pos.it->pre, pos.it, new T(val));
            pos.it->pre->next = t;
            pos.it->pre = t;
            ++size;
            return iterator(t, this);
        }

        void delete_head() {
            erase(iterator(begin_atom, this));
        }

        void delete_tail() {
            erase(iterator(end_atom->pre, this));
        }

        /**
         * if didn't contain anything, return true,
         * otherwise false.
         */
        bool empty() const {
            return size == 0;
        }

        size_t length() const {
            return size;
        }

        void clear() {
            while (size != 0)delete_head();
        }

        void swap(double_list &other) {
            atom *tb = begin_atom;
            begin_atom = other.begin_atom;
            other.begin_atom = tb;
            atom *te = end_atom;
            end_atom = other.end_atom;
            other.end_atom = te;
            size_t ts = size;
            size = other.size;
            other.size = ts;
        }

        [[nodiscard]] static bool validity_check(const const_iterator &it) {
            if (it.contain_it == nullptr)return false;
            iterator tmp = it.contain_it->begin();
            while (true) {
                if (tmp == it)return true;
                if (tmp == it.contain_it->end()) break;
                ++tmp;
            }
            return false;
        }
    };



    template<class T>
    class deque;
    template<class T>
    class block {
    private:
        friend class deque<T>;
        T **_data;
        int _head,_tail,_count,_capacity;
        int _idx(int i) const {
            return (i + _capacity) % _capacity;
        }

    public:
        class iterator {
            friend class block;
        private:
            block *_ptr;
            int _pos;

        public:
            iterator(block *p = nullptr, int pos = 0) : _ptr(p), _pos(pos) {}
            T& operator*() const {
                return *(_ptr->_data[_ptr->_idx(_ptr->_head + _pos)]);
            }

            T* operator->() const {
                return _ptr->_data[_ptr->_idx(_ptr->_head + _pos)];
            }

            iterator& operator++() {
                _pos++;
                return *this;
            }

            iterator operator++(int) {
                iterator temp = *this;
                _pos++;
                return temp;
            }

            iterator& operator--() {
                _pos--;
                return *this;
            }
            iterator operator--(int) {
                iterator temp = *this;
                _pos--;
                return temp;
            }
            bool operator==(const iterator &rhs) const {
                return _ptr == rhs._ptr && _pos == rhs._pos;
            }

            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }
            iterator operator+(int n) const { return iterator(_ptr, _pos + n); }
            iterator operator-(int n) const { return iterator(_ptr, _pos - n); }
            int operator-(const iterator &rhs) const { return _pos - rhs._pos; }
        };

        block(int cap) : _head(0), _tail(0), _count(0), _capacity(cap + 2) {
            _data = new T*[_capacity];
            for (int i = 0; i < _capacity; ++i) _data[i] = nullptr;
        }

        block(const block &other) : _head(0), _tail(other._count), _count(other._count), _capacity(other._capacity) {
            _data = new T*[_capacity];
            for (int i = 0; i < _count; ++i) {
                _data[i] = new T(other[i]);
            }
            for (int i = _count; i < _capacity; ++i) _data[i] = nullptr;
        }

        block &operator=(const block &other) {
            if (this == &other) return *this;
            for (int i = 0; i < _capacity; ++i) {
                if (_data[i]) {
                    delete _data[i];
                    _data[i] = nullptr;
                }
            }
            delete[] _data;
            _capacity = other._capacity;
            _head = 0;
            _tail = other._count;
            _count = other._count;
            _data = new T*[_capacity];
            for (int i = 0; i < _count; ++i) _data[i] = new T(other[i]);
            for (int i = _count; i < _capacity; ++i) _data[i] = nullptr;
            return *this;
        }

        ~block() {
            for (int i = 0; i < _capacity; ++i) {
                if (_data[i]) delete _data[i];
            }
            delete[] _data;
        }
        size_t size() const { return _count; }
        bool empty() const { return _count == 0; }
        bool full() const { return _count >= _capacity - 1; }

        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, _count); }

        const T &front() const { return (*this)[0]; }
        const T &back() const { return (*this)[_count - 1]; }

        static int position(const iterator &it) { return it._pos; }

        T& operator[](int pos) {
            return *_data[_idx(_head + pos)];
        }

        const T& operator[](int pos) const {
            return *_data[_idx(_head + pos)];
        }
        void push_back(const T &val) {
            _data[_tail] = new T(val);
            _tail = _idx(_tail + 1);
            _count++;
        }

        void push_front(const T &val) {
            _head = _idx(_head - 1);
            _data[_head] = new T(val);
            _count++;
        }

        void pop_back() {
            _tail = _idx(_tail - 1);
            delete _data[_tail];
            _data[_tail] = nullptr;
            _count--;
        }

        void pop_front() {
            delete _data[_head];
            _data[_head] = nullptr;
            _head = _idx(_head + 1);
            _count--;
        }

        iterator insert(int pos, const T &val) {
            if (pos == 0) { push_front(val); return begin(); }
            if (pos == _count) { push_back(val); return iterator(this, _count - 1); }
            if (pos < _count / 2) {
                _head = _idx(_head - 1);
                for (int i = 0; i < pos; ++i) {
                    _data[_idx(_head + i)] = _data[_idx(_head + i + 1)];
                }
            } else {
                for (int i = _count; i > pos; --i) {
                    _data[_idx(_head + i)] = _data[_idx(_head + i - 1)];
                }
                _tail = _idx(_tail + 1);
            }
            _data[_idx(_head + pos)] = new T(val);
            _count++;
            return iterator(this, pos);
        }

        T insert_full(int pos, const T &val) {
            T ret = (*this)[_count - 1];
            insert(pos, val);
            pop_back();
            return ret;
        }

        iterator erase(int pos) {
            delete _data[_idx(_head + pos)];
            if (pos < _count / 2) {
                for (int i = pos; i > 0; --i) {
                    _data[_idx(_head + i)] = _data[_idx(_head + i - 1)];
                }
                _data[_head] = nullptr;
                _head = _idx(_head + 1);
            } else {
                for (int i = pos; i < _count - 1; ++i) {
                    _data[_idx(_head + i)] = _data[_idx(_head + i + 1)];
                }
                _tail = _idx(_tail - 1);
                _data[_tail] = nullptr;
            }
            _count--;
            if (pos > _count) return end();
            return iterator(this, pos);
        }
    };


    template<class T>
    class deque {
    public:
        typedef double_list<block<T>> BlockT_list;
        typedef typename BlockT_list::iterator BlockT_list_iterator;
        typedef typename BlockT_list::const_iterator BlockT_list_citerator;
        typedef typename block<T>::iterator Block_iterator;
        class iterator;
        class const_iterator;
    private:
        /*Components
         *length,ideal_length
         *double_list contains the blocks
         */
        size_t blockMinor = 4096 / sizeof(T);
        BlockT_list _blocks;
        size_t length, ideal_block_length;
        void check(iterator &pos) {
            auto &mp_it = pos.mp_it;
            auto &bl_it = pos.bl_it;
            if (mp_it->size() <= ideal_block_length * 2) return;
            block<T> next_block(ideal_block_length * 2);
            int offset = block<T>::position(bl_it);
            size_t half = mp_it->size() / 2;
            size_t right_size = mp_it->size() - half;
            for (size_t i = half; i < mp_it->size(); ++i) {
                next_block.push_back((*mp_it)[i]);
            }
            for (size_t i = 0; i < right_size; ++i) {
                mp_it->pop_back();
            }
            auto insert_pos = mp_it;
            ++insert_pos;
            _blocks.insert(insert_pos, next_block);
            if (offset > static_cast<int>(half + right_size)) {
                offset = static_cast<int>(half + right_size);
            }
            if (offset >= static_cast<int>(half)) {
                auto right_it = mp_it;
                ++right_it;
                mp_it = right_it;
                bl_it = mp_it->begin() + (offset - static_cast<int>(half));
            } else {
                bl_it = mp_it->begin() + offset;
            }
        }

        void conquer(iterator &pos) {
            auto &mp_it = pos.mp_it;
            auto &bl_it = pos.bl_it;
            if (mp_it == _blocks.end()) return;
            if (mp_it->empty()) return;
            int offset = block<T>::position(bl_it);

            auto next_it = mp_it;
            ++next_it;
            if (next_it != _blocks.end() && mp_it->size() + next_it->size() <= ideal_block_length) {
                for (size_t i = 0; i < next_it->size(); ++i) {
                    mp_it->push_back((*next_it)[i]);
                }
                _blocks.erase(next_it);
                if (offset > static_cast<int>(mp_it->size())) offset = static_cast<int>(mp_it->size());
                bl_it = mp_it->begin() + offset;
                return;
            }
            if (mp_it != _blocks.begin()) {
                auto prev_it = mp_it;
                --prev_it;
                if (prev_it->size() + mp_it->size() <= ideal_block_length) {
                    int old_prev_size = prev_it->size();
                    for (size_t i = 0; i < mp_it->size(); ++i) {
                        prev_it->push_back((*mp_it)[i]);
                    }
                    auto to_delete = mp_it;
                    mp_it = prev_it;
                    bl_it = mp_it->begin() + old_prev_size + offset;
                    _blocks.erase(to_delete);
                    if (bl_it == mp_it->end()) {
                        auto nxt = mp_it;
                        ++nxt;
                        if (nxt != _blocks.end()) {
                            mp_it = nxt;
                            bl_it = mp_it->begin();
                        }
                    }
                    return;
                }
            }
            if (bl_it == mp_it->end()) {
                auto nxt = mp_it;
                ++nxt;
                if (nxt != _blocks.end()) {
                    mp_it = nxt;
                    bl_it = mp_it->begin();
                }
            }
        }

        void add(iterator &pos) {
            ++length;
            if ((ideal_block_length + 1) * (ideal_block_length + 1) <= length) {
                ideal_block_length = std::max(blockMinor, ideal_block_length + 1);
            }
            check(pos);
        }

        void add() {
            ++length;
            if ((ideal_block_length + 1) * (ideal_block_length + 1) <= length) {
                ideal_block_length = std::max(blockMinor, ideal_block_length + 1);
            }
        }

        void minor(iterator &pos) {
            --length;
            if (ideal_block_length * ideal_block_length > length) {
                ideal_block_length = std::max(blockMinor, ideal_block_length - 1);
            }
            conquer(pos);
        }

        void minor() {
            --length;
            if (ideal_block_length * ideal_block_length > length) {
                ideal_block_length = std::max(blockMinor, ideal_block_length - 1);
            }
        }

    public:
        friend class iterator;
        friend class const_iterator;

        class iterator {
            friend class deque;
        private:
            /**
             * includes three components
             * container pointer
             * the pointer to where the block rests in the double_list:mp_it
             * the pointer to where the data rests in the corresponding block:bl_it
             */


        public:
            BlockT_list *contain_it;
            BlockT_list_iterator mp_it;
            Block_iterator bl_it;

            iterator() = default;

            iterator(BlockT_list *a, const BlockT_list_iterator &b, const Block_iterator &c)
            : contain_it(a), mp_it(b), bl_it(c) {}

            iterator(const iterator &other) : mp_it(other.mp_it), bl_it(other.bl_it), contain_it(other.contain_it) {
            }

            ~iterator() = default;

            /**
             * return a new iterator which points to the n-next element.
             * if there are not enough elements, the behaviour is undefined.
             * same for operator-.
             */
            iterator operator+(const int &n) const {
                iterator tmp = *this;

                return tmp += n;
            }

            iterator operator-(const int &n) const {
                iterator tmp = *this;
                return tmp -= n;
            }

            /**
             * return the distance between two iterators.
             * if they point to different vectors, throw
             * invaild_iterator.
             */
            int operator-(const iterator &rhs) const {
                if (rhs.contain_it != contain_it)throw std::invalid_argument("iterator-iterator");
                if (mp_it == rhs.mp_it) {
                    return (bl_it - rhs.bl_it);
                }
                int dist= 0;
                auto it = rhs.mp_it;
                dist += ((*it).end() - rhs.bl_it);
                ++it;
                while (it != contain_it->end() && it != mp_it) {
                    dist += (*it).size();
                    ++it;
                }

                if (it == mp_it) {
                    dist += (bl_it - (*it).begin());
                    return dist;
                }
                dist = 0;
                it = mp_it;
                dist += ((*it).end() - bl_it);
                ++it;
                while (it != contain_it->end() && it != rhs.mp_it) {
                    dist += (*it).size();
                    ++it;
                }
                dist += (rhs.bl_it - (*it).begin());
                return -dist;
            }

            iterator &operator+=(const int &n) {
                if (n==0) return *this;
                if (n < 0) return *this -= (-n);
                int step = n;
                while (step > 0) {
                    int remain = (*mp_it).end() - bl_it;
                    if (step < remain) {
                        bl_it = bl_it + step;
                        return *this;
                    }
                    step -= remain;
                    auto nxt = mp_it;
                    ++nxt;
                    if (nxt == contain_it->end()) {
                        bl_it = (*mp_it).end();
                        return *this;
                    }
                    mp_it = nxt;
                    bl_it = (*mp_it).begin();
                }
                return *this;
            }

            iterator &operator-=(const int &n) {
                if (n==0) return *this;
                if (n < 0) return *this += (-n);
                int step = n;
                while (step > 0) {
                    int left = bl_it - (*mp_it).begin();
                    if (step <= left) {
                        bl_it = bl_it - step;
                        return *this;
                    }
                    step -= left;
                    if (mp_it == contain_it->begin()) {
                        bl_it = (*mp_it).begin();
                        return *this;
                    }
                    --mp_it;
                    bl_it = (*mp_it).end();
                }
                return *this;
            }

            /**
             * iter++
             */
            iterator operator++(int) {
                iterator tmp = *this;
                ++*this;
                return tmp;
            }

            /**
             * ++iter
             */
            iterator &operator++() {
                if (contain_it == nullptr) throw invalid_iterator("++iterator");
                auto last = contain_it->end();
                --last;
                if (mp_it == last && bl_it == (*mp_it).end()) throw invalid_iterator("++iterator");
                ++bl_it;
                if (bl_it == (*mp_it).end()) {
                    BlockT_list_iterator next_mp = mp_it;
                    ++next_mp;
                    if (next_mp != contain_it->end()) {
                        mp_it = next_mp;
                        bl_it = (*mp_it).begin();
                    }
                }
                return *this;
            }

            /**
             * iter--
             */
            iterator operator--(int) {
                iterator tmp = *this;
                --(*this);
                return tmp;
            }

            /**
             * --iter
             */
            iterator &operator--() {
                if (contain_it == nullptr) throw invalid_iterator("--iterator");
                if (mp_it == contain_it->begin() && bl_it == (*mp_it).begin()) throw invalid_iterator("--iterator");
                if (bl_it == (*mp_it).begin()) {
                    --mp_it;
                    bl_it = (*mp_it).end();
                }
                --bl_it;
                return *this;
            }

            /**
             * *it
             */
            T &operator*() const {
                if (contain_it == nullptr) throw invalid_iterator("*iterator");
                auto last = contain_it->end();
                --last;
                if (mp_it == last && bl_it == (*mp_it).end()) throw invalid_iterator("*iterator");
                return *bl_it;
            }

            /**
             * it->field
             */
            T *operator->() const {
                if (contain_it == nullptr) throw invalid_iterator("->iterator");
                auto last = contain_it->end();
                --last;
                if (mp_it == last && bl_it == (*mp_it).end()) throw invalid_iterator("->iterator");
                return bl_it.operator->();
            }

            /**
             * check whether two iterators are the same (pointing to the same
             * memory).
             */
            bool operator==(const iterator &rhs) const {
                return (contain_it == rhs.contain_it) && (bl_it == rhs.bl_it) && (mp_it == rhs.mp_it);
            }

            bool operator==(const const_iterator &rhs) const {
                return (contain_it == rhs.contain_it) && (bl_it == rhs.bl_it) && (mp_it == rhs.mp_it);
            }

            /**
             * some other operator for iterators.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }

            operator const_iterator() const {
                return const_iterator(*this);
            }
        };

        class const_iterator {
        friend class deque;
        private:
            /**
             * includes three components
             * container pointer
             * the pointer to where the block rests in the double_list:mp_it
             * the pointer to where the data rests in the corresponding block:bl_it
             */


        public:
            BlockT_list *contain_it;
            BlockT_list_iterator mp_it;
            Block_iterator bl_it;

            const_iterator() = default;

            const_iterator(BlockT_list *a, const BlockT_list_iterator &b, const Block_iterator &c)
            : contain_it(a), mp_it(b), bl_it(c) {}

            const_iterator(const const_iterator &other) : mp_it(other.mp_it), bl_it(other.bl_it), contain_it(other.contain_it) {
            }

            const_iterator(const iterator &other) : mp_it(other.mp_it), bl_it(other.bl_it), contain_it(other.contain_it) {
            }

            ~const_iterator() = default;

            /**
             * return a new iterator which points to the n-next element.
             * if there are not enough elements, the behaviour is undefined.
             * same for operator-.
             */
            const_iterator operator+(const int &n) const {
                const_iterator tmp = *this;
                tmp += n;
                return tmp;
            }

            const_iterator operator-(const int &n) const {
                const_iterator tmp = *this;
                tmp -= n;
                return tmp;
            }

            /**
             * return the distance between two iterators.
             * if they point to different vectors, throw
             * invaild_iterator.
             */
            int operator-(const const_iterator &rhs) const {
                if (rhs.contain_it != contain_it)throw std::invalid_argument("const_iterator-const_iterator");
                if (mp_it == rhs.mp_it) {
                    return (bl_it - rhs.bl_it);
                }
                int dist= 0;
                auto it = rhs.mp_it;
                dist += ((*it).end() - rhs.bl_it);
                ++it;
                while (it != contain_it->end() && it != mp_it) {
                    dist += (*it).size();
                    ++it;
                }

                if (it == mp_it) {
                    dist += (bl_it - (*it).begin());
                    return dist;
                }
                dist = 0;
                it = mp_it;
                dist += ((*it).end() - bl_it);
                ++it;
                while (it != contain_it->end() && it != rhs.mp_it) {
                    dist += (*it).size();
                    ++it;
                }
                dist += (rhs.bl_it - (*it).begin());
                return -dist;
            }

            const_iterator &operator+=(const int &n) {
                if (n==0) return *this;
                if (n < 0) return *this -= (-n);
                int step = n;
                while (step > 0) {
                    int remain = (*mp_it).end() - bl_it;
                    if (step < remain) {
                        bl_it = bl_it + step;
                        return *this;
                    }
                    step -= remain;
                    auto nxt = mp_it;
                    ++nxt;
                    if (nxt == contain_it->end()) {
                        bl_it = (*mp_it).end();
                        return *this;
                    }
                    mp_it = nxt;
                    bl_it = (*mp_it).begin();
                }
                return *this;
            }

            const_iterator &operator-=(const int &n) {
                if (n==0) return *this;
                if (n < 0) return *this += (-n);
                int step = n;
                while (step > 0) {
                    int left = bl_it - (*mp_it).begin();
                    if (step <= left) {
                        bl_it = bl_it - step;
                        return *this;
                    }
                    step -= left;
                    if (mp_it == contain_it->begin()) {
                        bl_it = (*mp_it).begin();
                        return *this;
                    }
                    --mp_it;
                    bl_it = (*mp_it).end();
                }
                return *this;
            }

            /**
             * iter++
             */
            const_iterator operator++(int) {
                const_iterator tmp = *this;
                ++*this;
                return tmp;
            }

            /**
             * ++iter
             */
            const_iterator &operator++() {
                if (contain_it == nullptr) throw invalid_iterator("++const_iterator");
                auto last = contain_it->end();
                --last;
                if (mp_it == last && bl_it == (*mp_it).end()) throw invalid_iterator("++const_iterator");
                ++bl_it;
                if (bl_it == (*mp_it).end()) {
                    BlockT_list_iterator next_mp = mp_it;
                    ++next_mp;
                    if (next_mp != contain_it->end()) {
                        mp_it = next_mp;
                        bl_it = (*mp_it).begin();
                    }
                }
                return *this;
            }

            /**
             * iter--
             */
            const_iterator operator--(int) {
                const_iterator tmp = *this;
                --(*this);
                return tmp;
            }

            /**
             * --iter
             */
            const_iterator &operator--() {
                if (contain_it == nullptr) throw invalid_iterator("--const_iterator");
                if (mp_it == contain_it->begin() && bl_it == (*mp_it).begin()) throw invalid_iterator("--const_iterator");
                if (bl_it == (*mp_it).begin()) {
                    --mp_it;
                    bl_it = (*mp_it).end();
                }
                --bl_it;
                return *this;
            }

            /**
             * *it
             */
            T &operator*() const {
                if (contain_it == nullptr) throw invalid_iterator("*const_iterator");
                auto last = contain_it->end();
                --last;
                if (mp_it == last && bl_it == (*mp_it).end()) throw invalid_iterator("*const_iterator");
                return *bl_it;
            }

            /**
             * it->field
             */
            T *operator->() const {
                if (contain_it == nullptr) throw invalid_iterator("->const_iterator");
                auto last = contain_it->end();
                --last;
                if (mp_it == last && bl_it == (*mp_it).end()) throw invalid_iterator("->const_iterator");
                return bl_it.operator->();
            }

            /**
             * check whether two iterators are the same (pointing to the same
             * memory).
             */
            bool operator==(const iterator &rhs) const {
                return (contain_it == rhs.contain_it) && (bl_it == rhs.bl_it) && (mp_it == rhs.mp_it);
            }

            bool operator==(const const_iterator &rhs) const {
                return (contain_it == rhs.contain_it) && (bl_it == rhs.bl_it) && (mp_it == rhs.mp_it);
            }

            /**
             * some other operator for iterators.
             */
            bool operator!=(const iterator &rhs) const {
                return !(*this == rhs);
            }

            bool operator!=(const const_iterator &rhs) const {
                return !(*this == rhs);
            }
        };

        /**
         * constructors.
         */
        deque() : blockMinor(4096 / sizeof(T)), length(0), ideal_block_length(4096 / sizeof(T)) {
            if (blockMinor == 0) blockMinor = 1;
            if (ideal_block_length == 0) ideal_block_length = 1;
            _blocks.insert_tail(block<T>(ideal_block_length));
        };

        deque(const deque &other) : _blocks(other._blocks), length(other.length), ideal_block_length(other.ideal_block_length) {}

        /**
         * deconstructor.
         */
        ~deque() {
            clear();
        }

        /**
         * assignment operator.
         */
        deque &operator=(const deque &other) {
            if (&other==this)return *this;
            deque tmp(other);
            _blocks.swap(tmp._blocks);
            size_t t1 = blockMinor;
            blockMinor = tmp.blockMinor;
            tmp.blockMinor = t1;
            size_t t2 = length;
            length = tmp.length;
            tmp.length = t2;
            size_t t3 = ideal_block_length;
            ideal_block_length = tmp.ideal_block_length;
            tmp.ideal_block_length = t3;
            return *this;
        }

        /**
         * access a specified element with bound checking.
         * throw index_out_of_bound if out of bound.
         */
        T &at(const size_t &pos) {
            size_t cnt = 0;
            if (pos >= length)throw index_out_of_bound("deque::at");
            BlockT_list_iterator i = _blocks.begin();
            while (i != _blocks.end() && cnt + i->size() <= pos) {
                cnt += i->size();
                ++i;
            }
            return (*i)[pos - cnt];
        }

        const T &at(const size_t &pos) const {
            size_t cnt = 0;
            if (pos >= length)throw index_out_of_bound("deque::at");
            BlockT_list_citerator i = _blocks.begin();
            while (i != _blocks.end() && cnt + i->size() <= pos) {
                cnt += i->size();
                ++i;
            }
            return (*i)[pos - cnt];
        }

        T &operator[](const size_t &pos) { return at(pos); }

        const T &operator[](const size_t &pos) const { return at(pos); }

        /**
         * access the first element.
         * throw container_is_empty when the container is empty.
         */
        const T &front() const {
            if (empty())throw container_is_empty("front");
            return _blocks.begin()->front();
        }

        /**
         * access the last element.
         * throw container_is_empty when the container is empty.
         */
        const T &back() const {
            if (empty()) throw container_is_empty("back");
            auto it = _blocks.end();
            --it;
            return it->back();
        }

        /**
         * return an const_iterator to the beginning.
         */
        iterator begin() {
            if (empty()) return end();
            return iterator(&_blocks, _blocks.begin(), _blocks.begin()->begin());
        }

        const_iterator cbegin() const {
            if (empty()) return cend();
            BlockT_list *ptr = const_cast<BlockT_list *>(&_blocks);
            return const_iterator(ptr, ptr->begin(), ptr->begin()->begin());
        }

        const_iterator begin() const {
            return cbegin();
        }

        /**
         * return an const_iterator to the end.
         */
        iterator end() {
            auto it = _blocks.end();
            --it;
            return iterator(&_blocks, it, it->end());
        }

        const_iterator cend() const {
            BlockT_list *ptr = const_cast<BlockT_list *>(&_blocks);
            auto it = ptr->end();
            --it;
            return const_iterator(ptr, it, it->end());
        }

        const_iterator end() const {
            return cend();
        }

        const_iterator cend() {
            auto it = _blocks.end();
            --it;
            return const_iterator(&_blocks, it, it->end());
        }

        /**
         * check whether the container is empty.
         */
        [[nodiscard]] bool empty() const {
            return length == 0;
        }

        /**
         * return the number of elements.
         */
        [[nodiscard]] size_t size() const {
            return length;
        }

        /**
         * clear all contents.
         */
        void clear() {
            _blocks.clear();
            length = 0;
            ideal_block_length = blockMinor;
            _blocks.insert_tail(block<T>(ideal_block_length));
        }

        /**
         * insert value before pos.
         * return an iterator pointing to the inserted value.
         * throw if the iterator is invalid or it points to a wrong place.
         */
        iterator insert(iterator pos, const T &value) {
            if (pos.contain_it != &_blocks) throw invalid_iterator("insert");
            add(pos);
            if (!pos.mp_it->full()) {
                int p = block<T>::position(pos.bl_it);
                pos.bl_it = pos.mp_it->insert(p, value);
                return pos;
            }

            auto next_it = pos.mp_it;
            ++next_it;
            if (next_it == _blocks.end() || next_it->full()) {
                _blocks.insert(next_it, block<T>(ideal_block_length));
                next_it = pos.mp_it;
                ++next_it;
            }
            int p = block<T>::position(pos.bl_it);
            if (p == static_cast<int>(pos.mp_it->size())) {
                next_it->push_front(value);
                pos.mp_it = next_it;
                pos.bl_it = pos.mp_it->begin();
                return pos;
            }
            T tmp = pos.mp_it->insert_full(p, value);
            next_it->push_front(tmp);
            return pos;
        }

        /**
         * remove the element at pos.
         * return an iterator pointing to the following element. if pos points to
         * the last element, return end(). throw if the container is empty,
         * the iterator is invalid, or it points to a wrong place.
         */
        iterator erase(iterator pos) {
            if (pos.contain_it != &_blocks) throw invalid_iterator("erase");
            if (empty()) throw container_is_empty("erase");
            if (pos.mp_it == _blocks.end()) throw invalid_iterator("erase");
            int p = block<T>::position(pos.bl_it);
            if (p < 0 || p >= static_cast<int>(pos.mp_it->size())) throw invalid_iterator("erase");
            pos.bl_it = pos.mp_it->erase(p);
            minor(pos);
            if (length == 0) {
                clear();
                return end();
            }
            if (pos.mp_it->empty() && _blocks.length() > 1) {
                auto next_it = pos.mp_it;
                ++next_it;
                _blocks.erase(pos.mp_it);
                if (next_it != _blocks.end()) {
                    pos.mp_it = next_it;
                    pos.bl_it = pos.mp_it->begin();
                } else {
                    return end();
                }
            }
            if (pos.bl_it == pos.mp_it->end()) {
                auto next_it = pos.mp_it;
                ++next_it;
                if (next_it != _blocks.end()) {
                    pos.mp_it = next_it;
                    pos.bl_it = pos.mp_it->begin();
                }
            }
            return pos;
        }

        /**
         * add an element to the end.
         */
        void push_back(const T &value) {
            add();
            auto pos_mp = _blocks.end();
            --pos_mp;
            if (pos_mp->full()) {
                _blocks.insert_tail(block<T>(ideal_block_length));
                pos_mp = _blocks.end();
                --pos_mp;
            }
            pos_mp->push_back(value);
        }

        /**
         * remove the last element.
         * throw when the container is empty.
         */
        void pop_back() {
            if (empty()) throw container_is_empty("pop_back");
            auto pos_mp = _blocks.end();
            --pos_mp;
            pos_mp->pop_back();
            minor();
            if (length == 0) {
                clear();
                return;
            }
            if (pos_mp->empty() && _blocks.length() > 1) {
                _blocks.delete_tail();
            }
        }
        /**
         * insert an element to the beginning.
         */
        void push_front(const T &value) {
            add();
            auto pos_mp = _blocks.begin();
            if (pos_mp->full()) {
                _blocks.insert_head(block<T>(ideal_block_length));
                pos_mp = _blocks.begin();
            }
            pos_mp->push_front(value);
        }

        /**
         * remove the first element.
         * throw when the container is empty.
         */
        void pop_front() {
            if (empty()) throw container_is_empty("pop_front");
            auto pos_mp = _blocks.begin();
            pos_mp->pop_front();
            minor();
            if (length == 0) {
                clear();
                return;
            }
            if (pos_mp->empty() && _blocks.length() > 1) {
                _blocks.delete_head();
            }
        }
    };
} // namespace sjtu

#endif
