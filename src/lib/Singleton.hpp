///////////////////////////////////////////////////////////////////////////////
//  Empire V - What you do is what you do
//
/// A generic Singleton template
///
/// @see https://codereview.stackexchange.com/questions/173929/modern-c-singleton-template
///
/// @file      lib/Singleton.hpp
/// @author    Mark Nelson <mr_nelson@icloud.com>
/// @copyright (c) 2021 Mark Nelson.  All rights reserved.
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>

#include <boost/core/type_name.hpp>        // For type_name
#include <boost/uuid/uuid.hpp>             // For uuid
#include <boost/uuid/uuid_generators.hpp>  // For nil_generator() random_generator()
#include <boost/uuid/uuid_io.hpp>          // For uuid::to_string()

// #include <memory>  // For unique_ptr make_unique()

#include "../typedefs.h"  // For singleton_counter_t

namespace empire {

/// Template for a Singleton class
///
/// This is more like a Singleton helper.  Users of Singleton will inherit this
/// class and create their own Singleton like this:
///
///     /// Create a basic Singleton class
///     class TestSingleton1 final : public Singleton< TestSingleton1 > {
///     public:
///        explicit TestSingleton1( [[maybe_unused]] token singletonToken ) {
///           cout << "Constructor1  " << TestSingleton1::info() << endl;
///        }
///
///        // Do something useful with this Singleton
///        void use() const {
///           cout << "Use1 " << TestSingleton1::info() << endl;
///        }
///     };
///
/// This of this as more of a helper to manage Singletons.  There are many ways
/// you can get something that's not a Singleton to compile and run.  As soon
/// as you start using them, they should start throwing exceptions.  In other
/// words the guarantees of Singleton are runtime and (I'd estimate) to be
/// relatively weak.
///
/// This class is also, as it's written, not thread-safe.
///
/// Singletons that need configuration parameters should probably... @todo Finish this
///
/// @pattern Singleton:  This is a base class for Singleton
/// @tparam T This derived class will be a Singleton
template< typename T >
class Singleton {
public:  // /////////////////// Constructors & Destructors /////////////////////

   /// This is mechanism Singleton uses to ensure objects inherit from Singleton.
   /// Inherited, concrete Singleton classes should pass `token` to their
   /// constructors.  If you can't access #Singleton.token, then you are not
   /// overriding Singleton.
   using token = struct token {};

   Singleton( Singleton& ) = delete;  ///< Disable copy constructor
   Singleton( const Singleton& ) = delete;  ///< Disable copy constructor
   Singleton( const Singleton&& ) = delete;  ///< Disable move constructor

   Singleton& operator=( const Singleton ) = delete;  ///< Disable copy assignment
   Singleton& operator=( const Singleton& ) = delete;  ///< Disable copy assignment
   Singleton& operator=( const Singleton&& ) = delete;  ///< Disable move assignment

   /// Destructor for Singleton
   virtual ~Singleton() {
      std::cout << "Destructor for " << info() << std::endl;

      s_pStaticInstance = nullptr;
      uuid = boost::uuids::nil_generator()();
      destructCounter += 1;
      validate();
   };


public:  // ///////////////////////// Static Methods ///////////////////////////

   static T& get();  // Defined below, so it can use `inline`

   /// Return a string about this object
   ///
   ///     TestSingleton1 UUID=0982ec0e-34c2-4769-837e-abd90834e407 constructed 14 times destroyed 13 times
   ///
   /// @return An info string
   static std::string info() {
      std::string infoString {} ;

      infoString += boost::core::type_name<T>();
      infoString += " UUID=" + to_string( uuid );
      infoString += " constructed " + std::to_string( constructCounter ) + " times";
      infoString += " destroyed " + std::to_string( destructCounter ) + " times";
      return infoString;
   }

   /// @return Number of times the Singleton has been constructed
   [[nodiscard]] static singleton_counter_t getConstructedCount() {
      return constructCounter;
   }

   /// @return Number of times the Singleton has been destroyed
   [[nodiscard]] static singleton_counter_t getDestroyedCount() {
      return destructCounter;
   }

