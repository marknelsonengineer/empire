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
// #include <boost/bimap>
#include <array>        // For the array

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

/// Thrown when someone tries to create a Nation when they shouldn't.  This
/// should be fatal.  When thown, we are telling the caller that someone
/// tried to create a new Nation.  All nations are created when the Core starts
/// and stored in the Nations object/array.  Any other Nations should not be
/// permitted.
struct nationLimitExceededException: virtual empireException { };

/// Thrown when someone tries to copy another Nation's name.
struct nationNameTakenException: virtual empireException { };

/// On a nationLimitExceededException, this holds the MAX_NATIONS limit.
typedef boost::error_info<struct tag_maxNations, Nation_ID> errinfo_maxNations;

/// On a nationLimitExceededException, this holds the current nationCounter.
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
/// @internal This class is `final` so it can't be subclassed.
///
class Nation final {
public:  ////////////////  Constructor and Operator Overrides  ////////////////

	/// Creates a new Nation.
	///
	/// The default name is the nation id number (which comes from nationCounter)
	/// The default status is NEW
	Nation() ;


public:  ///////////////////////////  Enumerations  ///////////////////////////

	/// Nation status
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
	/// to ensure that no more than MAX_NATIONS are created.
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
   ///
   /// There's no setter.  Instead, use rename().
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
/// @internal This class is `final` so it can't be subclassed.
///
class Nations final : public Singleton<Nations> {
public:  ///////////////////////// Constructors ///////////////////////////////
	/// Creates and initializes the Nations of Empire ][.
	///
	/// `token` is a protected Singleton struct... thereby preventing
	/// non-inherited classes from invoking this constructor.
	Nations( token ) ;


private:  /////////////////////////////  Members  /////////////////////////////
	/// Array holding all of the `Nation`s.  This will likely be a hotspot for
	/// Empire ][.
	///
	/// @internal
	/// I've tried making this compiletime static and runtime static, but
	/// becuase Nation needs complex initialization logic, we need a full-up
	/// initializer.  So, I've decided to make Nations a singleton and hold
	/// nations as an array.
//	Nation nations[MAX_NATIONS] ;  /// @todo: Use blArray
//	blArray<Nation, MAX_NATIONS> nations ;
	std::array<Nation, MAX_NATIONS> nations;

	/// Map of Nation names to Index.
	std::map<std::string_view, Nation_ID> nameMap ;  /// @todo: Use bimap

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
	/// This method does *not* do Nation::fixupName first... so you should
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


	// /////////////////////////  Nations Iterator  ////////////////////////////
	/// Iterator of Nations
	///
	/// @see https://github.com/navyenzo/blIteratorAPI
	/// @see https://internalpointers.com/post/writing-custom-iterators-modern-cpp

	typedef blRawIterator<Nation>              iterator;

	/// Const Nations iterator
	typedef blRawIterator<const Nation>        const_iterator;

	/// Reverse Nations iterator
	typedef blRawReverseIterator<Nation>       reverse_iterator;

	/// Const Reverse Nations iterator
	typedef blRawReverseIterator<const Nation> const_reverse_iterator;


public:  //////////////////////////// Methods /////////////////////////////////

	/// Returns an iterator to the first Nation
	iterator begin(){ return iterator( &nations[0] ); }

	/// Returns an iterator to a sentinal representing the last Nation (one past
	/// the actual last nation).
	iterator end(){ return iterator( &nations[MAX_NATIONS] ); }

	/// Returns the constant iterator to the first Nation
	const_iterator cbegin(){ return const_iterator( &nations[0] ); }

	/// Returns a constant iterator to a sentinal representing the last Nation
	/// (one past the actual last nation).
	const_iterator cend(){ return const_iterator( &nations[MAX_NATIONS] ); }

	/// Returns a reverse iterator to the last Nation
	reverse_iterator rbegin(){ return reverse_iterator( &nations[MAX_NATIONS - 1] ); }

	/// Returns a reverse iterator to a sentinal representing the first Nation
	/// (one past the actual first nation).
	reverse_iterator rend(){ return reverse_iterator( &nations[-1] ); }

	/// Returns a constant reverse iterator to the last Nation
	const_reverse_iterator crbegin(){ return const_reverse_iterator( &nations[MAX_NATIONS - 1] ); }

	/// Returns a constant reverse iterator to a sentinal representing the
	/// first Nation (one past the actual first nation).
	const_reverse_iterator crend(){ return const_reverse_iterator( &nations[-1] ); }


	/// Return true if name is a Nation
	bool isNation( const std::string_view name ) const ;

   /// Validate the health of the Nations container
   bool validate() const ;

   /// Dump information about Nations to the TRACE_LOG
   void dump() const ;

/*
/// Experimental . discard if I can get this working
private:  //////////////////////////// Methods /////////////////////////////////
	const inline blArray<Nation, MAX_NATIONS> get_nations_as_const() const { return nations; }
*/

};  // class Nations
}  // namespace empire
