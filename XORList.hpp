#ifndef _SCC_XORLIST_HPP_
#define _SCC_XORLIST_HPP_
#pragma once

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <iterator>
#include <type_traits>
#include <limits>
#include <algorithm>
#include <initializer_list>

namespace scc
{
    enum class CanThrow
    {
        NoThrow,
        Throw,
    };

    template <typename T, CanThrow canThrow = CanThrow::NoThrow, typename Allocator = std::allocator<T>>
    class XORList
    {
    private:
        struct Node
        {
            T data;
            Node *npx; // XOR of next and previous node pointers

            Node(T value) : data(value), npx(nullptr) {}
        };

        Node *m_head_;
        Node *m_tail_;
        size_t m_size_;
        using NodeAllocator = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
        NodeAllocator alloc_;

        Node *XOR(Node *a, Node *b)
        {
            return reinterpret_cast<Node *>(reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b));
        }

        const Node *XOR(const Node *a, const Node *b) const
        {
            return reinterpret_cast<const Node *>(reinterpret_cast<uintptr_t>(a) ^ reinterpret_cast<uintptr_t>(b));
        }

        Node *allocate_node(const T &value)
        {
            Node *newNode = std::allocator_traits<NodeAllocator>::allocate(alloc_, 1);
            std::allocator_traits<NodeAllocator>::construct(alloc_, newNode, value);
            return newNode;
        }

        void deallocate_node(Node *node)
        {
            std::allocator_traits<NodeAllocator>::destroy(alloc_, node);
            std::allocator_traits<NodeAllocator>::deallocate(alloc_, node, 1);
        }