   /// Validate the health of the Singleton
   static void validate() {
      /// @throws logic_error if the info() string is not constructed correctly
      if( info().find( "UUID" ) == std::string::npos ) {
         throw std::logic_error( "The Singleton's info string was not constructed correctly");
      }

      if( s_pStaticInstance == nullptr ) {
         if( uuid != boost::uuids::nil_generator()() ) {
            /// @throws logic_error if an empty Singleton has a populated #uuid
            throw std::logic_error( "UUID should be nil for an empty Singleton");
         }
         if( constructCounter != destructCounter ) {
            /// @throws range_error if an empty Singleton's #constructCounter and #destructCounter are not equal
            throw std::range_error( "The constructCounter should equal the destructCounter in an empty Singleton" );
         }
      } else {
         /// @throws range_error if a populated Singleton's #constructCounter is <= 0
         if( constructCounter <= 0 ) {
            throw std::range_error( "A Singleton's constructCounter should be > 0" );
         }

         /// @throws range_error if a populated Singleton's #destructCounter is anything other than #constructCounter - 1
         if( destructCounter != constructCounter - 1 ) {
            throw std::range_error( "A Singleton's constructCounter should be one more than it's destructCounter" );
         }

         /// @throws logic_error if a populated Singleton's #uuid is not set
         if( uuid == boost::uuids::nil_generator()() ) {
            throw std::logic_error( "A Singleton's UUID should be set");
         }
      }
   }

   /// @return A Universally Unique IDentifier or UUID for the Singleton
   [[nodiscard]] boost::uuids::uuid getUUID() const {
      return uuid;
   }


   /// Erase the Singleton instance
   ///
   /// Singleton guarantees that only one instance of the object can exist at a
   /// time, but it does not guarantee that it can't be deleted and re-created.
   /// The ability to delete and recreate a Singleton is beneficial for unit
   /// testing and is one of the (few) things that makes having a Singleton
   /// palatable.
   static void erase() {
      std::cout << "Erase " << info() << std::endl;

      if( s_pStaticInstance == nullptr ) {
         return;
      }

      // Fires ~Singleton which resets all the member variables
      delete s_pStaticInstance;
   }


protected:  // /////////////// Protected Methods & Members /////////////////////

   /// Construct an empty Singleton
   ///
   /// The Singleton is initially empty and is only instantiated when
   /// a client calls get().  This way, we don't pay for what we don't use.
   ///
   /// This is protected so `<T>` can override it should it choose.
   Singleton() {
      std::cout << "Base class constructor for " << info() << std::endl;

      if( s_pStaticInstance != nullptr ) {
         throw std::logic_error( "Attempt to create a new Singleton on top of an existing one" );
      }

      uuid = boost::uuids::random_generator()();
      constructCounter += 1;
      s_pStaticInstance = dynamic_cast<T*>(this);

      std::cout << "Instantiated " << info() << std::endl;
      // validate();  // It's not safe to call validate() at this point because
                      // the child-class's constructor has not run yet.
   }

private:  // //////////////////// Private Static Members ///////////////////////
   static T* s_pStaticInstance;  ///< Pointer to the Singleton instance

   static boost::uuids::uuid uuid;               ///< UUID for this Singleton
   static singleton_counter_t constructCounter;  ///< Number of times this Singleton has been constructed
   static singleton_counter_t destructCounter;   ///< Number of times this Singleton has been destroyed

}; // Singleton


template< typename T >
T* Singleton< T >::s_pStaticInstance = nullptr;

template< typename T >
boost::uuids::uuid Singleton< T >::uuid = boost::uuids::nil_generator()();

template< typename T >
singleton_counter_t Singleton< T >::constructCounter = 0;

template< typename T >
singleton_counter_t Singleton< T >::destructCounter = 0;


/// Get an instance of a Singleton
///
/// Inlined to maximize performance
///
/// @tparam T The Singleton class
/// @return The one and only instance of `T`
template< typename T >
inline T& Singleton< T >::get() {

   if( s_pStaticInstance == nullptr ) {
      /// @todo Implement smart pointers
      s_pStaticInstance = new T( token { } );
   }

   validate();

   return *s_pStaticInstance;
}

}  // namespace empire
