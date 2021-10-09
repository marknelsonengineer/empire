///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Templates used to create array-based iterators.
/// This is a header-file only.
///
/// @todo      Cleanup the documentation
///
/// @file      lib/Iterator.hpp
/// @version   1.0 - Initial version
///
/// @see https://github.com/navyenzo/blIteratorAPI
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      4 May 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <iterator>


/// Define a Raw Iterator class based on the datatype in `blDataType`
///
template<typename blDataType>
class blRawIterator : public std::iterator<std::contiguous_iterator_tag
                                          ,blDataType
                                          ,ptrdiff_t
                                          ,blDataType*
                                          ,blDataType& > {
public:

    blRawIterator(blDataType* ptr = nullptr){m_ptr = ptr;}
    blRawIterator(const blRawIterator<blDataType>& rawIterator) = default;
    ~blRawIterator(){}

    blRawIterator<blDataType>&                  operator=(const blRawIterator<blDataType>& rawIterator) = default;
    blRawIterator<blDataType>&                  operator=(blDataType* ptr){m_ptr = ptr;return (*this);}

    operator                                    bool()const {
        if(m_ptr)
            return true;
        else
            return false;
    }

    bool                                        operator==(const blRawIterator<blDataType>& rawIterator)const{return (m_ptr == rawIterator.getConstPtr());}
    bool                                        operator!=(const blRawIterator<blDataType>& rawIterator)const{return (m_ptr != rawIterator.getConstPtr());}

    blRawIterator<blDataType>&                  operator+=(const ptrdiff_t& movement){m_ptr += movement;return (*this);}
    blRawIterator<blDataType>&                  operator-=(const ptrdiff_t& movement){m_ptr -= movement;return (*this);}
    blRawIterator<blDataType>&                  operator++(){++m_ptr;return (*this);}
    blRawIterator<blDataType>&                  operator--(){--m_ptr;return (*this);}
    blRawIterator<blDataType>                   operator++(int){auto temp(*this);++m_ptr;return temp;}
    blRawIterator<blDataType>                   operator--(int){auto temp(*this);--m_ptr;return temp;}
    blRawIterator<blDataType>                   operator+(const ptrdiff_t& movement){auto oldPtr = m_ptr;m_ptr+=movement;auto temp(*this);m_ptr = oldPtr;return temp;}
    blRawIterator<blDataType>                   operator-(const ptrdiff_t& movement){auto oldPtr = m_ptr;m_ptr-=movement;auto temp(*this);m_ptr = oldPtr;return temp;}

    ptrdiff_t                                   operator-(const blRawIterator<blDataType>& rawIterator){return std::distance(rawIterator.getPtr(),this->getPtr());}

    blDataType&                                 operator*()       { return *m_ptr; }
    const blDataType&                           operator*() const { return *m_ptr; }
    blDataType*                                 operator->()      { return  m_ptr; }

    blDataType*                                 getPtr()const{return m_ptr;}
    const blDataType*                           getConstPtr()const{return m_ptr;}

protected:

    blDataType*                                 m_ptr;
} ;  // blRawIterator



template<typename blDataType>
class blRawReverseIterator : public blRawIterator<blDataType> {
public:

    blRawReverseIterator(blDataType* ptr = nullptr):blRawIterator<blDataType>(ptr){}
    blRawReverseIterator(const blRawIterator<blDataType>& rawIterator){this->m_ptr = rawIterator.getPtr();}
    blRawReverseIterator(const blRawReverseIterator<blDataType>& rawReverseIterator) = default;
    ~blRawReverseIterator(){}

    blRawReverseIterator<blDataType>&           operator=(const blRawReverseIterator<blDataType>& rawReverseIterator) = default;
    blRawReverseIterator<blDataType>&           operator=(const blRawIterator<blDataType>& rawIterator){this->m_ptr = rawIterator.getPtr();return (*this);}
    blRawReverseIterator<blDataType>&           operator=(blDataType* ptr){this->setPtr(ptr);return (*this);}

