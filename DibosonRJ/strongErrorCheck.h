// Dear emacs, this is -*- c++ -*-
// $Id: errorcheck.h 299732 2014-03-27 17:41:34Z krasznaa $
#ifndef RJIGSAWTOOLS_ERRORCHECK_H
#define RJIGSAWTOOLS_ERRORCHECK_H

#define STRONG_CHECK( ARG )                                     \
   do {                                                  \
      const bool result = ARG;                           \
      if( ! result ) {                                   \
         ::Error( __PRETTY_FUNCTION__, "Failed to execute: \"%s\"", \
                  #ARG );                                \
	return EL::StatusCode::FAILURE;		 \
      }                                                  \
   } while( false )

#endif // RJIGSAWTOOLS_ERRORCHECK_H
