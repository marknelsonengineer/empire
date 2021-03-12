///////////////////////////////////////////////////////////////////////////////
//  Empire ][
//
/// Concrete class for all nations.
///
//  The documentation for classes in this file are in the .hpp file.
/// data that varies between instances of a commodity.
///
/// @file      Nation.hpp
/// @version   1.0 - Initial version
///
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @date      7 Mar 2021
/// @copyright (c) 2021 Mark Nelson
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>  // For the int8_t datatypes
#include <string>   // For the nation's name

namespace empire {

/// The standard datatype for a Nation's ID number.	
typedef uint8_t Nation_ID;


/// Maximum number of Nations.
///
/// @internal This is closely tied to the `Nations::nations[MAX_NATIONS]` 
///           definition in Nation.cpp.  The number of Nations must be
///           hand-created in there -- as they are static.
constinit static const Nation_ID MAX_NATIONS = 25;  // @TODO Increase to 100


//////////////////////////                            /////////////////////////
//////////////////////////  Nation Class Declaration  /////////////////////////
//////////////////////////                            /////////////////////////


/// Concrete class for Nations.
///
///
/// Nation's constructor is a consteval.  This means it is created at compile 
/// time and there is no other way to instantiate a nation.
///
/// By convention, in Empire, we will call Countries, Territories, Soverign
/// States... collectively "Nation".
///
class Nation {
public:  ////////////////  Constructor and Operator Overrides  ////////////////
	
	/// Creates a new Nation.
	/// 
	/// The default name is inNumber
	/// The default status is NEW
	Nation( const Nation_ID inNumber );


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


private:  /////////////////////////////  Members  /////////////////////////////

	/// The Nation ID, Nation UID and/or Nation Number...  all the same.
	const Nation_ID id;

	/// The name of our nation, up to Nation::MAX_NAME
	std::string name;

	/// The status of the nation
	Status status;


public:  /////////////////////////// Getters //////////////////////////////////
	/// Get the ID of the nation.
   constexpr const Nation_ID getID() const;


public:  //////////////////////////// Methods /////////////////////////////////
	
   /// Validate the health of the Nation
   const bool validate() const ;

};  // class Nation


/// Container holding all of the Nation objects.
class Nations {
private:  /////////////////////////////  Members  /////////////////////////////
	static constinit const Nation nations[MAX_NATIONS];
	
	
public:  //////////////////////////// Methods /////////////////////////////////
   /// Validate the health of the Nations container
   static bool validate() ;
	
};  // class Nations

}  // namespace empire
