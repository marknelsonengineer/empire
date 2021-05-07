///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete classes for all nations.
///
/// @file      Nations/Nation.hpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      7 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>      // For the int8_t Nation_ID datatype
#include <string>       // For the nation's name
#include <map>          // For mapping the name to an ID number
#include <string_view>  // For the returning name as a string_view

#include "../lib/EmpireExceptions.hpp"
#include "../lib/Singleton.hpp"
#include "../lib/Iterator.hpp"

namespace empire {


//////////////////////                                   //////////////////////
//////////////////////  Fundamental Nation Declarations  //////////////////////
//////////////////////                                   //////////////////////

/// The standard datatype for a Nation's ID number.
typedef uint8_t Nation_ID;


/// Maximum number of Nations.
///
/// Nation IDs will go from 0 to MAX_NATIONS-1 .  As we all know, C bases arrays
/// with 0.  Philosophically, I'd like to keep the Empire ][ codebase simple.
/// To that end, I'm going to allow a country_name of 0... which will default to
/// Pogo, the Deity.
constinit static const Nation_ID MAX_NATIONS = 8;  // @TODO Increase to 100



//////////////////////////////                     ////////////////////////////
//////////////////////////////  Nation Exceptions  ////////////////////////////
//////////////////////////////                     ////////////////////////////

/// Thrown when someone tries to create a Nation when they shouldn't have.  This
/// should be fatal. When this is thown, we are telling the caller that someone
/// tried to create a new Nation.  All nations are created when the Core starts
/// and stored in the Nations object/array.  Any other Nations should not be
/// permitted.
struct nationLimitExceededException: virtual empireException { };

/// Thrown when someone tries to copy another Nation's name.
struct nationNameTakenException: virtual empireException { };

/// On a NationLimitExceededException, this holds the MAX_NATIONS limit.
typedef boost::error_info<struct tag_maxNations, Nation_ID> errinfo_maxNations;

/// On a NationLimitExceededException, this holds the current nationCounter.
typedef boost::error_info<struct tag_requestedId, Nation_ID> errinfo_currentNationCounter;

/// On a nationNameTakenException, this holds the ID of a duplicate nation
typedef boost::error_info<struct tag_requestedId, Nation_ID> errinfo_NationID;

/// On a nationNameTakenException, this holds the name of the nation
typedef boost::error_info<struct tag_requestedId, std::string_view> errinfo_NationName;



//////////////////////////                            /////////////////////////
//////////////////////////  Nation Class Declaration  /////////////////////////
//////////////////////////                            /////////////////////////


/// Concrete class for a Nation.
///
/// By convention, in Empire, we call Countries, Territories, and/or Soverign
/// States... collectively "Nation".
///
class Nation final {
public:  ////////////////  Constructor and Operator Overrides  ////////////////

	/// Creates a new Nation.
	///
	/// The default name is the nation id number (which comes from nationCounter)
	/// The default status is NEW
	Nation() ;


public:  ///////////////////////////  Enumerations  ///////////////////////////

	/// Nation status.
   enum Status { UNUSED     ///< Not in use
   				 ,NEW        ///< Just initialized
   				 ,VISITOR    ///< Visitor
   				 ,SANCTUARY  ///< Still in sanctuary
   				 ,ACTIVE     ///< Active (sanctuary broken)
   				 ,DEITY      ///< Deity powers
   				} ;

   /// Identifies the Nation's Status by type.
   typedef enum Status Status ;

   /// Maximum length of a nation's name
   constinit static const uint8_t MAX_NAME = 20 ;


private:  //////////////////////////  Static Members  /////////////////////////
	/// An internal counter... used to assign unique ID numbers to nations and
	/// to ensure that no more than MAX_NATIONS get created.
	static Nation_ID nationCounter ;


public:  //////////////////////////  Static Methods  //////////////////////////
	/// Fixup new country names.  Specifically:
	///   1. Trim leading and trailing spaces
	///   2. Replace non-alphanumeric characters with a space (apoligies to non-
	///      english users, we'll sort out [Unicode](https://unicode.org)
	///      support in the future).
	///   3. Collapse repeating interior whitespace into one ' '.
	static std::string fixupName( const std::string_view name ) ;


private:  /////////////////////////////  Members  /////////////////////////////

