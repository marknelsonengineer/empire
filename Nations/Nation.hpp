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

#include <cstdint>           // For the int8_t datatypes
#include <string_view>
#include <boost/chrono.hpp>  // For time_point

namespace empire {
	
/// Maximum number of Nations.
constexpr uint8_t MAX_NATIONS = 4;  // @TODO Increase to 100


//////////////////////////                            /////////////////////////
//////////////////////////  Nation Class Declaration  /////////////////////////
//////////////////////////                            /////////////////////////


/// Concrete class for Nations.
///
/// By convention, in Empire, we will call Countries, Territories, Soverign
/// States... collectively "Nation".
///
class Nation {
public:  ////////////////  Constructor and Operator Overrides  ////////////////
	
	/// Creates a new Nation.
	constexpr Nation( const uint8_t inNumber );


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
   static const uint8_t MAX_NAME = 20;


private:  /////////////////////////////  Members  /////////////////////////////

	/// The Nation ID, Nation UID and/or Nation Number...  all the same.
	const uint8_t number;

	/// The name of our nation, up to Nation::MAX_NAME
	// std::string_view name;

	/// The status of the nation
	Status status;

public:  //////////////////////////// Methods /////////////////////////////////
	
   /// Validate the health of the Nation
   constexpr bool validate() const ;

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
