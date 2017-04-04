
/*
 * Copyright (C) hanchong liu
 */


#ifndef _SAE_MEMORY_H_INCLUDED_
#define _SAE_MEMORY_H_INCLUDED_

#include <iostream>
using namespace std;

namespace sae_sp_alloc
{
class sae_mem
{
public:
    sae_mem();
    ~sae_mem();
    
    void *sae_allocate(size_t size)
    {
        return malloc(size);
    }
    
    void destroy(void *p, size_t size)
    {
        free(p);
    }
    
private:
    
};
    
template <typename T>
class sae_allocator
{
public:
    typedef	T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T* const_pointer;
    typedef	const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    
    //rebind<Type>	member class
    
public:
    sae_allocator()
    {
        mem = NULL;
    }
    
    sae_allocator(sae_mem *mem)
    {
        this->mem = mem;
    }
    
    ~sae_allocator()
    {
        mem = NULL;
    }
    
    pointer address(reference x) const
    {
        return &x;
    }
    
    const_pointer address(const_reference x) const
    {
        return &x;
    }
    
    pointer allocate(size_type n, allocator<void>::const_pointer hint = 0)
    {
        if (mem)
        {
            return reinterpret_cast<pointer>(mem->sae_allocate(n * sizeof(value_type)));
        }
        else
        {
            return reinterpret_cast<pointer>(malloc(n * sizeof(value_type)));
        }
    }
    
    void deallocate(pointer p, size_type n)
    {
        if (mem)
        {
            mem->destroy(p, n * sizeof(value_type));
        }
        else
        {
            free(p);
        }
    }
    
    size_type max_size() const throw()
    {
        return ULONG_MAX / sizeof(value_type);
    }
    
    void construct(pointer p, const_reference val)
    {
        new ((void *)p) value_type(val);
    }
    
    void destroy(pointer p)
    {
        p->~value_type();
    }
    
private:
    sae_mem *mem;
};
}


#endif /* _SAE_MEMORY_H_INCLUDED_ */