	/// The Nation ID, Nation UID and/or Nation Number...  all the same.
	const Nation_ID id ;

	/// The name of our nation, up to Nation::MAX_NAME.  It should be unique
	/// within empire.  Spaces are allowed, but not at the beginning or end.
	/// Any non-alphanumeric characters are mapped to a space (sorry non-english
	/// users, we will sort out [Unicode](https://unicode.org) support in the
	/// future).  Collapse repeating interior whitespace into one ' '.
	std::string name ;

	/// The status of the nation
	Status status ;


public:  /////////////////////////// Getters //////////////////////////////////
	/// Get the ID of the nation
	constexpr const Nation_ID getID() const { return id; }

   /// Get the name of the nation
   const std::string_view getName() const { return std::string_view( name ); }

   /// Get the status of a nation
   constexpr Status getStatus() const { return status; }

	/// Set the status of a nation
	///
	/// @todo:  Validate and consider a state machine
	/// @todo:  We *really* need to work out a security model for Empire ][
   constexpr void setStatus( const Status newStatus ) { status = newStatus; }



public:  //////////////////////////// Methods /////////////////////////////////
	/// Rename a Nation.
	///
	/// The maximum size of the name is Nation::MAX_NAME.  It should be unique
	/// within empire.  Spaces are allowed, but not at the beginning or end.
	/// Any non-alphanumeric characters are mapped to a space (sorry non-english
	/// users, we will sort out [Unicode](https://unicode.org) support in the
	/// future).  Collapse repeating interior whitespace into one ' '.
	///
	/// @throws std::invalid_argument if newName is 0 length
	/// @throws std::length_error if newName exceeds Nation::MAX_NAME
	/// @throws nationNameTakenException if the name is already in use
	void rename( const std::string_view newName ) ;


	/// Validate the health of a Nation object (from the code's perspective...
	/// from the game's perspective, you're on your own).
	///
	/// @throws BOOST_ASSERT if the validation fails.  A server-level wrapper can
	///         catch the exception -- so we can get a stack trace to the root
	///         cause.
	///
	/// @returns true if the object is valid -- so we can easily test for
	///          validity in BOOST_ASSERT().
   const bool validate() const ;


	/// Dump the current state of Nation to the TRACE_LOG
	void dump() const;

};  // class Nation



//////////////////////////                             ////////////////////////
//////////////////////////  Nations Class Declaration  ////////////////////////
//////////////////////////                             ////////////////////////

/// Container holding all of the Nation objects.
///
/// @pattern Singleton:  Nations is a singleton
///
class Nations final : public Singleton<Nations>{
public:  ///////////////////////// Constructors ///////////////////////////////
	/// Creates and initializes the Nations of Empire ][.
	///
	/// token Is a protected Singleton struct... thereby preventing
	/// non-inherited classes from invoking this constructor.
	Nations( token ) ;


private:  /////////////////////////////  Members  /////////////////////////////
	/// Array holding all of the Nations.  This will likely be a hotspot for
	/// Empire ][.
	///
	/// @internal
	/// I've tried making this compiletime static and runtime static, but
	/// becuase Nation needs complex initialization logic, we need a full-up
	/// initializer.  So, I've decided to make Nations a singleton and hold
	/// nations as an array.
	Nation nations[MAX_NATIONS] ;
//	blArray<Nation, MAX_NATIONS> nations;

	/// Map of Nation names to Index.
	std::map<std::string_view, Nation_ID> nameMap ;

	/// Declare Nation::rename() to be a friend of Nations... so it can directly
	/// access nameMap;
	friend void Nation::rename( std::string_view newName ) ;


public:  //////////////////////////// Methods /////////////////////////////////

	/// Get a Nation (by Nation_ID)
	///
	/// @param[in] index Must be between 0 and MAX_NATIONS-1
	///
	/// @throws std::out_of_range if index < 0 or >= MAX_NATIONS
	Nation& operator[]( const Nation_ID index ) ;

	/// Get a Nation (by name)
	///
	/// @param[in] name The Nation's name.  It must be well formatted (run it
	///                 through Nation::fixupName first) and have a length between
	///                 1 and Nation::MAX_NAME.
	///
	/// @throws std::out_of_range if name is not found
	/// @throws std::invalid_argument if `name != fixupName( name )`
	Nation& operator[]( const std::string_view name ) ;

