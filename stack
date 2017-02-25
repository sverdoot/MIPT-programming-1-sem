#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
#include <cmath>
#include <cstring>

using namespace std;

#define ASSERT_OK()                 \
    {                               \
    if (!ok ())                     \
        {                           \
        dump ();                    \
        assert (0);                 \
        }                           \
    resize_ ();                     \
    }



const double EPSILON = 0.0001;
const int ST_SIZE_MUL = 2;
const int ST_SIZE_ADD = 10;

template <typename T>
class Stack
    {
    public:

    explicit Stack (size_t capacity);
    ~Stack ();
    bool ok () const;
    void dump () const;
    bool check_ctrl_sum () const;
    bool push (T value);
    T pop ();
    int gettop () const;
    bool resize_ ();
    T peek (int num) const;
    T* top () const;
    bool empty_ () const;

    private:

    int protection1_;
    T * data_;
    size_t size_;
    T ctrl_sum_;
    unsigned int top_;
    int protection2_;
    };

template <typename T>
Stack<T>::Stack (size_t capacity):
    size_ (capacity + 2),
    top_ (1),
    protection1_ (0xBADBEEF),
    protection2_ (0xBADBEEF)
    {
        data_ = new T[size_];
        for (unsigned int i = 0; i < size_; i++)
            data_[i] = 0.0;
        ctrl_sum_ = 0;
        data_[0] = 0xBADBEEF;
        data_[size_ - 1] = 0xBADBEEF;
    }


template <typename T>
bool Stack<T>::push (T value)
    {
    ASSERT_OK()

    if (top_ < size_ - 1)
        {
        data_ [top_++] = value;
        ctrl_sum_ = ctrl_sum_ + value;
        }
    else
        {
        resize_ ();
        push (value);
        }

    ASSERT_OK()
    return true;
    }

template <typename T>
T Stack<T>::pop ()
    {
    ASSERT_OK()

    if (top_ > 0)
        {
        T value = data_ [--top_];
        data_[top_] = 0.0;
        ctrl_sum_ = ctrl_sum_ - value;

        ASSERT_OK()
        return value;
        }
    }

template <typename T>
T Stack<T>::peek(int num) const
    {
    ASSERT_OK()

    if (num <= top_ - 1);
        return data_[top_ - num - 1];
    }

template <typename T>
int Stack<T>::gettop () const
    {
    ASSERT_OK()

    return top_;
    }

template <typename T>
bool Stack<T>::ok () const
    {
    if (!this)
        return false;
    if (data_ == NULL ||protection1_ != 0xBADBEEF || protection2_ != 0xBADBEEF || top_ < 1)
        return false;
    if (!check_ctrl_sum ())
        return false;
    if (data_[0] != 0xBADBEEF || data_[size_ - 1] != 0xBADBEEF)
        return false;

    return true;
    }

template <typename T>
bool Stack<T>::check_ctrl_sum () const
    {
    T sum = 0;

    for (unsigned int i = 1; i < size_ - 1; i++)
        sum += data_[i];

    if (fabs (sum - ctrl_sum_) < EPSILON)
        return true;
    else
        return false;
    return false;
    }

template <typename T>
void Stack<T>::dump () const
    {
    FILE* file = fopen ("dump.txt", "a");

    fprintf (file, "== DUMP_stack ==\n");

    if (ok () == 0)
        fprintf (file, "stack is bad\n");
    else
        fprintf (file, "stack is good\n");

    fprintf (file, "\t this = %p\n", this);
    fprintf (file, "\t data = %p\n", data_);
    fprintf (file, "\t size_ = %d\n", size_);
    fprintf (file, "\t top_ = %d\n", top_);
    fprintf (file, "\t ctrl_sum %lg\n", ctrl_sum_);
    fprintf (file, "\t protection1_ = %x\n", protection1_);
    fprintf (file, "\t protection2_ = %x\n", protection2_);

    for (int unsigned i = 0; i < top_; i++)
        fprintf (file, "\t data_[%d] = %lg\n", i, data_[i]);
    fprintf (file, "\t data_[size_ - 1] = %lg\n", data_[size_ - 1]);

    fprintf (file, "\n");
    fclose (file);
    }

template <typename T>
bool Stack<T>::resize_ ()
    {
    T* buf = NULL;
    size_t size_n = 0;

    if (top_ >= size_ - 3)
        {
        buf = (T*) calloc (top_ + ST_SIZE_ADD + 2, sizeof (T));
        size_n = (size_t) (top_ + ST_SIZE_ADD + 2);
        }

    if (buf != NULL)
        {
        data_ [size_ - 1] = 0.0;
        memmove (buf, data_, top_ * sizeof (*buf));
        data_ = buf;
        size_ = size_n;
        data_[size_ - 1] = 0xBADBEEF;
        }
    return true;
    }

template <typename T>
Stack<T>::~Stack ()
    {
    top_ = 0;
    size_ = 0;
    ctrl_sum_ = -100500;
    protection1_ = -100500;
    protection2_ = -100500;
    delete [] data_;
    }

template <typename T>
T* Stack<T>::top () const
    {
    ASSERT_OK()

    return &data_[top_ - 1];
    }

template <typename T>
bool Stack<T>::empty_ () const
    {
    ASSERT_OK()

    return (top_ > 1);
    }


