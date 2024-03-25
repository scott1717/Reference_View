//
//  Reference_View.hpp
//
//  Created by Scott Schampers on 3/16/24.
//

#ifndef Reference_View_h
#define Reference_View_h

#include <cassert>

namespace revive
{
    /**
     Reference_View_Iterator exposes the unlying pointer type as a reference.
     
     The only safety check provided is an assert(*vIt) that makes sure that the pointer is not nullptr.
     */
struct Default_Filter
{
    template<typename T>
    constexpr bool operator()(const T& t)
    {
        return true;
    }
};

// Validated class holds both the const and normal iterators that perfoms a validator_t::validate() test on the pointer
template<typename collection_t, typename filter_t = Default_Filter>
struct Validated
{
    struct iterator
    {
        static iterator begin(collection_t& collection);
        static iterator end(collection_t& collection);
        
        iterator& operator++();
        
        iterator& operator++(int);
        
        bool operator==(const iterator& rhs);
        
        bool operator!=(const iterator& rhs);
        
        auto& operator*();
        
        bool has_nullptr() const;
        

    private:
        iterator(typename collection_t::iterator it, typename collection_t::iterator end);
        
        typename collection_t::iterator vIt;
        typename collection_t::iterator vEnd;
    };
    
    struct const_iterator
    {
        static const_iterator begin(const collection_t& collection);
        static const_iterator end(const collection_t& collection);
        
        const const_iterator& operator++() const;
        
        const const_iterator& operator++(int) const;
        
        bool operator==(const const_iterator& rhs) const;
        
        bool operator!=(const const_iterator& rhs) const;
        
        const auto& operator*() const;
        
        bool has_nullptr() const;
    
        
    private:
        const_iterator(typename collection_t::const_iterator it, typename collection_t::const_iterator end);

        mutable typename collection_t::const_iterator vIt;
        typename collection_t::const_iterator vEnd;
    };
};

// Unvalidated class holds both the const and normal iterators that perfom no further test on the pointer
template<typename collection_t>
struct Unvalidated
{
    struct iterator
    {
        static iterator begin(collection_t& collection);
        static iterator end(collection_t& collection);
        
        iterator& operator++();
        
        iterator& operator++(int);
        
        bool operator==(const iterator& rhs);
        
        bool operator!=(const iterator& rhs);
        
        auto& operator*();
        
        bool has_nullptr() const;
        

    private:
        iterator(typename collection_t::iterator it);
        
        typename collection_t::iterator vIt;
    };
    
    struct const_iterator
    {
        static const_iterator begin(const collection_t& collection);
        static const_iterator end(const collection_t& collection);
        
        const const_iterator& operator++() const;
        
        const const_iterator& operator++(int) const;
        
        bool operator==(const const_iterator& rhs) const;
        
        bool operator!=(const const_iterator& rhs) const;
        
        const auto& operator*() const;
        
        bool has_nullptr() const;
    
        
    private:
        const_iterator(typename collection_t::const_iterator it);

        mutable typename collection_t::const_iterator vIt;
    };
};

    /**
     The Reference_View wraps a collection of pointers and returns iterators that expose each pointer as a reference.
     Requirement - Object must be able to be dereferenced with * operator.
     */

template<typename collection_t, typename iterator_validation_t = Validated<collection_t>>
class Reference_View
{
public:
    
//    using iterator = Reference_View_Iterator<typename collection_t::iterator>;
//    using const_iterator = Const_Reference_View_Iterator<typename collection_t::const_iterator>;
    
    using iterator = typename iterator_validation_t::iterator;
    using const_iterator = typename iterator_validation_t::const_iterator;
    
    Reference_View(collection_t& collection);
    
    iterator begin() noexcept;
    
    iterator end() noexcept;
    
    const_iterator begin() const noexcept;
    