    blRawReverseIterator<blDataType>&           operator+=(const ptrdiff_t& movement){this->m_ptr -= movement;return (*this);}
    blRawReverseIterator<blDataType>&           operator-=(const ptrdiff_t& movement){this->m_ptr += movement;return (*this);}
    blRawReverseIterator<blDataType>&           operator++(){--this->m_ptr;return (*this);}
    blRawReverseIterator<blDataType>&           operator--(){++this->m_ptr;return (*this);}
    blRawReverseIterator<blDataType>            operator++(int){auto temp(*this);--this->m_ptr;return temp;}
    blRawReverseIterator<blDataType>            operator--(int){auto temp(*this);++this->m_ptr;return temp;}
    blRawReverseIterator<blDataType>            operator+(const int& movement){auto oldPtr = this->m_ptr;this->m_ptr-=movement;auto temp(*this);this->m_ptr = oldPtr;return temp;}
    blRawReverseIterator<blDataType>            operator-(const int& movement){auto oldPtr = this->m_ptr;this->m_ptr+=movement;auto temp(*this);this->m_ptr = oldPtr;return temp;}

    ptrdiff_t                                   operator-(const blRawReverseIterator<blDataType>& rawReverseIterator){return std::distance(this->getPtr(),rawReverseIterator.getPtr());}

    blRawIterator<blDataType>                   base(){blRawIterator<blDataType> forwardIterator(this->m_ptr); ++forwardIterator; return forwardIterator;}
} ;  // blRawReverseIterator



///////////////////////////////////////////////////////////////////////////////



//template<std::contiguous_iterator blDataType, size_t blArraySize>
template<typename blDataType, size_t blArraySize>
class blArray {
public: // Public typedefs

    typedef blRawIterator<blDataType>                       iterator;
    typedef blRawIterator<const blDataType>                 const_iterator;

    typedef blRawReverseIterator<blDataType>                reverse_iterator;
    typedef blRawReverseIterator<const blDataType>          const_reverse_iterator;

public: // Constructors and destructors

    // Default constructor
    blArray();

    // Copy constructor
    blArray( const blArray<blDataType, blArraySize>& array ) = default;

    // Move constructor
    blArray( blArray<blDataType, blArraySize>&& array ) = default;

    // Initializer-list constructor
    blArray( std::initializer_list<blDataType> theList );

    // Copy constructor from a different size and/or different data type array
    template<typename blDataType2,size_t blArraySize2>
    blArray( const blArray<blDataType2, blArraySize2>& array );

    // Constructor using a raw array
    template<typename blDataType2,size_t blArraySize2>
    blArray( const blDataType2 (&staticArray)[blArraySize2] );

    // Constructor using iterators
    template<typename blIteratorType>
    blArray( blIteratorType sourceBegin, blIteratorType sourceEnd );

    // Destructor
    ~blArray() {
    }

public: // Assignment operators

    blArray<blDataType,blArraySize>&                        operator=(const blArray<blDataType,blArraySize>& array) = default;

    template<typename blDataType2,size_t blArraySize2>
    blArray<blDataType,blArraySize>&                        operator=(const blArray<blDataType2,blArraySize2>& array);

    blArray<blDataType,blArraySize>&                        operator=(std::initializer_list<blDataType> theList);

    template<typename blDataType2,size_t blArraySize2>
    blArray<blDataType,blArraySize>&                        operator=(const blDataType2 (&staticArray)[blArraySize2]);

public: // Public functions

    // Additional operator overloads
    bool                                                    operator==(const blArray<blDataType, blArraySize>& array)const;
    bool                                                    operator!=(const blArray<blDataType, blArraySize>& array)const;

    // Element access functions
    blDataType&                                             operator[](const size_t elementIndex);
    const blDataType&                                       operator[](const size_t elementIndex)const;

/// MARK:  I added this code here... but I'm not sure it's right
//	blDataType& operator*() const { return *m_ptr; }

    // Functions used to return the array size
    size_t                                                  length()const;
    size_t                                                  size()const;
    size_t                                                  max_size()const;
    bool                                                    empty()const;

    // Function used to swap values between two arrays of equal length

