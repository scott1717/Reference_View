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
    template<typename iterator_t>
    class Reference_View_Iterator
    {
    public:
        Reference_View_Iterator(iterator_t it);
        
        Reference_View_Iterator& operator++();
        
        Reference_View_Iterator& operator++(int);
        
        bool operator==(const Reference_View_Iterator& rhs);
        
        bool operator!=(const Reference_View_Iterator& rhs);
        
        auto& operator*();
        
        bool is_nullptr() const;
        
    private:
        iterator_t vIt;
    };

    template<typename const_interator_t>
    class Const_Reference_View_Iterator
    {
    public:
        Const_Reference_View_Iterator(const_interator_t it);
        
        const Const_Reference_View_Iterator& operator++() const;
        
        const Const_Reference_View_Iterator& operator++(int) const;
        
        bool operator==(const Const_Reference_View_Iterator& rhs) const;
        
        bool operator!=(const Const_Reference_View_Iterator& rhs) const;
        
        const auto& operator*() const;
        
        bool is_nullptr() const;
        
    private:
        mutable const_interator_t vIt;
    };


    /**
     The Reference_View wraps a collection of pointers and returns iterators that expose each pointer as a reference.
     Requirement - Object must be able to be dereferenced with * operator.
     */
    template<typename collection_t>
    class Reference_View
    {
    public:
        
        using iterator = Reference_View_Iterator<typename collection_t::iterator>;
        using const_iterator = Const_Reference_View_Iterator<typename collection_t::const_iterator>;
        
        Reference_View(collection_t& collection);
        
        iterator begin() noexcept;
        
        iterator end() noexcept;
        
        const_iterator begin() const noexcept;
        
        const_iterator end() const noexcept;
        
    private:
        collection_t& rCollection;
    };


    template<typename iterator_t>
    Reference_View_Iterator<iterator_t>::Reference_View_Iterator(iterator_t it)
    : vIt(it)
    {
        
    }
        
    template<typename iterator_t>
    Reference_View_Iterator<iterator_t>& Reference_View_Iterator<iterator_t>::operator++()
    {
        ++this->vIt;
        return *this;
    }
        
    template<typename iterator_t>
    Reference_View_Iterator<iterator_t>& Reference_View_Iterator<iterator_t>::operator++(int)
    {
        this->vIt++;
        return *this;
    }
    
    template<typename iterator_t>
    bool Reference_View_Iterator<iterator_t>::operator==(const Reference_View_Iterator& rhs)
    {
        return (this->vIt == rhs.vIt);
    }
        
    template<typename iterator_t>
    bool Reference_View_Iterator<iterator_t>::operator!=(const Reference_View_Iterator& rhs)
    {
        return (this->vIt != rhs.vIt);
    }
        
    template<typename iterator_t>
    auto& Reference_View_Iterator<iterator_t>::operator*()
    {
        assert(*this->vIt);
        
        return **this->vIt;
    }

template<typename iterator_t>
bool Reference_View_Iterator<iterator_t>::is_nullptr() const
{
    return (*(this->vIt) == nullptr);
}

    template<typename iterator_t>
    Const_Reference_View_Iterator<iterator_t>::Const_Reference_View_Iterator(iterator_t it)
    : vIt(it)
    {
        
    }

    template<typename iterator_t>
    const Const_Reference_View_Iterator<iterator_t>& Const_Reference_View_Iterator<iterator_t>::operator++() const
    {
        ++this->vIt;
        return *this;
    }
        
    template<typename iterator_t>
    const Const_Reference_View_Iterator<iterator_t>& Const_Reference_View_Iterator<iterator_t>::operator++(int) const
    {
        this->vIt++;
        return *this;
    }
    
    template<typename iterator_t>
    bool Const_Reference_View_Iterator<iterator_t>::operator==(const Const_Reference_View_Iterator& rhs) const
    {
        return (this->vIt == rhs.vIt);
    }
        
    template<typename iterator_t>
    bool Const_Reference_View_Iterator<iterator_t>::operator!=(const Const_Reference_View_Iterator& rhs) const
    {
        return (this->vIt != rhs.vIt);
    }
        
    template<typename iterator_t>
    const auto& Const_Reference_View_Iterator<iterator_t>::operator*() const
    {
        assert(*this->vIt);
        
        return **this->vIt;
    }

template<typename iterator_t>
bool Const_Reference_View_Iterator<iterator_t>::is_nullptr() const
{
    return (*(this->vIt) == nullptr);
}

    template<typename collection_t>
    Reference_View<collection_t>::Reference_View(collection_t& collection)
    : rCollection(collection)
    {

    }

    template<typename collection_t>
    Reference_View<collection_t>::iterator Reference_View<collection_t>::begin() noexcept
    {
        // If the wrapped collection is const then Reference_View should be too.
        static_assert(!(std::is_const_v<std::remove_reference_t<decltype(rCollection)>>),"Reference_View should be const if wrapping a const collection...");
        
        return Reference_View_Iterator(rCollection.begin());
    }

    template<typename collection_t>
    Reference_View<collection_t>::iterator Reference_View<collection_t>::end() noexcept
    {
        // If the wrapped collection is const then Reference_View should be too.
        static_assert(!(std::is_const_v<std::remove_reference_t<decltype(rCollection)>>),"Reference_View should be const if wrapping a const collection...");
        
        return Reference_View_Iterator(rCollection.end());
    }

    template<typename collection_t>
    Reference_View<collection_t>::const_iterator Reference_View<collection_t>::begin() const noexcept
    {
        // if code path came here, make sure that the collection is indeed const
        const auto& const_rCollection = rCollection;
        
        return Const_Reference_View_Iterator(const_rCollection.begin());
    }

    template<typename collection_t>
    Reference_View<collection_t>::const_iterator Reference_View<collection_t>::end() const noexcept
    {
        // if code path came here, make sure that the collection is indeed const
        const auto& const_rCollection = rCollection;
        
        return Const_Reference_View_Iterator(const_rCollection.end());
    }

}



#endif /* Reference_View_h */
