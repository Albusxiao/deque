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
        atom *it;
        // --------------------------
        /**
         * the follows are constructors and destructors
         * you can also add some if needed.
         */
        iterator() : it(nullptr) {
        }

        iterator(atom *t) : it(t) {
        }

        iterator(const iterator &t) : it(t.it) {
        }

        ~iterator() = default;

        /**
         * iter++
         */
        iterator operator++(int) {
            iterator tmp = *this;
            if (is_end_atom(it)) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->next;
                return tmp;
            }
            throw sjtu::invalid_iterator();
        }

        /**
         * ++iter
         */
        iterator &operator++() {
            if (is_end_atom(it)) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->next;
                return *this;
            }
            throw sjtu::invalid_iterator();
        }

        /**
         * iter--
         */
        iterator operator--(int) {
            iterator tmp = *this;
            if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->pre;
                return tmp;
            }
            throw sjtu::invalid_iterator();
        }

        /**
         * --iter
         */
        iterator &operator--() {
            if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->pre;
                return *this;
            }
            throw sjtu::invalid_iterator();
        }

        /**
         * if the iter didn't point to a value
         * throw " invalid"
         */
        T &operator*() const {
            if (it != nullptr && !is_end_atom(it)) return *(it->data);
            throw sjtu::invalid_iterator();
        }

        /**
         * other operation
         */
        T *operator->() const {
            if (it != nullptr && !is_end_atom(it)) return it->data;
            throw sjtu::invalid_iterator();
        }

        bool operator==(const iterator &rhs) const {
            return rhs.it == it;
        }

        bool operator!=(const iterator &rhs) const {
            return rhs.it != it;
        }

        bool operator==(const const_iterator &rhs) const { return it == rhs.it; }
        bool operator!=(const const_iterator &rhs) const { return it != rhs.it; }
    };

    class const_iterator {
    public:
        atom *it;

        const_iterator() : it(nullptr) {
        }

        const_iterator(atom *t) : it(t) {
        }
        const_iterator(const iterator &t): it(t.it) {
        }
        const_iterator(const const_iterator &t) : it(t.it) {
        }

        ~const_iterator() = default;

        const_iterator operator++(int) {
            const_iterator tmp = *this;
            if (is_end_atom(it)) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->next;
                return tmp;
            }
            throw sjtu::invalid_iterator();
        }

        const_iterator &operator++() {
            if (is_end_atom(it)) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->next;
                return *this;
            }
            throw sjtu::invalid_iterator();
        }

        const_iterator operator--(int) {
            const_iterator tmp = *this;
            if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->pre;
                return tmp;
            }
            throw sjtu::invalid_iterator();
        }

        const_iterator &operator--() {
            if (it != nullptr && it->pre == nullptr) throw sjtu::invalid_iterator();
            if (it != nullptr) {
                it = it->pre;
                return *this;
            }
            throw sjtu::invalid_iterator();
        }

        const T &operator*() const {
            if (it != nullptr && !is_end_atom(it)) return *(it->data);
            throw sjtu::invalid_iterator();
        }

        const T *operator->() const {
            if (it != nullptr && !is_end_atom(it)) return it->data;
            throw sjtu::invalid_iterator();
        }

        bool operator==(const const_iterator &rhs) const {
            return rhs.it == it;
        }

        bool operator!=(const const_iterator &rhs) const {
            return rhs.it != it;
        }

        bool operator==(const iterator &rhs) const { return it == rhs.it; }
        bool operator!=(const iterator &rhs) const { return it != rhs.it; }
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
};



template <class T>
class block {
    size_t block_size,block_volume;
    T* _rear,_front,_base;
public:
    block():block_size(0),block_volume(100) {
        _base=_rear=_front= new T[block_volume];
    }
    block(size_t volume):block_size(0),block_volume(volume) {
        _base=_rear=_front=new T[block_volume];
    }
    ~block() {
        delete[] _base;
    }