    const_iterator end() const noexcept;
    
private:
    collection_t& rCollection;
};

template<typename filter_t = revive::Default_Filter, typename collection_t>
revive::Reference_View<collection_t, revive::Validated<collection_t, filter_t>> Ref_View(collection_t& collection)
{
    return revive::Reference_View<collection_t, revive::Validated<collection_t, filter_t>>(collection);
}

template<typename collection_t>
revive::Reference_View<collection_t, revive::Unvalidated<collection_t>> Unvalidated_Ref_View(collection_t& collection)
{
    return revive::Reference_View<collection_t, revive::Unvalidated<collection_t>>(collection);
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::iterator::iterator(typename collection_t::iterator it, typename collection_t::iterator end)
: vIt(it), vEnd(end)
{
    
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::iterator Validated<collection_t, filter_t>::iterator::begin(collection_t& collection)
{
    iterator it(collection.begin(), collection.end());
    
    if (it.vIt == it.vEnd) // do not increment beyond end
        return it;
    
    if (!it.has_nullptr()) // validate the pointer
    {
        filter_t filter;
        
        if (filter(*it)) // filter
            return it;
    }
    
    return ++it;
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::iterator Validated<collection_t, filter_t>::iterator::end(collection_t& collection)
{
    return iterator(collection.end(), collection.end()); // no validation needed
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::iterator& Validated<collection_t, filter_t>::iterator::operator++()
{
    while (true) {
        ++vIt;
        
        if (vIt == vEnd)  // don't increment beyond end
            break;
        
        if (this->has_nullptr() == false) // pointer is valid (except maybe dangling reference - user error
        {
            filter_t filter;
            
            if (filter(**this) == true) // filter
                break;
        }
    }
    
    return *this; // either the end() iterator or an iterator pointing to a validated and filtered object
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::iterator& Validated<collection_t, filter_t>::iterator::operator++(int)
{
    while (true) {
        vIt++;
        
        if (vIt == vEnd)  // don't increment beyond end
            break;
        
        if (this->has_nullptr() == false) // pointer is valid (except maybe dangling reference - user error
        {
            filter_t filter;
            
            if (filter(this->operator*()) == true) // filter
                break;
        }
    }
    
    return *this; // either the end() iterator or an iterator pointing to a validated and filtered object
}

template<typename collection_t, typename filter_t>
bool Validated<collection_t, filter_t>::iterator::operator==(const iterator& rhs)
{
    return (this->vIt == rhs.vIt);
}

template<typename collection_t, typename filter_t>
bool Validated<collection_t, filter_t>::iterator::operator!=(const iterator& rhs)
{
    return (this->vIt != rhs.vIt);
}

template<typename collection_t, typename filter_t>
auto& Validated<collection_t, filter_t>::iterator::operator*()
{
    assert(*this->vIt); // debug-time check
    
    return **this->vIt; // dereference the pointer
}

template<typename collection_t, typename filter_t>
bool Validated<collection_t, filter_t>::iterator::has_nullptr() const
{
    return (*this->vIt == nullptr);  // check to make sure that pointer exists
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::const_iterator::const_iterator(typename collection_t::const_iterator it, typename collection_t::const_iterator end)
: vIt(it), vEnd(end)
{
    
}

template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::const_iterator Validated<collection_t, filter_t>::const_iterator::begin(const collection_t& collection)
{
    const_iterator it(collection.begin(), collection.end());
    
    if (it.vIt == it.vEnd) // do not increment beyond end
        return it;
    
    if (!it.has_nullptr()) // validate the pointer
    {
        filter_t filter;
        
        if (filter(*it)) // filter
            return it;
    }
    
    return ++it;
}
    
template<typename collection_t, typename filter_t>
Validated<collection_t, filter_t>::const_iterator Validated<collection_t, filter_t>::const_iterator::end(const collection_t& collection)
{
    return const_iterator(collection.end(), collection.end());
}

template<typename collection_t, typename filter_t>
const Validated<collection_t, filter_t>::const_iterator& Validated<collection_t, filter_t>::const_iterator::operator++() const
{
    while (true) {
        ++vIt;
        
        if (vIt == vEnd)  // don't increment beyond end
            break;
        
        if (this->has_nullptr() == false) // pointer is valid (except maybe dangling reference - user error
        {
            filter_t filter;
            
            if (filter(this->operator*()) == true) // filter
                break;
        }
    }
    
    return *this; // either the end() iterator or an iterator pointing to a validated and filtered object
}

template<typename collection_t, typename filter_t>
const Validated<collection_t, filter_t>::const_iterator& Validated<collection_t, filter_t>::const_iterator::operator++(int) const
{
    while (true) {
        vIt++;
        
        if (vIt == vEnd)  // don't increment beyond end
            break;
        
        if (this->has_nullptr() == false) // pointer is valid (except maybe dangling reference - user error
        {
            filter_t filter;
            
            if (filter(this->operator*()) == true) // filter
                break;
        }
    }
    
    return *this; // either the end() iterator or an iterator pointing to a validated and filtered object
}

template<typename collection_t, typename filter_t>
bool Validated<collection_t, filter_t>::const_iterator::operator==(const const_iterator &rhs) const
{
    return (this->vIt == rhs.vIt);
}
    
template<typename collection_t, typename filter_t>
bool Validated<collection_t, filter_t>::const_iterator::operator!=(const const_iterator &rhs) const
{
    return (this->vIt != rhs.vIt);
}

template<typename collection_t, typename filter_t>
const auto& Validated<collection_t, filter_t>::const_iterator::operator*() const
{
    assert(*this->vIt); // debug-time check
    
    return **this->vIt;
}
    
template<typename collection_t, typename filter_t>
bool Validated<collection_t, filter_t>::const_iterator::has_nullptr() const
{
    return (*this->vIt == nullptr);
}


template<typename collection_t>
Unvalidated<collection_t>::iterator::iterator(typename collection_t::iterator it)
: vIt(it)
{
    
}

template<typename collection_t>
Unvalidated<collection_t>::iterator Unvalidated<collection_t>::iterator::begin(collection_t& collection)
{
    return iterator(collection.begin());
}

template<typename collection_t>
Unvalidated<collection_t>::iterator Unvalidated<collection_t>::iterator::end(collection_t& collection)
{
    return iterator(collection.end());
}

template<typename collection_t>
Unvalidated<collection_t>::iterator& Unvalidated<collection_t>::iterator::operator++()
{
    ++this->vIt;
    
    return *this;
}

template<typename collection_t>
Unvalidated<collection_t>::iterator& Unvalidated<collection_t>::iterator::operator++(int)
{
    this->vIt++;
    
    return *this;
}

template<typename collection_t>
bool Unvalidated<collection_t>::iterator::operator==(const iterator& rhs)
{
    return (this->vIt == rhs.vIt);
}

template<typename collection_t>
bool Unvalidated<collection_t>::iterator::operator!=(const iterator& rhs)
{
    return (this->vIt != rhs.vIt);
}

template<typename collection_t>
auto& Unvalidated<collection_t>::iterator::operator*()
{
    assert(*this->vIt); // debug-time check
    
    return **this->vIt; // dereference the pointer
}

template<typename collection_t>
bool Unvalidated<collection_t>::iterator::has_nullptr() const
{
    return (*this->vIt == nullptr);  // check to make sure that pointer exists
}

template<typename collection_t>
Unvalidated<collection_t>::const_iterator::const_iterator(typename collection_t::const_iterator it)
: vIt(it)
{
    
}

template<typename collection_t>
Unvalidated<collection_t>::const_iterator Unvalidated<collection_t>::const_iterator::begin(const collection_t& collection)
{
    return const_iterator(collection.begin());
}
    
template<typename collection_t>
Unvalidated<collection_t>::const_iterator Unvalidated<collection_t>::const_iterator::end(const collection_t& collection)
{
    return const_iterator(collection.end());
}

template<typename collection_t>
const Unvalidated<collection_t>::const_iterator& Unvalidated<collection_t>::const_iterator::operator++() const
{
    ++this->vIt;
    
    return *this;
}

template<typename collection_t>
const Unvalidated<collection_t>::const_iterator& Unvalidated<collection_t>::const_iterator::operator++(int) const
{
    this->vIt++;
    
    return *this;
}

template<typename collection_t>
bool Unvalidated<collection_t>::const_iterator::operator==(const const_iterator &rhs) const
{
    return (this->vIt == rhs.vIt);
}
    
template<typename collection_t>
bool Unvalidated<collection_t>::const_iterator::operator!=(const const_iterator &rhs) const
{
    return (this->vIt != rhs.vIt);
}

template<typename collection_t>
const auto& Unvalidated<collection_t>::const_iterator::operator*() const
{
    assert(*this->vIt); // debug-time check
    
    return **this->vIt;
}
    
template<typename collection_t>
bool Unvalidated<collection_t>::const_iterator::has_nullptr() const
{
    return (*this->vIt == nullptr);
}

template<typename collection_t, typename iterator_validation_t>
Reference_View<collection_t, iterator_validation_t>::Reference_View(collection_t& collection)
: rCollection(collection)
{

}

template<typename collection_t, typename iterator_validation_t>
Reference_View<collection_t, iterator_validation_t>::iterator Reference_View<collection_t, iterator_validation_t>::begin() noexcept
{
    return iterator::begin(this->rCollection);
}

template<typename collection_t, typename iterator_validation_t>
Reference_View<collection_t, iterator_validation_t>::iterator Reference_View<collection_t, iterator_validation_t>::end() noexcept
{
    return iterator::end(this->rCollection);
}

template<typename collection_t, typename iterator_validation_t>
Reference_View<collection_t, iterator_validation_t>::const_iterator Reference_View<collection_t, iterator_validation_t>::begin() const noexcept
{
    return const_iterator::begin(this->rCollection);
}

template<typename collection_t, typename iterator_validation_t>
Reference_View<collection_t, iterator_validation_t>::const_iterator Reference_View<collection_t, iterator_validation_t>::end() const noexcept
{
    return const_iterator::end(this->rCollection);
}




}



#endif /* Reference_View_h */