    void                                                    swap(const blArray<blDataType, blArraySize>& Array);

    // Function used to fill the array with a specified value

    void                                                    fill(const blDataType& value);

    // Function used to return a reference to the front and back elements
    blDataType&                                             front();
    const blDataType&                                       front()const;

    blDataType&                                             back();
    const blDataType&                                       back()const;

    // Functions used to get a raw pointer to the first element
    blDataType*                                             data();
    const blDataType*                                       data()const;
    const blDataType*                                       c_str()const;

    // Functions used to get iterators to this container
    iterator                                                begin();
    iterator                                                end();
    const_iterator                                          cbegin()const;
    const_iterator                                          cend()const;

    reverse_iterator                                        rbegin();
    reverse_iterator                                        rend();
    const_reverse_iterator                                  crbegin()const;
    const_reverse_iterator                                  crend()const;

private: // Private data

    /// The raw array
    blDataType                                              m_container[blArraySize];

    /// The null end element is useful when using this array as a constant
    /// character string
    blDataType                                              m_nullChar;

};  // blArray


// Default constructor
template<typename blDataType, size_t blArraySize>
//inline blArray<blDataType, blArraySize>::blArray() : m_nullChar(0) {
inline blArray<blDataType, blArraySize>::blArray() {
}


// Initializer-list constructor
template<typename blDataType, size_t blArraySize>
inline blArray<blDataType, blArraySize>::blArray(std::initializer_list<blDataType> theList) : m_nullChar(0) {
    auto Iter1 = this->begin();
    auto Iter2 = theList.begin();

    for(;
        Iter1 != this->end(),Iter2 != theList.end();
        ++Iter1,++Iter2) {
        (*Iter1) = (*Iter2);
    }
}


/// Copy constructor
template<typename blDataType, size_t blArraySize>
template<typename blDataType2, size_t blArraySize2>
inline blArray<blDataType, blArraySize>::blArray(const blArray<blDataType2, blArraySize2>& array) : m_nullChar(0) {
	auto Iter1 = this->begin();
	auto Iter2 = array.begin();

	for( ; Iter1 != this->end(),Iter2 != array.end() ; ++Iter1,++Iter2) {
		(*Iter1) = (*Iter2);
	}
}