	/// Checks if name is in Nations
	///
	/// This functions does *not* do Nation::fixupName first... so you should
	/// fixup the name before calling this.
	///
	/// Use this to see if Nations has name... if it does, then it's safe to
	/// call `Nations::get()["your name here"]`.
	///
	/// @param[in] name The Nation's name.  If must have a length between 1 and
	///                 Nation::MAX_NAME.
	///
	/// @return true if name is in Nations.
	bool contains( const std::string_view name ) const ;

	/// Refresh nameMap
	///
	/// @throws boost_assert If there's a duplicate name.
	void refreshNameMap() ;



	typedef blRawIterator<Nation>              iterator;
	typedef blRawIterator<const Nation>        const_iterator;

	typedef blRawReverseIterator<Nation>       reverse_iterator;
	typedef blRawReverseIterator<const Nation> const_reverse_iterator;


	// /////////////////////////  Nations Iterator  ////////////////////////////
	/// Iterator of Nations
	///
	/// @see https://internalpointers.com/post/writing-custom-iterators-modern-cpp
/*
	struct Iterator {
	public:
		/// Determines the capabilities of this iterator.  Used by std::Algorithms
		/// to pick appropriate optimizations when using the Iterator.
		///
		/// @todo Figure out if input_iterator_tag is the kind of iterator we want
		using iterator_category = std::input_iterator_tag;

		/// Determines how the Iterator can compute the difference between two
		/// iterators.  We are holding Nation in an array, so this will use
		/// array-index-pointer arithmetic to comptue the difference.
		using difference_type = std::ptrdiff_t;

		using value_type = Nation;   ///< Datatype the Iterator is iterating over
		using pointer    = Nation*;  ///< Pointer type of the Iterator
		using reference  = Nation&;  ///< Reference type of the Iterator

		/// Constructor for the iterator.  It takes a pointer to the Nations class
		Iterator(pointer ptr) : m_ptr(ptr) {}

		/// Returns the Nation that the Iterator is currently at
		reference operator*() const { return *m_ptr; }

		/// Returns the const Nation that the Iterator is currently at
		//const reference operator*() const { return *m_ptr; }

		/// Returns a pointer to the Nation the Iterator is currently at
		pointer operator->() { return m_ptr; }

		/// Increments the loop count of this Iterator and then return the Iterator.
		Iterator& operator++() { m_ptr++; return *this; }

		/// Increments the loop count of this iterator, returning a copy with
		/// the value *before* the increment.
		Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		/// Iterators are equal if they both point to the same Nation.
		friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };

		/// Iteratrors are not equal if they don't point to the same Nation.
		friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		/// Pointer to a Nation in the nations array.
		pointer m_ptr;
	};
*/

public:  //////////////////////////// Methods /////////////////////////////////


	iterator begin(){ return iterator( &nations[0] ); }
	iterator end(){ return iterator( &nations[MAX_NATIONS] ); }

	const_iterator cbegin(){ return const_iterator( &nations[0] ); }
	const_iterator cend(){ return const_iterator( &nations[MAX_NATIONS] ); }

	reverse_iterator rbegin(){ return reverse_iterator( &nations[MAX_NATIONS - 1] ); }
	reverse_iterator rend(){ return reverse_iterator( &nations[-1] ); }

	const_reverse_iterator crbegin(){ return const_reverse_iterator( &nations[MAX_NATIONS - 1] ); }
	const_reverse_iterator crend(){ return const_reverse_iterator( &nations[-1] ); }


	/// Returns iterator to first Nation
	//Iterator begin() { return Iterator(&nations[0]); }

	/// Returns Iterator to a sentinal representing the last Nation (one past
	/// the actual last nation).
	//Iterator end()   { return Iterator(&nations[MAX_NATIONS]); }

	/// Return true if name is a Nation
	bool isNation( const std::string_view name ) const ;

   /// Validate the health of the Nations container
   bool validate() const ;

   /// Dump information about Nations to the TRACE_LOG
   void dump() const ;

};  // class Nations

}  // namespace empire