    public:
        explicit XORList(const Allocator &allocator = Allocator()) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0), alloc_(allocator) {}

        XORList(size_t count, const T &value, const Allocator &allocator = Allocator()) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0), alloc_(allocator)
        {
            for (size_t i = 0; i < count; ++i)
            {
                push_back(value);
            }
        }

        explicit XORList(size_t count, const Allocator &allocator = Allocator()) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0), alloc_(allocator)
        {
            for (size_t i = 0; i < count; ++i)
            {
                push_back(T());
            }
        }

        template <class InputIt, typename = std::_RequireInputIter<InputIt>>
        XORList(InputIt first, InputIt last, const Allocator &allocator = Allocator()) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0), alloc_(allocator)
        {
            for (InputIt it = first; it != last; ++it)
            {
                push_back(*it);
            }
        }

        XORList(const XORList &other) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0), alloc_(std::allocator_traits<NodeAllocator>::select_on_container_copy_construction(other.alloc_))
        {
            for (Node *current = other.m_head_, *prev = nullptr, *next; current != nullptr; prev = current, current = next)
            {
                next = XOR(prev, current->npx);
                push_back(current->data);
            }
        }

        XORList(XORList &&other) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(other.m_head_), m_tail_(other.m_tail_), m_size_(other.m_size_), alloc_(std::move(other.alloc_))
        {
            other.m_head_ = other.m_tail_ = nullptr;
            other.m_size_ = 0;
        }

        XORList(std::initializer_list<T> init, const Allocator &allocator = Allocator()) noexcept(canThrow == CanThrow::NoThrow)
            : m_head_(nullptr), m_tail_(nullptr), m_size_(0), alloc_(allocator)
        {
            for (const T &value : init)
            {
                push_back(value);
            }
        }

        bool operator==(const XORList &other) const noexcept
        {
            if (m_size_ != other.m_size_)
            {
                return false;
            }

            const Node *current = m_head_;
            const Node *prev = nullptr;
            const Node *next;

            const Node *other_current = other.m_head_;
            const Node *other_prev = nullptr;
            const Node *other_next;

            while (current != nullptr && other_current != nullptr)
            {
                if (current->data != other_current->data)
                {
                    return false;
                }

                next = XOR(prev, current->npx);
                other_next = XOR(other_prev, other_current->npx);

                prev = current;
                current = next;

                other_prev = other_current;
                other_current = other_next;
            }

            return current == nullptr && other_current == nullptr;
        }

        bool operator!=(const XORList &other) const noexcept
        {
            return !(*this == other);
        }

        ~XORList()
        {
            clear();
        }

        XORList &operator=(const XORList &other) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (this != &other)
            {
                clear();
                if constexpr (std::allocator_traits<NodeAllocator>::propagate_on_container_copy_assignment::value)
                {
                    alloc_ = other.alloc_;
                }
                for (Node *current = other.m_head_, *prev = nullptr, *next; current != nullptr; prev = current, current = next)
                {
                    next = XOR(prev, current->npx);
                    push_back(current->data);
                }
            }
            return *this;
        }

        XORList &operator=(XORList &&other) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (this != &other)
            {
                clear();
                if constexpr (std::allocator_traits<NodeAllocator>::propagate_on_container_move_assignment::value)
                {
                    alloc_ = std::move(other.alloc_);
                }
                else if (alloc_ != other.alloc_)
                {
                    for (Node *current = other.m_head_, *prev = nullptr, *next; current != nullptr; prev = current, current = next)
                    {
                        next = XOR(prev, current->npx);
                        push_back(std::move(current->data));
                    }
                    other.clear();
                    return *this;
                }
                m_head_ = other.m_head_;
                m_tail_ = other.m_tail_;
                m_size_ = other.m_size_;

                other.m_head_ = other.m_tail_ = nullptr;
                other.m_size_ = 0;
            }
            return *this;
        }

        XORList &operator=(std::initializer_list<T> ilist) noexcept(canThrow == CanThrow::NoThrow)
        {
            clear();
            for (const T &value : ilist)
            {
                push_back(value);
            }
            return *this;
        }

        void assign(size_t count, const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            clear();
            for (size_t i = 0; i < count; ++i)
            {
                push_back(value);
            }
        }

        template <class InputIt, typename = std::_RequireInputIter<InputIt>>
        void assign(InputIt first, InputIt last) noexcept(canThrow == CanThrow::NoThrow)
        {
            clear();
            for (InputIt it = first; it != last; ++it)
            {
                push_back(*it);
            }
        }

        void assign(std::initializer_list<T> ilist) noexcept(canThrow == CanThrow::NoThrow)
        {
            clear();
            for (const T &value : ilist)
            {
                push_back(value);
            }
        }

        auto get_allocator() const noexcept
        {
            return Allocator(alloc_);
        }

        T &front() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return m_tail_->data; // undefined behavior
                }
            }
            return m_head_->data;
        }

        const T &front() const noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return m_tail_->data; // undefined behavior
                }
            }
            return m_head_->data;
        }

        T &back() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return m_tail_->data; // undefined behavior
                }
            }
            return m_tail_->data;
        }

        const T &back() const noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return m_tail_->data; // undefined behavior
                }
            }
            return m_tail_->data;
        }

        template <bool IsConst>
        class XORListIterator
        {
        private:
            using NodeType = std::conditional_t<IsConst, const Node, Node>;
            using XORListType = std::conditional_t<IsConst, const XORList, XORList>;

            NodeType *prev_;
            NodeType *current_;
            XORListType *list_;

            friend class XORList<T, canThrow, Allocator>;

        public:
            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = std::conditional_t<IsConst, const T *, T *>;
            using reference = std::conditional_t<IsConst, const T &, T &>;

            XORListIterator(NodeType *prev, NodeType *current, XORListType *list)
                : prev_(prev), current_(current), list_(list) {}

            reference operator*() const { return current_->data; }
            pointer operator->() const { return &(current_->data); }

            XORListIterator &operator++()
            {
                if (current_ == nullptr)
                {
                    return *this; // Already at end, do nothing
                }

                NodeType *next = list_->XOR(prev_, current_->npx);
                prev_ = current_;
                current_ = next;
                return *this;
            }

            XORListIterator operator++(int)
            {
                XORListIterator temp = *this;
                ++(*this);
                return temp;
            }

            XORListIterator &operator--()
            {
                if (prev_ == nullptr)
                {
                    return *this; // Already at begin, do nothing
                }

                NodeType *next = list_->XOR(prev_->npx, current_);
                current_ = prev_;
                prev_ = next;
                return *this;
            }

            XORListIterator operator--(int)
            {
                XORListIterator temp = *this;
                --(*this);
                return temp;
            }

            XORListIterator operator+(difference_type n) const
            {
                XORListIterator temp = *this;
                for (difference_type i = 0; i < n; ++i)
                {
                    if (temp.current_ == nullptr)
                        break;
                    ++temp;
                }
                return temp;
            }

            XORListIterator operator-(difference_type n) const
            {
                XORListIterator temp = *this;
                for (difference_type i = 0; i < n; ++i)
                {
                    if (temp.prev_ == nullptr && temp.current_ == list_->m_head_)
                        break;
                    --temp;
                }
                return temp;
            }

            difference_type operator-(const XORListIterator &other) const
            {
                if (list_ != other.list_)
                    return -1;

                difference_type count = 0;
                XORListIterator it = *this;
                if (it < other)
                {
                    while (it != other)
                    {
                        ++it;
                        ++count;
                    }
                }
                else
                {
                    while (it != other)
                    {
                        --it;
                        ++count;
                    }
                }
                return count;
            }

            bool operator<(const XORListIterator &other) const
            {
                // lexigraphical comparison
                if (list_ != other.list_)
                    return false;

                if (prev_ == nullptr && other.prev_ == nullptr)
                    return current_ < other.current_;
                if (prev_ == nullptr)
                    return true;
                if (other.prev_ == nullptr)
                    return false;
                return prev_ < other.prev_;
            }

            bool operator==(const XORListIterator &other) const { return current_ == other.current_; }
            bool operator!=(const XORListIterator &other) const { return current_ != other.current_; }
        };

        // Aliases for iterator and const_iterator
        using iterator = XORListIterator<false>;
        using const_iterator = XORListIterator<true>;

        iterator begin() noexcept
        {
            return iterator(nullptr, m_head_, this);
        }

        const_iterator cbegin() const noexcept
        {
            return const_iterator(nullptr, m_head_, this);
        }

        iterator end() noexcept
        {
            return iterator(m_tail_, nullptr, this);
        }

        const_iterator cend() const noexcept
        {
            return const_iterator(m_tail_, nullptr, this);
        }

        iterator rbegin() noexcept
        {
            return iterator(nullptr, m_tail_, this);
        }

        const_iterator crbegin() const noexcept
        {
            return const_iterator(nullptr, m_tail_, this);
        }

        iterator rend() noexcept
        {
            return iterator(m_head_, nullptr, this);
        }

        const_iterator crend() const noexcept
        {
            return const_iterator(m_head_, nullptr, this);
        }

        bool empty() const noexcept
        {
            return m_size_ == 0;
        }

        size_t size() const noexcept
        {
            return m_size_;
        }

        size_t max_size() const noexcept
        {
            return std::numeric_limits<size_t>::max();
        }

        void clear() noexcept
        {
            if (empty())
            {
                return;
            }

            Node *current = m_head_;
            Node *prev = nullptr;
            Node *next;

            while (current != nullptr)
            {
                next = XOR(prev, current->npx);
                deallocate_node(current);
                prev = current;
                current = next;
            }

            m_head_ = m_tail_ = nullptr;
            m_size_ = 0;
        }

        iterator insert(const_iterator pos, const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            return emplace(pos, value);
        }

        iterator insert(const_iterator pos, T &&value) noexcept(canThrow == CanThrow::NoThrow)
        {
            return emplace(pos, std::move(value));
        }

        iterator insert(const_iterator pos, size_t count, const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (count == 0)
                return pos;

            iterator it = pos;
            for (size_t i = 0; i < count; ++i)
            {
                it = emplace(it, value);
                ++it;
            }
            return pos;
        }

        template <class InputIt, typename = std::_RequireInputIter<InputIt>>
        iterator insert(const_iterator pos, InputIt first, InputIt last) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (first == last)
                return pos;

            iterator it = pos;
            for (InputIt i = first; i != last; ++i)
            {
                it = emplace(it, *i);
                ++it;
            }
            return pos;
        }

        iterator insert(const_iterator pos, std::initializer_list<T> ilist) noexcept(canThrow == CanThrow::NoThrow)
        {
            return insert(pos, ilist.begin(), ilist.end());
        }

        template <class... Args>
        iterator emplace(const_iterator pos, Args &&...args) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (pos == cbegin())
            {
                emplace_front(std::forward<Args>(args)...);
                return begin();
            }
            if (pos == cend())
            {
                emplace_back(std::forward<Args>(args)...);
                return --end();
            }

            Node *newNode = allocate_node(T(std::forward<Args>(args)...));
            if (!newNode)
            {
                return end();
            }

            Node *prev = const_cast<Node *>(pos.prev_);
            Node *current = const_cast<Node *>(pos.current_);
            Node *next = XOR(prev, current->npx);

            newNode->npx = XOR(prev, current);
            prev->npx = XOR(XOR(prev->npx, current), newNode);
            current->npx = XOR(newNode, next);

            ++m_size_;
            return iterator(prev, newNode, this);
        }

        iterator erase(const_iterator pos) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (pos == cend())
            {
                return end();
            }

            Node *prev = const_cast<Node *>(pos.prev_);
            Node *current = const_cast<Node *>(pos.current_);
            Node *next = XOR(prev, current->npx);

            if (prev != nullptr)
            {
                prev->npx = XOR(XOR(prev->npx, current), next);
            }
            else
            {
                m_head_ = next;
            }

            if (next != nullptr)
            {
                next->npx = XOR(prev, XOR(current, next->npx));
            }
            else
            {
                m_tail_ = prev;
            }

            deallocate_node(current);
            --m_size_;

            return iterator(prev, next, this);
        }

        iterator erase(const_iterator first, const_iterator last) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (first == last)
            {
                //  If [first, last) is an empty range, then last is returned.
                return iterator(const_cast<Node *>(last.prev_), const_cast<Node *>(last.current_), this);
            }

            Node *first_prev = const_cast<Node *>(first.prev_);
            Node *last_current = const_cast<Node *>(last.current_);
            Node *last_next = XOR(last_current->npx, const_cast<Node *>(last.prev_));
            Node *current = const_cast<Node *>(first.current_);
            Node *current_prev = first_prev;
            Node *current_backup = current;

            while (current != last_current)
            {
                Node *next = XOR(current_prev, current->npx);
                current_prev = current;
                deallocate_node(current);
                current = next;
                --m_size_;
            }

            if (first_prev != nullptr)
            {
                first_prev->npx = XOR(XOR(first_prev->npx, current_backup), last_current);
            }
            else
            {
                last_current->npx = XOR(first_prev, last_next);
                m_head_ = last_current;
            }

            if (last_current != nullptr)
            {
                last_current->npx = XOR(first_prev, XOR(const_cast<Node *>(last.prev_), last_current->npx));
            }
            else
            {
                first_prev->npx = XOR(XOR(first_prev->npx, current_backup), last_current);
                m_tail_ = first_prev;
            }

            return iterator(first_prev, last_current, this);
        }

        void push_back(const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = allocate_node(value);
            if (!newNode)
            {
                return; // No operation on allocation failure
            }

            newNode->npx = m_tail_;

            if (m_tail_ != nullptr)
            {
                m_tail_->npx = XOR(newNode, m_tail_->npx);
            }
            else
            {
                m_head_ = newNode;
            }

            m_tail_ = newNode;
            ++m_size_;
        }

        void push_back(T &&value) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = allocate_node(std::forward<T>(value));
            if (!newNode)
            {
                return; // No operation on allocation failure
            }

            newNode->npx = m_tail_;

            if (m_tail_ != nullptr)
            {
                m_tail_->npx = XOR(newNode, m_tail_->npx);
            }
            else
            {
                m_head_ = newNode;
            }

            m_tail_ = newNode;
            ++m_size_;
        }

        template <class... Args>
        T &emplace_back(Args &&...args) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = allocate_node(T(std::forward<Args>(args)...));
            if (!newNode)
            {
                return m_tail_->data; // undefined behavior
            }

            newNode->npx = m_tail_;

            if (m_tail_ != nullptr)
            {
                m_tail_->npx = XOR(newNode, m_tail_->npx);
            }
            else
            {
                m_head_ = newNode;
            }

            m_tail_ = newNode;
            ++m_size_;
            return newNode->data;
        }

        void pop_back() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return; // No operation on empty list
                }
            }

            Node *temp = m_tail_;
            Node *prev = XOR(nullptr, m_tail_->npx);

            if (prev != nullptr)
            {
                prev->npx = XOR(temp, prev->npx);
            }
            else
            {
                m_head_ = nullptr;
            }

            m_tail_ = prev;
            deallocate_node(temp);
            --m_size_;
        }

        void push_front(const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = allocate_node(value);
            if (!newNode)
            {
                return; // No operation on allocation failure
            }

            newNode->npx = m_head_;

            if (m_head_ != nullptr)
            {
                m_head_->npx = XOR(newNode, m_head_->npx);
            }
            else
            {
                m_tail_ = newNode;
            }

            m_head_ = newNode;
            ++m_size_;
        }

        void push_front(T &&value) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = allocate_node(std::forward<T>(value));
            if (!newNode)
            {
                return; // No operation on allocation failure
            }

            newNode->npx = m_head_;

            if (m_head_ != nullptr)
            {
                m_head_->npx = XOR(newNode, m_head_->npx);
            }
            else
            {
                m_tail_ = newNode;
            }

            m_head_ = newNode;
            ++m_size_;
        }

        template <class... Args>
        T &emplace_front(Args &&...args) noexcept(canThrow == CanThrow::NoThrow)
        {
            Node *newNode = allocate_node(T(std::forward<Args>(args)...));
            if (!newNode)
            {
                return m_head_->data; // undefined behavior
            }

            newNode->npx = m_head_;

            if (m_head_ != nullptr)
            {
                m_head_->npx = XOR(newNode, m_head_->npx);
            }
            else
            {
                m_tail_ = newNode;
            }

            m_head_ = newNode;
            ++m_size_;
            return newNode->data;
        }

        void pop_front() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (empty())
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::runtime_error("List is empty");
                }
                else
                {
                    return; // No operation on empty list
                }
            }

            Node *temp = m_head_;
            Node *next = XOR(nullptr, m_head_->npx);

            if (next != nullptr)
            {
                next->npx = XOR(temp, next->npx);
            }
            else
            {
                m_tail_ = nullptr;
            }

            m_head_ = next;
            deallocate_node(temp);
            --m_size_;
        }

        void resize(size_t count, const T &value = T()) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (count < m_size_)
            {
                while (m_size_ > count)
                {
                    pop_back();
                }
            }
            else if (count > m_size_)
            {
                while (m_size_ < count)
                {
                    push_back(value);
                }
            }
        }

        void swap(XORList &other) noexcept
        {
            using std::swap;

            if constexpr (std::allocator_traits<NodeAllocator>::propagate_on_container_swap::value)
            {
                swap(alloc_, other.alloc_);
            }

            swap(m_head_, other.m_head_);
            swap(m_tail_, other.m_tail_);
            swap(m_size_, other.m_size_);
        }

        void merge(XORList<T> &other_list) noexcept
        {
            if (other_list.empty())
            {
                return;
            }

            if (empty())
            {
                m_head_ = other_list.m_head_;
                m_tail_ = other_list.m_tail_;
                m_size_ = other_list.m_size_;
                other_list.m_head_ = other_list.m_tail_ = nullptr;
                other_list.m_size_ = 0;
                return;
            }

            m_tail_->npx = XOR(XOR(m_tail_->npx, nullptr), other_list.m_head_);
            other_list.m_head_->npx = XOR(m_tail_, other_list.m_head_->npx);

            m_tail_ = other_list.m_tail_;
            m_size_ += other_list.m_size_;

            other_list.m_head_ = other_list.m_tail_ = nullptr;
            other_list.m_size_ = 0;
        }

        void splice(size_t position, XORList<T> &other_list) noexcept(canThrow == CanThrow::NoThrow)
        {
            if (position > m_size_)
            {
                if constexpr (canThrow == CanThrow::Throw)
                {
                    throw std::out_of_range("Position out of range");
                }
                else
                {
                    return; // No operation on out of range position
                }
            }

            if (other_list.empty())
            {
                return;
            }

            if (position == 0)
            {
                if (m_head_ != nullptr)
                {
                    other_list.m_tail_->npx = XOR(other_list.m_tail_->npx, m_head_);
                    m_head_->npx = XOR(other_list.m_tail_, XOR(nullptr, m_head_->npx));
                }
                else
                {
                    m_tail_ = other_list.m_tail_;
                }

                m_head_ = other_list.m_head_;
            }
            else if (position == m_size_)
            {
                m_tail_->npx = XOR(m_tail_->npx, other_list.m_head_);
                other_list.m_head_->npx = XOR(m_tail_, other_list.m_head_->npx);

                m_tail_ = other_list.m_tail_;
            }
            else
            {
                Node *prev = nullptr;
                Node *current = m_head_;
                Node *next;

                for (size_t i = 0; i < position; ++i)
                {
                    next = XOR(prev, current->npx);
                    prev = current;
                    current = next;
                }

                Node *other_tail = other_list.m_tail_;
                other_tail->npx = XOR(other_tail->npx, current);
                prev->npx = XOR(XOR(prev->npx, current), other_list.m_head_);
                other_list.m_head_->npx = XOR(prev, other_list.m_head_->npx);
                current->npx = XOR(other_list.m_tail_, XOR(prev, current->npx));
            }

            m_size_ += other_list.m_size_;

            other_list.m_head_ = other_list.m_tail_ = nullptr;
            other_list.m_size_ = 0;
        }

        size_t remove(const T &value) noexcept(canThrow == CanThrow::NoThrow)
        {
            size_t count = 0;
            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next = nullptr;

            while (current != nullptr)
            {
                next = XOR(prev, current->npx);
                if (current->data == value)
                {
                    if (prev != nullptr)
                    {
                        prev->npx = XOR(XOR(prev->npx, current), next);
                    }
                    else
                    {
                        m_head_ = next;
                    }
                    if (next != nullptr)
                    {
                        next->npx = XOR(prev, XOR(current, next->npx));
                    }
                    else
                    {
                        m_tail_ = prev;
                    }
                    deallocate_node(current);
                    --m_size_;
                    ++count;
                }
                else
                {
                    prev = current;
                }
                current = next;
            }
            return count;
        }

        template <class UnaryPredicate>
        size_t remove_if(UnaryPredicate p) noexcept(canThrow == CanThrow::NoThrow)
        {
            size_t count = 0;
            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next = nullptr;

            while (current != nullptr)
            {
                next = XOR(prev, current->npx);
                if (p(current->data))
                {
                    if (prev != nullptr)
                    {
                        prev->npx = XOR(XOR(prev->npx, current), next);
                    }
                    else
                    {
                        m_head_ = next;
                    }
                    if (next != nullptr)
                    {
                        next->npx = XOR(prev, XOR(current, next->npx));
                    }
                    else
                    {
                        m_tail_ = prev;
                    }
                    deallocate_node(current);
                    --m_size_;
                    ++count;
                }
                else
                {
                    prev = current;
                }
                current = next;
            }
            return count;
        }

        void reverse() noexcept
        {
            Node *temp = m_head_;
            m_head_ = m_tail_;
            m_tail_ = temp;
        }

        void unique() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (m_size_ < 2)
            {
                return;
            }

            Node *prev = nullptr;
            Node *current = m_head_;
            Node *next = XOR(prev, current->npx);

            while (next != nullptr)
            {
                if (current->data == next->data)
                {
                    Node *next_next = XOR(current, next->npx);
                    current->npx = XOR(prev, next_next);

                    if (next_next != nullptr)
                    {
                        next_next->npx = XOR(current, XOR(next, next_next->npx));
                    }
                    else
                    {
                        m_tail_ = current;
                    }

                    deallocate_node(next);
                    next = next_next;
                    --m_size_;
                }
                else
                {
                    prev = current;
                    current = next;
                    next = XOR(prev, current->npx);
                }
            }
        }

        void sort() noexcept(canThrow == CanThrow::NoThrow)
        {
            if (m_size_ < 2)
            {
                return;
            }

            std::sort(begin(), end());
        }
    };
}

#endif // _SCC_XORLIST_HPP_