    bool full() {
        return block_size==block_volume;
    }
    bool empty() {
        return block_size==0;
    }
    void push_back(const T& data) {
        if (full())throw sjtu::index_out_of_bound("push_back out of block");
        *_rear=data;
        _rear=(_rear-_base+1+block_volume)%block_volume+_base;
        ++block_size;
    }
    void push_front(const T& data) {
        if (full())throw sjtu::index_out_of_bound("push_front out of block");
        _front=(_front-_base-1+block_volume)%block_volume+_base;
        *_front=data;
        ++block_size;
    }
    void pop_front() {
        if (empty())throw sjtu::container_is_empty("pop_front an empty block");
        _front=(_front-_base+1)%block_volume+_base;
        --block_size;
    }
    void pop_back() {
        if (empty())throw sjtu::container_is_empty("pop_back an empty block");
        _rear=(_rear-_base-1)%block_volume+_base;
        --block_size;
    }
    T& operator[](size_t pos) {
        if (pos<0||pos>=block_size)throw sjtu::index_out_of_bound("operator[]");
        return *(_base+pos%block_volume+_base);
    }
    T& at(size_t pos) {
        if (pos<0||pos>=block_size)throw sjtu::index_out_of_bound("operator[]");
        return *(_base+pos%block_volume+_base);
    }   
};
template <class T>
class deque {
public:
  class const_iterator;
  class iterator {
  private:
    /**
     * add data members.
     * just add whatever you want.
     */
  public:
    /**
     * return a new iterator which points to the n-next element.
     * if there are not enough elements, the behaviour is undefined.
     * same for operator-.
     */
    iterator operator+(const int &n) const {}
    iterator operator-(const int &n) const {}

    /**
     * return the distance between two iterators.
     * if they point to different vectors, throw
     * invaild_iterator.
     */
    int operator-(const iterator &rhs) const {}
    iterator &operator+=(const int &n) {}
    iterator &operator-=(const int &n) {}

    /**
     * iter++
     */
    iterator operator++(int) {}
    /**
     * ++iter
     */
    iterator &operator++() {}
    /**
     * iter--
     */
    iterator operator--(int) {}
    /**
     * --iter
     */
    iterator &operator--() {}

    /**
     * *it
     */
    T &operator*() const {}
    /**
     * it->field
     */
    T *operator->() const noexcept {}

    /**
     * check whether two iterators are the same (pointing to the same
     * memory).
     */
    bool operator==(const iterator &rhs) const {}
    bool operator==(const const_iterator &rhs) const {}
    /**
     * some other operator for iterators.
     */
    bool operator!=(const iterator &rhs) const {}
    bool operator!=(const const_iterator &rhs) const {}
  };

  class const_iterator {
    /**
     * it should has similar member method as iterator.
     * you can copy them, but with care!
     * and it should be able to be constructed from an iterator.
     */
  };

  /**
   * constructors.
   */
  deque() {}
  deque(const deque &other) {}

  /**
   * deconstructor.
   */
  ~deque() {}

  /**
   * assignment operator.
   */
  deque &operator=(const deque &other) {}

  /**
   * access a specified element with bound checking.
   * throw index_out_of_bound if out of bound.
   */
  T &at(const size_t &pos) {}
  const T &at(const size_t &pos) const {}
  T &operator[](const size_t &pos) {}
  const T &operator[](const size_t &pos) const {}

  /**
   * access the first element.
   * throw container_is_empty when the container is empty.
   */
  const T &front() const {}
  /**
   * access the last element.
   * throw container_is_empty when the container is empty.
   */
  const T &back() const {}

  /**
   * return an iterator to the beginning.
   */
  iterator begin() {}
  const_iterator cbegin() const {}

  /**
   * return an iterator to the end.
   */
  iterator end() {}
  const_iterator cend() const {}

  /**
   * check whether the container is empty.
   */
  bool empty() const {}

  /**
   * return the number of elements.
   */
  size_t size() const {}

  /**
   * clear all contents.
   */
  void clear() {}

  /**
   * insert value before pos.
   * return an iterator pointing to the inserted value.
   * throw if the iterator is invalid or it points to a wrong place.
   */
  iterator insert(iterator pos, const T &value) {}

  /**
   * remove the element at pos.
   * return an iterator pointing to the following element. if pos points to
   * the last element, return end(). throw if the container is empty,
   * the iterator is invalid, or it points to a wrong place.
   */
  iterator erase(iterator pos) {}

  /**
   * add an element to the end.
   */
  void push_back(const T &value) {}

  /**
   * remove the last element.
   * throw when the container is empty.
   */
  void pop_back() {}

  /**
   * insert an element to the beginning.
   */
  void push_front(const T &value) {}

  /**
   * remove the first element.
   * throw when the container is empty.
   */
  void pop_front() {}
};

} // namespace sjtu

#endif
