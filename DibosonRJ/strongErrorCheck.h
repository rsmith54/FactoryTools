// Dear emacs, this is -*- c++ -*-
// $Id: errorcheck.h 299732 2014-03-27 17:41:34Z krasznaa $
#ifndef DIBOSONRJ_ERRORCHECK_H
#define DIBOSONRJ_ERRORCHECK_H

#define STRONG_CHECK( ARG )                                     \
   do {                                                  \
      const bool result = ARG;                           \
      if( ! result ) {                                   \
         ::Error( __PRETTY_FUNCTION__, "Failed to execute: \"%s\"", \
                  #ARG );                                \
         return 1;                                       \
      }                                                  \
   } while( false )

#endif // DIBOSONRJ_ERRORCHECK_H