// Move constructor
template<typename blDataType, size_t blArraySize>
template<typename blDataType2, size_t blArraySize2>
inline blArray<blDataType, blArraySize>::blArray(const blDataType2 (&staticArray)[blArraySize2]) : m_nullChar(0) {
	auto Iter1 = this->begin();

	for( size_t i = 0 ; Iter1 != this->end(), i < blArraySize2 ; ++Iter1,++i) {
		(*Iter1) = staticArray[i];
	}
}


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
template<typename blIteratorType>
inline blArray<blDataType,blArraySize>::blArray(blIteratorType sourceBegin,
                                                blIteratorType sourceEnd) : m_nullChar(0)
{
    auto Iter1 = this->begin();

    for(size_t i = 0;
        Iter1 != this->end(),sourceBegin != sourceEnd;
        ++Iter1,sourceBegin)
    {
        (*Iter1) = (*sourceBegin);
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
template<typename blDataType2,size_t blArraySize2>
inline blArray<blDataType,blArraySize>& blArray<blDataType,blArraySize>::operator=(const blArray<blDataType2,blArraySize2>& array)
{
    if(this != &array)
    {
        auto iter1 = this->begin();
        auto iter2 = array.begin();

        for(;
            iter1 != this->end(),iter2 != array.end();
            ++iter1,++iter2)
        {
            (*iter1) = (*iter2);
        }
    }

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline blArray<blDataType,blArraySize>& blArray<blDataType,blArraySize>::operator=(std::initializer_list<blDataType> theList)
{
    auto Iter1 = this->begin();
    auto Iter2 = theList.begin();

    for(;
        Iter1 != this->end(),Iter2 != theList.end();
        ++Iter1,++Iter2)
    {
        (*Iter1) = (*Iter2);
    }

    return (*this);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline bool blArray<blDataType,blArraySize>::operator==(const blArray<blDataType,blArraySize>& array)const
{
    if(this == &array)
        return true;

    auto iter1 = cbegin();
    auto iter2 = array.cbegin();

    for(;
        iter1 != cend();
        ++iter1,++iter2)
    {
        if((*iter1) != (*iter2))
            return false;
    }

    return true;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline bool blArray<blDataType,blArraySize>::operator!=(const blArray<blDataType,blArraySize>& array)const
{
    return !( (*this) == array );
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline blDataType& blArray<blDataType,blArraySize>::operator[](const size_t elementIndex)
{
    return m_container[elementIndex];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline const blDataType& blArray<blDataType,blArraySize>::operator[](const size_t elementIndex)const
{
    return m_container[elementIndex];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline size_t blArray<blDataType,blArraySize>::length()const
{
    return blArraySize;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline size_t blArray<blDataType,blArraySize>::size()const
{
    return blArraySize;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline size_t blArray<blDataType,blArraySize>::max_size()const
{
    return blArraySize;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline bool blArray<blDataType,blArraySize>::empty()const
{
    return (blArraySize != 0);
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline void blArray<blDataType,blArraySize>::swap(const blArray<blDataType, blArraySize>& Array)
{
    blDataType TempValue;

    for(size_t i = 0; i < blArraySize; ++i)
    {
        TempValue = m_container[i];
        m_container[i] = Array[i];
        Array[i] = TempValue;
    }
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline void blArray<blDataType,blArraySize>::fill(const blDataType& value)
{
    for(size_t i = 0; i < blArraySize; ++i)
        m_container[i] = value;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline blDataType& blArray<blDataType,blArraySize>::front()
{
    return m_container[0];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline const blDataType& blArray<blDataType,blArraySize>::front()const
{
    return m_container[0];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline blDataType& blArray<blDataType,blArraySize>::back()
{
    return m_container[blArraySize - 1];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline const blDataType& blArray<blDataType,blArraySize>::back()const
{
    return m_container[blArraySize - 1];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline blDataType* blArray<blDataType,blArraySize>::data()
{
    return &m_container[0];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline const blDataType* blArray<blDataType,blArraySize>::data()const
{
    return &m_container[0];
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
inline const blDataType* blArray<blDataType,blArraySize>::c_str()const {
    return &m_container[0];
}
//-------------------------------------------------------------------


//////////////////////////   The Iterators   ////////////////////////

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::iterator blArray<blDataType, blArraySize>::begin() {
    return &(m_container[0]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::iterator blArray<blDataType, blArraySize>::end() {
    return &(m_container[blArraySize]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::const_iterator blArray<blDataType, blArraySize>::cbegin() const {
    return &(m_container[0]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::const_iterator blArray<blDataType, blArraySize>::cend() const {
    return &(m_container[blArraySize]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType,blArraySize>::reverse_iterator blArray<blDataType,blArraySize>::rbegin() {
    return &(m_container[blArraySize - 1]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::reverse_iterator blArray<blDataType, blArraySize>::rend() {
    return &(m_container[-1]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::const_reverse_iterator blArray<blDataType, blArraySize>::crbegin()const {
    return &(m_container[blArraySize - 1]);
}

template<typename blDataType, size_t blArraySize>
inline typename blArray<blDataType, blArraySize>::const_reverse_iterator blArray<blDataType, blArraySize>::crend()const {
    return &(m_container[-1]);
}



/// Function used to automatically deduct
/// the type and size of array from a raw
/// array
//-------------------------------------------------------------------
template<typename blDataType,size_t blArraySize>
blArray<blDataType,blArraySize> getArray(const blDataType (&staticArray)[blArraySize])
{
    return blArray<blDataType,blArraySize>(staticArray);
}
//-------------------------------------------------------------------


/// Automatically deduct the type and size of array from a NULL
/// terminated array
///
template<typename blDataType,size_t blArraySize>
blArray<blDataType,blArraySize - 1> getArrayFromNullTerminated(const blDataType (&staticArray)[blArraySize])
{
    return blArray<blDataType,blArraySize - 1>(staticArray);
}
