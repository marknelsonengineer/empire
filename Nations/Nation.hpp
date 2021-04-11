///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete classes for all nations.
///
//  The documentation for classes in this file are in the .hpp file.
///
/// @file      Nation/Nation.hpp
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


namespace empire {


//////////////////////                                   //////////////////////
//////////////////////  Fundamental Nation Declarations  //////////////////////
//////////////////////                                   //////////////////////

/// The standard datatype for a Nation's ID number.
typedef uint8_t Nation_ID;


/// Maximum number of Nations.
///
/// Nation IDs will go from 0 to MAX_NATIONS -1 .  As we all know, C bases arrays
/// with 0.  Philosophically, I'd like Empire ][ to be *simple*.  To that end, 
/// I'm going to allow a country_name of 0... which will default to Pogo, the 
/// Deity.
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
/// By convention, in Empire, we call Countries, Territories, Soverign
/// States... collectively "Nation".
///
class Nation final {
public:  ////////////////  Constructor and Operator Overrides  ////////////////

	/// Creates a new Nation.
	///
	/// The default name is the nation id number (which comes from nationCounter)
	/// The default status is NEW
	Nation();


public:  ///////////////////////////  Enumerations  ///////////////////////////

	/// Nation status.
   enum Status { UNUSED     ///< Not in use
   				 ,NEW        ///< Just initialized
   				 ,VISITOR    ///< Visitor
   				 ,SANCTUARY  ///< Still in sanctuary
   				 ,ACTIVE     ///< Active (sanctuary broken)
   				 ,DEITY      ///< Deity powers
   				};

   /// Identifies the Nation's Status by type.
   typedef enum Status Status;

   /// Maximum length of a nation's name
   constinit static const uint8_t MAX_NAME = 20;


private:  //////////////////////////  Static Members  /////////////////////////
	/// An internal counter... used to assign unique ID numbers to nations and
	/// to ensure that no more than MAX_NATIONS get created.
	static Nation_ID nationCounter;


public:  //////////////////////////  Static Methods  //////////////////////////
	/// Fixup new country names.  Specifically:
	///   1. Trim leading and trailing spaces
	///   2. Replace non-alphanumeric characters with a space (sorry to non-
	///      english users, we'll sort out Unicode support in the future).
	///   3. Collapse repeating interior whitespace into one ' '.
	static std::string fixupName( const std::string_view name );


private:  /////////////////////////////  Members  /////////////////////////////

	/// The Nation ID, Nation UID and/or Nation Number...  all the same.
	const Nation_ID id;

	/// The name of our nation, up to Nation::MAX_NAME.  It should be unique 
	/// within empire.  Spaces are allowed, but not at the beginning or end.
	/// Any non-alphanumeric characters are mapped to a space (sorry non-english
	/// users, we will sort out Unicode support in the future).  Collapse 
	/// repeating interior whitespace into one ' '.
	std::string name;

	/// The status of the nation
	Status status;


public:  /////////////////////////// Getters //////////////////////////////////
	/// Get the ID of the nation
	constexpr const Nation_ID getID() const { return id; }
   
   /// Get the name of the nation
   const std::string_view getName() const { return std::string_view( name ); }
   	
   /// Get the status of a nation
   constexpr Status getStatus() const { return status; }

	/// Set the status of a nation
	/// @todo:  Validate and consider a state machine
	/// @todo:  We *really* need to work out a security model for Empire ][
   constexpr void setStatus( const Status newStatus ) { status = newStatus; }



public:  //////////////////////////// Methods /////////////////////////////////
	/// Rename a Nation.
	///
	/// The maximum size of the name is Nation::MAX_NAME.  It should be unique 
	/// within empire.  Spaces are allowed, but not at the beginning or end.
	/// Any non-alphanumeric characters are mapped to a space (sorry non-english
	/// users, we will sort out Unicode support in the future).  Collapse 
	/// repeating interior whitespace into one ' '.
	///
	/// @throws std::invalid_argument if newName is 0 length
	/// @throws std::length_error if newName exceeds Nation::MAX_NAME
	/// @throws nationNameTakenException if the name is already in use
	void rename( const std::string_view newName );


   /// Validate the health of the Nation
   const bool validate() const ;
   
   ///@TODO Implement a dump() function.  Question:  Where should the dump 
   ///      output to?  Console or log?

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
	Nations(token) ;

private:  /////////////////////////////  Members  /////////////////////////////
	Nation nations[MAX_NATIONS];

	/// Map of Nation names to Index.
	std::map<std::string_view, Nation_ID> nameMap;

	/// Declare Nation::rename() to be a friend of Nations... so it can directly
	/// access nameMap;
	friend void Nation::rename( std::string_view newName );

public:  //////////////////////////// Methods /////////////////////////////////

	/// Get a Nation (by Nation ID)
	///
	/// Although C will create a Nation[0], we are not going to use it.  This way
	/// Nation IDs, which start with 1, will match up with their array index.
	///
	/// @throws std::out_of_range if index < 1 or > MAX_NATIONS
	Nation& get1 ( const Nation_ID index );
	
	/// Get a Nation (by name)
	///
	/// @throes std::out_of_range if name is not found
	Nation& get1 ( const std::string_view name );
		
	/// Return true if name is a Nation
	bool isNation( const std::string_view name ) const;

   /// Validate the health of the Nations container
   bool validate() ;
   ///@TODO Implement a dump() function

};  // class Nations

}  // namespace empire
