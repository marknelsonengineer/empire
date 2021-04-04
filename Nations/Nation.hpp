///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete class for all nations.
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

namespace empire {


//////////////////////                                   //////////////////////
//////////////////////  Fundamental Nation Declarations  //////////////////////
//////////////////////                                   //////////////////////

/// The standard datatype for a Nation's ID number.
typedef uint8_t Nation_ID;


/// Maximum number of Nations.
constinit static const Nation_ID MAX_NATIONS = 8;  // @TODO Increase to 100



//////////////////////////////                     ////////////////////////////
//////////////////////////////  Nation Exceptions  ////////////////////////////
//////////////////////////////                     ////////////////////////////

/// Thrown when someone tries to create a Nation when they shouldn't have.  This
/// should be fatal and the logic error resolved.
/// When this is thown, we are telling the caller that someone tried to create
/// a new Nation.  All nations are created when the Core starts and stored in the
/// Nations object/array.  Any other Nations should not be permitted.
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
	/// Get the ID of the nation.
   constexpr const Nation_ID getID() const;
   
   const std::string_view getName() const;


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

};  // class Nation



/// @TODO Need to figure out if we want to allow Nation 0 or start with Nation 1

//////////////////////////                             ////////////////////////
//////////////////////////  Nations Class Declaration  ////////////////////////
//////////////////////////                             ////////////////////////

/// Container holding all of the Nation objects.
class Nations final {
private:  /////////////////////////////  Members  /////////////////////////////
	static Nation nations[MAX_NATIONS];
	
	/// Map of Nation names to Index.
	static std::map<std::string_view, Nation_ID> nameMap;

	/// Declare Nation::rename() to be a friend of Nations... so it can directly
	/// access nameMap;
	friend void Nation::rename( std::string_view newName );

public:  //////////////////////////// Methods /////////////////////////////////
	
	/// Get a Nation (by Nation ID)
	///
	/// @throws std::out_of_range if index >= MAX_NATIONS
	static Nation& get ( const Nation_ID index ); 
	
   /// Validate the health of the Nations container
   static bool validate() ;

};  // class Nations

}  // namespace empire
