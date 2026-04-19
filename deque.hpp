#ifndef SJTU_DEQUE_HPP
#define SJTU_DEQUE_HPP

#include "exceptions.hpp"

#include <cstddef>

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
                if (is_end_atom(it)) throw sjtu::invalid_iterator("it++");
                if (it != nullptr) {
                    it = it->next;
                    return tmp;
                }
                throw sjtu::invalid_iterator("it++");
            }

            /**
             * ++iter
             */
            iterator &operator++() {
                if (is_end_atom(it)) throw sjtu::invalid_iterator("++it");
                if (it != nullptr) {
                    it = it->next;
                    return *this;
                }
                throw sjtu::invalid_iterator("++it");
            }

            /**
             * iter--
             */
            iterator operator--(int) {
                iterator tmp = *this;
                if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator("it--");
                if (it != nullptr) {
                    it = it->pre;
                    return tmp;
                }
                throw sjtu::invalid_iterator("it--");
            }

            /**
             * --iter
             */
            iterator &operator--() {
                if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator("--it");
                if (it != nullptr) {
                    it = it->pre;
                    return *this;
                }
                throw sjtu::invalid_iterator("--it");
            }

            /**
             * if the iter didn't point to a value
             * throw " invalid"
             */
            T &operator*() const {
                if (it != nullptr && !is_end_atom(it)) return *(it->data);
                throw sjtu::invalid_iterator("*it");
            }

            /**
             * other operation
             */
            T *operator->() const {
                if (it != nullptr && !is_end_atom(it)) return it->data;
                throw sjtu::invalid_iterator("->it");
            }

            bool operator==(const iterator &rhs) const {
                return rhs.it == it;
            }

            bool operator!=(const iterator &rhs) const {
                return rhs.it != it;
            }

            bool operator==(const const_iterator &rhs) const { return it == rhs.it; }
            bool operator!=(const const_iterator &rhs) const { return it != rhs.it; }
            [[nodiscard]]bool validity_check() const {
                return double_list<T>::validity_check(it);
            }
            operator const_iterator() const{
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
                if (is_end_atom(it)) throw sjtu::invalid_iterator("it++");
                if (it != nullptr) {
                    it = it->next;
                    return tmp;
                }
                throw sjtu::invalid_iterator("it++");
            }

            const_iterator &operator++() {
                if (is_end_atom(it)) throw sjtu::invalid_iterator("++it");
                if (it != nullptr) {
                    it = it->next;
                    return *this;
                }
                throw sjtu::invalid_iterator("++it");
            }

            const_iterator operator--(int) {
                const_iterator tmp = *this;
                if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator("it--");
                if (it != nullptr) {
                    it = it->pre;
                    return tmp;
                }
                throw sjtu::invalid_iterator("it--");
            }

            const_iterator &operator--() {
                if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator("--it");
                if (it != nullptr) {
                    it = it->pre;
                    return *this;
                }
                throw sjtu::invalid_iterator("--it");
            }

            const T &operator*() const {
                if (it != nullptr && !is_end_atom(it)) return *(it->data);
                throw sjtu::invalid_iterator("*cit");
            }

            const T *operator->() const {
                if (it != nullptr && !is_end_atom(it)) return it->data;
                throw sjtu::invalid_iterator("->cit");
            }

            bool operator==(const const_iterator &rhs) const {
                return rhs.it == it;
            }

            bool operator!=(const const_iterator &rhs) const {
                return rhs.it != it;
            }

            bool operator==(const iterator &rhs) const { return it == rhs.it; }
            bool operator!=(const iterator &rhs) const { return it != rhs.it; }
            [[nodiscard]]bool validity_check() const {
                return double_list<T>::validity_check(it);
            }
        };

        /**
         * return an iterator to the beginning
         */
        iterator begin() {
            return iterator(begin_atom);
        }

        /**
         * return an iterator to the ending
         * in fact, it returns the iterator point to nothing,
         * just after the last element.
         */
        iterator end() {
            return iterator(end_atom);
        }

        const_iterator begin() const {
            return const_iterator(begin_atom);
        }

        const_iterator end() const {
            return const_iterator(end_atom);
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
            if (!is_end_atom(tmp)) return iterator(tmp);
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

        void delete_head() {
            erase(iterator(begin_atom));
        }

        void delete_tail() {
            erase(iterator(end_atom->pre));
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

        [[nodiscard]] static  bool validity_check(const const_iterator &it) const {
            if (it.contain_it == nullptr)return false;
            iterator tmp = it.contain_it->begin();
            while (tmp != it.contain_it->end()) {
                if (tmp == it)return true;
                ++tmp;
            }
            return false;
        }
    };


    template<class T>
    class block {
        size_t block_size, block_volume;
        T *_rear, _front, _base;
        /**
         *check whether the iterator is valid
         */


    public:
        static bool validity_check(const T *_data, const block *_block) {
            if (!(_data >= _block->_base && _data < (_block->_base + _block->block_volume)))return false;
            if (_block->_front <= _block->_rear) {
                if (_data <= _block->_rear && _data >= _block->_front)return true;
                return false;
            } else {
                if (_data <= _block->_rear || _data >= _block->_front)return true;
                return false;
            }
        }

        class const_iterator;

        class iterator {
        public:
            T *_data;
            block *_block;

            [[nodiscard]] bool validity_check() const {
                return block::validity_check(_data, _block);
            }

            iterator() : _data(nullptr), _block(nullptr) {
            }

            iterator(T *dat, block *blo) : _data(dat), _block(blo) {
            }

            iterator(const iterator &other) : _data(other._data), _block(other._block) {
            };

            ~iterator() = default;

            iterator &operator++() {
                if (!validity_check())throw sjtu::invalid_iterator("operator++");
                _data = _block->_base + (_data - _block->_base + 1 + _block->block_volume) % _block->block_volume;
                return *this;
            }

            iterator &operator+(const int &n) {
                if (!validity_check())throw sjtu::invalid_iterator("operator+n");
                _data = _block->_base + (_data - _block->_base + n + _block->block_volume) % _block->block_volume;
                return *this;
            }

            iterator &operator--() {
                if (!validity_check())throw sjtu::invalid_iterator("operator--");
                if (_data == _block->_front)return _block->begin();
                _data = _block->_base + (_data - _block->_base - 1 + _block->block_volume) % _block->block_volume;
                return *this;
            }

            iterator &operator-(const int &n) {
                if (!validity_check())throw sjtu::invalid_iterator("operator-n");
                _data = _block->_base + (_data - _block->_base - n + _block->block_volume) % _block->block_volume;
                return *this;
            }

            iterator &operator=(const iterator &other) {
                if (&other == this)return *this;
                _data = other._data;
                _block = other._block;
                return *this;
            }

            /**
             * compare functions
             */
            [[nodiscard]] bool operator==(const iterator &rhs) {
                return _data == rhs._data && _block == rhs._block;
            }

            [[nodiscard]] bool operator!=(const iterator &rhs) {
                return !(*this == rhs);
            }

            [[nodiscard]] bool operator==(const_iterator &rhs) {
                return _data == rhs._data && _block == rhs._block;
            }

            [[nodiscard]] bool operator!=(const_iterator &rhs) {
                return !(*this == rhs);
            }


            T *operator->() const {
                if (!validity_check())throw sjtu::invalid_iterator("operator->");
                return _data;
            }

            T &operator*() const {
                if (!validity_check())throw sjtu::invalid_iterator("operator->");
                return *_data;
            }
            operator const_iterator() const{
                return const_iterator(*this);
            }
        };

        class const_iterator {
        public:
            T *_data;
            block *_block;

            [[nodiscard]] bool validity_check() const {
                return block::validity_check(_data, _block);
            }

            const_iterator() : _data(nullptr), _block(nullptr) {
            }

            const_iterator(T *dat, block *blo) : _data(dat), _block(blo) {
            }

            const_iterator(const iterator &other) : _data(other._data), _block(other._block) {
            };

            const_iterator(const_iterator &other) : _data(other._data), _block(other._block) {
            };

            ~const_iterator() = default;

            const_iterator &operator++() {
                if (!validity_check(_data, _block))throw sjtu::invalid_iterator("operator++");
                _data = _block->_base + (_data - _block->_base + 1 + _block->block_volume) % _block->block_volume;
                return *this;
            }

            const_iterator &operator--() {
                if (!validity_check(_data, _block))throw sjtu::invalid_iterator("operator--");
                if (_data == _block->_front)return _block->begin();
                _data = _block->_base + (_data - _block->_base - 1 + _block->block_volume) % _block->block_volume;
                return *this;
            }

            const_iterator &operator=(const_iterator &other) {
                if (&other == this)return *this;
                _data = other._data;
                _block = other._block;
                return *this;
            }

            /**
             * compare functions
             */
            [[nodiscard]] bool operator==(const iterator &rhs) {
                return _data == rhs._data && _block == rhs._block;
            }

            [[nodiscard]] bool operator!=(const iterator &rhs) {
                return !(*this == rhs);
            }

            [[nodiscard]] bool operator==(const_iterator &rhs) {
                return _data == rhs._data && _block == rhs._block;
            }

            [[nodiscard]] bool operator!=(const_iterator &rhs) {
                return !(*this == rhs);
            }


            const T *operator->() const {
                if (!validity_check(_data, _block))throw sjtu::invalid_iterator("operator->");
                return _data;
            }

            const T &operator*() const {
                if (!validity_check(_data, _block))throw sjtu::invalid_iterator("operator->");
                return *_data;
            }
        };


        block() : block_size(0), block_volume(100) {
            _base = _rear = _front = new T[block_volume];
        }

        block(size_t volume) : block_size(0), block_volume(volume) {
            _base = _rear = _front = new T[block_volume];
        }

        ~block() {
            delete[] _base;
        }


        /*
         *state access
        */

        [[nodiscard]] size_t size() const {
            return block_size;
        }

        [[nodiscard]] bool full() const {
            return block_size == block_volume;
        }

        [[nodiscard]] bool empty() const {
            return block_size == 0;
        }


        [[nodiscard]] static size_t position(const iterator &it) {
            if (!validity_check(it._data, it._block))throw sjtu::invalid_iterator("position_check");
            if (it._block->_front < it._block->_rear)throw sjtu::invalid_iterator("position_check");
            if (it._block->_front < it._block->_rear)return it._data - it._block->_front;
            else {
                if (it._block->_front < it._data)return it._data - it._block->_front;
                else if (it._block->_rear > it._data)return it._block->block_size - (it._block->_rear - it._data - 1);
                throw sjtu::runtime_error("positionfail");
            }
        }

        //
        void push_back(const T &data) {
            if (full())throw sjtu::index_out_of_bound("push_back out of block");
            *_rear = data;
            _rear = (_rear - _base + 1 + block_volume) % block_volume + _base;
            ++block_size;
        }

        void push_front(const T &data) {
            if (full())throw sjtu::index_out_of_bound("push_front out of block");
            _front = (_front - _base - 1 + block_volume) % block_volume + _base;
            *_front = data;
            ++block_size;
        }

        void pop_front() {
            if (empty())throw sjtu::container_is_empty("pop_front an empty block");
            _front = (_front - _base + 1) % block_volume + _base;
            --block_size;
        }

        void pop_back() {
            if (empty())throw sjtu::container_is_empty("pop_back an empty block");
            _rear = (_rear - _base - 1) % block_volume + _base;
            --block_size;
        }


        //random access
        T &operator[](size_t pos) {
            if (pos >= block_size)throw sjtu::index_out_of_bound("operator[]");
            return *(_base + pos % block_volume + _base);
        }

        T &at(size_t pos) {
            if (pos >= block_size)throw sjtu::index_out_of_bound("operator[]");
            return *(_base + pos % block_volume + _base);
        }

        //begin&end
        iterator begin() const {
            return iterator(_front, this);
        }

        iterator end() const {
            return iterator(_rear, this);
        }

        const_iterator begin() {
            return const_iterator(_front, this);
        }

        const_iterator cbegin() const {
            return const_iterator(_front, this);
        }

        const_iterator end() {
            return const_iterator(_rear, this);
        }

        const_iterator cend() const {
            return const_iterator(_rear, this);
        }


        T &front() {
            if (empty())throw sjtu::container_is_empty("block.front");
            return *_front;
        }

        T &back() {
            if (empty())throw sjtu::container_is_empty("block.back");
            return *(_rear - 1);
        }
    };


    template<class T>
    class deque {
    public:
        typedef double_list<block<T> *> BlockT_list;
        typedef typename double_list<block<T> *>::iterator BlockT_list_iterator;
        typedef typename double_list<block<T> *>::const_iterator BlockT_list_citerator;
        typedef typename block<T>::iterator Block_iterator;
    private:
        /*Components
         *length,ideal_length
         *double_list contains the blocks
         */
        BlockT_list _blocks;
        size_t length, ideal_block_length;

    public:
        class const_iterator;
        friend class iterator;
        friend class const_iterator;

        class iterator {
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

            iterator(const BlockT_list &a, const BlockT_list_iterator &b, const Block_iterator &c) : mp_it(a), bl_it(b),
                contain_it(c) {
            }

            iterator(const iterator &other) : mp_it(other.mp_it), bl_it(other.bl_it), contain_it(other.contain_it) {
            }

            ~iterator() = default;

            /**
             * return a new iterator which points to the n-next element.
             * if there are not enough elements, the behaviour is undefined.
             * same for operator-.
             */
            iterator operator+(const int &n) const {
                iterator r = *this;
                size_t pos = block<T>::position(r.bl_it), len = r.bl_it._block->block_size;
                if (len - pos - 1 >= n) {
                    r.bl_it = r.bl_it + n;
                    return r;
                }
                int cnt = n;
                size_t length1;
                cnt -= len - 1 - pos;
                while (r.mp_it != r.contain_it->end()) {
                    ++r.mp_it;
                    length1 = r.mp_it._block->block_size;
                    if (cnt <= length1) {
                        r.bl_it = *(r.mp_it)[cnt - 1];
                        return r;
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            iterator operator-(const int &n) const {
                iterator r = *this;
                size_t pos = block<T>::position(r.bl_it);
                if (pos >= n) {
                    r.bl_it = r.bl_it - n;
                    return r;
                }
                size_t cnt = n, length1;
                cnt -= pos;
                while (r.mp_it != r.contain_it->begin()) {
                    --r.mp_it;
                    length1 = r.mp_it._block->block_size;
                    if (cnt <= length1) {
                        r.bl_it = *r.mp_it[length1 - cnt];
                        return r;
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            /**
             * return the distance between two iterators.
             * if they point to different vectors, throw
             * invaild_iterator.
             */
            int operator-(const iterator &rhs) const {
                if (rhs.contain_it != contain_it)throw sjtu::invalid_iterator("iterator::operator-");
                iterator r = *this;
                size_t num = r.bl_it._block->block_size - block<T>::position(r.bl_it) - 1;
                if (r.mp_it == rhs.mp_it)return block<T>::position(r.bl_it) - block<T>::position(rhs.bl_it);
                while (r.mp_it != contain_it->end()) {
                    ++r.mp_it;
                    r.bl_it = (*r.mp_it)->_front;
                    if (r.mp_it == rhs.mp_it)return num + block<T>::position(rhs.bl_it) + 1;
                    num += (*r.mp_it)->block_size;
                }
                r = *this;
                num = block<T>::position(r.bl_it);
                while (r.mp_it != contain_it->begin()) {
                    --r.mp_it;
                    r.bl_it = (*r.mp_it)->_front;
                    if (r.mp_it == rhs.mp_it)return num + rhs.bl_it._block->block_size - 1 - block<T>::position(
                                                        rhs.bl_it);
                    num += r.bl_it._block->block_size;
                }
                throw sjtu::invalid_iterator("iterator::operator-");
            }

            iterator &operator+=(const int &n) {
                size_t pos = block<T>::position((*this).bl_it), len = (*this).bl_it._block->block_size;
                if (len - pos - 1 >= n) {
                    (*this).bl_it = (*this).bl_it + n;
                    return (*this);
                }
                int cnt = n;
                size_t length1;
                cnt -= len - 1 - pos;
                while ((*this).mp_it != (*this).contain_it->end()) {
                    ++(*this).mp_it;
                    length1 = (*this).mp_it._block->block_size;
                    if (cnt <= length1) {
                        (*this).bl_it = *((*this).mp_it)[cnt - 1];
                        return (*this);
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            iterator &operator-=(const int &n) {
                size_t pos = block<T>::position((*this).bl_it);
                if (pos >= n) {
                    (*this).bl_it = (*this).bl_it - n;
                    return (*this);
                }
                size_t cnt = n, length1;
                cnt -= pos;
                while ((*this).mp_it != (*this).contain_it->begin()) {
                    --(*this).mp_it;
                    length1 = (*this).mp_it._block->block_size;
                    if (cnt <= length1) {
                        (*this).bl_it = *(*this).mp_it[length1 - cnt];
                        return (*this);
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            /**
             * iter++
             */
            iterator operator++(int) {
                return (*this + 1);
            }

            /**
             * ++iter
             */
            iterator &operator++() {
                return (*this += 1);
            }

            /**
             * iter--
             */
            iterator operator--(int) {
                return (*this - 1);
            }

            /**
             * --iter
             */
            iterator &operator--() {
                return (*this -= 1);
            }

            /**
             * *it
             */
            T &operator*() const {
                return *bl_it;
            }

            /**
             * it->field
             */
            T *operator->() const noexcept {
                return &(*bl_it);
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
            operator const_iterator() const{
                return const_iterator(*this);
            }
        };

        class const_iterator {
        private:
            /**
             * includes three components
             * container pointer
             * the pointer to where the block rests in the double_list:mp_it
             * the pointer to where the data rests in the corresponding block:bl_it
             */
            BlockT_list *contain_it;
            BlockT_list_iterator mp_it;
            Block_iterator bl_it;

        public:
            const_iterator() = default;

            const_iterator(const BlockT_list &a, const BlockT_list_iterator &b,
                           const Block_iterator &c) : mp_it(a), bl_it(b), contain_it(c) {
            }

            const_iterator(const const_iterator &other) : mp_it(other.mp_it), bl_it(other.bl_it),
                                                          contain_it(other.contain_it) {
            }

            ~const_iterator() = default;

            /**
             * return a new const_iterator which points to the n-next element.
             * if there are not enough elements, the behaviour is undefined.
             * same for operator-.
             */
            const_iterator operator+(const int &n) const {
                const_iterator r = *this;
                size_t pos = block<T>::position(r.bl_it), len = r.bl_it._block->block_size;
                if (len - pos - 1 >= n) {
                    r.bl_it = r.bl_it + n;
                    return r;
                }
                int cnt = n;
                size_t length1;
                cnt -= len - 1 - pos;
                while (r.mp_it != r.contain_it->end()) {
                    ++r.mp_it;
                    length1 = r.mp_it._block->block_size;
                    if (cnt <= length1) {
                        r.bl_it = *(r.mp_it)[cnt - 1];
                        return r;
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            const_iterator operator-(const int &n) const {
                const_iterator r = *this;
                size_t pos = block<T>::position(r.bl_it);
                if (pos >= n) {
                    r.bl_it = r.bl_it - n;
                    return r;
                }
                size_t cnt = n, length1;
                cnt -= pos;
                while (r.mp_it != r.contain_it->begin()) {
                    --r.mp_it;
                    length1 = r.mp_it._block->block_size;
                    if (cnt <= length1) {
                        r.bl_it = *r.mp_it[length1 - cnt];
                        return r;
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            /**
             * return the distance between two iterators.
             * if they point to different vectors, throw
             * invaild_iterator.
             */
            int operator-(const const_iterator &rhs) const {
                if (rhs.contain_it != contain_it)throw sjtu::invalid_iterator("const_iterator::operator-");
                const_iterator r = *this;
                size_t num = r.bl_it._block->block_size - block<T>::position(r.bl_it) - 1;
                if (r.mp_it == rhs.mp_it)return block<T>::position(r.bl_it) - block<T>::position(rhs.bl_it);
                while (r.mp_it != contain_it->end()) {
                    ++r.mp_it;
                    r.bl_it = (*r.mp_it)->_front;
                    if (r.mp_it == rhs.mp_it)return num + block<T>::position(rhs.bl_it) + 1;
                    num += (*r.mp_it)->block_size;
                }
                r = *this;
                num = block<T>::position(r.bl_it);
                while (r.mp_it != contain_it->begin()) {
                    --r.mp_it;
                    r.bl_it = (*r.mp_it)->_front;
                    if (r.mp_it == rhs.mp_it)return num + rhs.bl_it._block->block_size - 1 - block<T>::position(
                                                        rhs.bl_it);
                    num += r.bl_it._block->block_size;
                }
                throw sjtu::invalid_iterator("const_iterator::operator-");
            }

            const_iterator &operator+=(const int &n) {
                size_t pos = block<T>::position((*this).bl_it), len = (*this).bl_it._block->block_size;
                if (len - pos - 1 >= n) {
                    (*this).bl_it = (*this).bl_it + n;
                    return (*this);
                }
                int cnt = n;
                size_t length1;
                cnt -= len - 1 - pos;
                while ((*this).mp_it != (*this).contain_it->end()) {
                    ++(*this).mp_it;
                    length1 = (*this).mp_it._block->block_size;
                    if (cnt <= length1) {
                        (*this).bl_it = *((*this).mp_it)[cnt - 1];
                        return (*this);
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            const_iterator &operator-=(const int &n) {
                size_t pos = block<T>::position((*this).bl_it);
                if (pos >= n) {
                    (*this).bl_it = (*this).bl_it - n;
                    return (*this);
                }
                size_t cnt = n, length1;
                cnt -= pos;
                while ((*this).mp_it != (*this).contain_it->begin()) {
                    --(*this).mp_it;
                    length1 = (*this).mp_it._block->block_size;
                    if (cnt <= length1) {
                        (*this).bl_it = *(*this).mp_it[length1 - cnt];
                        return (*this);
                    }
                }
                throw sjtu::runtime_error("position+n fail");
            }

            /**
             * iter++
             */
            const_iterator operator++(int) {
                return (*this + 1);
            }

            /**
             * ++iter
             */
            const_iterator &operator++() {
                return (*this += 1);
            }

            /**
             * iter--
             */
            const_iterator operator--(int) {
                return (*this - 1);
            }

            /**
             * --iter
             */
            const_iterator &operator--() {
                return (*this -= 1);
            }

            /**
             * *it
             */
            const T &operator*() const {
                return *bl_it;
            }

            /**
             * it->field
             */
            const T *operator->() const noexcept {
                return &(*bl_it);
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
        deque() = default;

        deque(const deque &other) : _blocks(double_list<block<T> *>(other._blocks)), length(other.length),
                                    ideal_block_length(other.ideal_block_length) {
        }

        /**
         * deconstructor.
         */
        ~deque() = default;

        /**
         * assignment operator.
         */
        deque &operator=(const deque &other) {
            _blocks = double_list<block<T> *>(other._blocks);
            length = other.length;
            ideal_block_length = other.ideal_block_length;
            return *this;
        }

        /**
         * access a specified element with bound checking.
         * throw index_out_of_bound if out of bound.
         */
        T &at(const size_t &pos) {
            size_t cnt = 0;
            if (_blocks.empty()) throw sjtu::index_out_of_bound("deque::at");
            if (pos >= length || pos < 0)throw sjtu::index_out_of_bound("deque::at");
            BlockT_list_iterator i = _blocks->begin();
            while (cnt + (*i)->block_size - 1 < pos) {
                cnt += (*i)->block_size;
                ++i;
            }
            return (*i)[pos - cnt];
        }

        const T &at(const size_t &pos) const {
            size_t cnt = 0;
            if (_blocks.empty()) throw sjtu::index_out_of_bound("deque::at");
            if (pos >= length || pos < 0)throw sjtu::index_out_of_bound("deque::at");
            BlockT_list_iterator i = _blocks->begin();
            while (cnt + (*i)->block_size - 1 < pos) {
                cnt += (*i)->block_size;
                ++i;
            }
            return (*i)[pos - cnt];
        }

        T &operator[](const size_t &pos) {
            try {
                return at(pos);
            } catch (const sjtu::index_out_of_bound &) {
                throw sjtu::index_out_of_bound("deque::[]");
            }
        }

        const T &operator[](const size_t &pos) const {
            try {
                return at(pos);
            } catch (sjtu::index_out_of_bound &) {
                throw sjtu::index_out_of_bound("deque::[] const");
            }
        }

        /**
         * access the first element.
         * throw container_is_empty when the container is empty.
         */
        const T &front() const {
            if (empty())throw sjtu::container_is_empty("front");
            return (*_blocks.begin())->front();
        }

        /**
         * access the last element.
         * throw container_is_empty when the container is empty.
         */
        const T &back() const {
            if ((*this).empty())throw sjtu::container_is_empty("back");
            return (*_blocks.end()--)->end();
        }

        /**
         * return an iterator to the beginning.
         */
        iterator begin() {
            if (empty())throw sjtu::container_is_empty("begin");
            return iterator(this, _blocks.begin(), (*_blocks.begin())->begin());
        }

        const_iterator cbegin() const {
            if (empty())throw sjtu::container_is_empty("begin");
            return const_iterator(this, _blocks.cbegin(), (*_blocks.cbegin())->cbegin());
        }

        /**
         * return an iterator to the end.
         */
        iterator end() {
            if (empty())throw sjtu::container_is_empty("begin");
            return iterator(this, _blocks.end(), nullptr);
        }

        const_iterator cend() const {
            if (empty())throw sjtu::container_is_empty("begin");
            return iterator(this, _blocks.cend(), nullptr);
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
        }

        /**
         * insert value before pos.
         * return an iterator pointing to the inserted value.
         * throw if the iterator is invalid or it points to a wrong place.
         */
        iterator insert(iterator pos, const T &value) {
            if (pos.contain_it != this)throw invalid_iterator("insert");
            auto i=pos.mp_it;
            if (!pos.mp_it.validity_check())throw invalid_iterator("insert");
            if (!pos.bl_it.validity_check())throw invalid_iterator("insert");
            if (!pos.bl_it._block->full())

        }

        /**
         * remove the element at pos.
         * return an iterator pointing to the following element. if pos points to
         * the last element, return end(). throw if the container is empty,
         * the iterator is invalid, or it points to a wrong place.
         */
        iterator erase(iterator pos) {
        }

        /**
         * add an element to the end.
         */
        void push_back(const T &value) {
        }

        /**
         * remove the last element.
         * throw when the container is empty.
         */
        void pop_back() {
        }

        /**
         * insert an element to the beginning.
         */
        void push_front(const T &value) {
        }

        /**
         * remove the first element.
         * throw when the container is empty.
         */
        void pop_front() {
        }
    };
} // namespace sjtu

#endif
