/* Copyright (c) 2006-2023 SpaceSUR and CONAE

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

This file is part of the SuriLib project and its derived programs. You must retain
this copyright notice and license text in all copies or substantial
portions of the software.

If you develop a program based on this software, you must provide a visible
notice to its users that it contains code from the SuriLib project and provide
a copy of this license. The notice should be displayed in a way that is easily
accessible to users, such as in the program's "About" box or documentation.

For more information about SpaceSUR, visit <https://www.spacesur.com>.
For more information about CONAE, visit <http://www.conae.gov.ar/>. */

#ifndef LOGMACROS_H_
#define LOGMACROS_H_

// Includes suri
#include "suri/messages.h"

// ------------------------------------------------------------------------------
// --REPORT MACROS--------------------------------------------------------------

#ifdef _MSC_VER
#  ifdef __FUNCTION__
#     define __func__ __FUNCTION__
#  else  // __FUNCTION__
#     define __func__ __FUNCDNAME__
#  endif // __FUNCTION__
#endif   // _MSC_VER
#if !defined(__GNUC__) && !defined(__func__)
#  ifdef __DEBUG__
#     warning "Definiendo __func__ como string vacio"
#  endif // __DEBUG__
#  define __func__ ""
#endif   // __func__
#ifndef __class__
#  include <string.h>
#  define __class__ ( (strchr(__FILE__, '.') ? __FILE__ -1) + 1)
#endif

#ifdef _MSC_VER
#  define __file__ __FILE__
#else // _MSC_VER
#  if !defined(__file__) && defined(__FILE__)
#     include <string.h>
// Le corta el path al nombre del archivo ya que molesta en el LOG
#     define __file__ ( (strrchr(__FILE__, '/') ? : __FILE__ - 1) + 1)
#  else // __file__
#     error "No se pudo definir __file__"
#  endif // __file__
#endif // _MSC_VER

// Macros para mostrar el contenido de un define
// http://stackoverflow.com/questions/1562074/how-do-i-show-the-value-of-a-define-at-compile-time
#define VALUE_TO_STRING(StringifyValue) #StringifyValue
#define VALUE(Value) VALUE_TO_STRING(Value)
#define VAR_NAME_VALUE(Value) #Value "="  VALUE(Value)
// Usar #pragma message(VAR_NAME_VALUE(MACRO_NAME)) para imprimir durante la
// compilacion el valor de una variable.

#ifndef __WXWIDGETS__
#  include <cstdio>
#  include <iostream>
#  include <fstream>
#  define DUMMY_SIZE 2048
#  define DUMMY(Var) char Var[DUMMY_SIZE]
#  define DUMMYSTR(Var) Var
#  define PRINTF(Dest, Message, ...) { snprintf(Dest, DUMMY_SIZE, Message, ## __VA_ARGS__); }
#  define FLUSH() std::cout.flush(); std::cerr.flush()
// ------------------------------------------------------------------------------
#  ifdef __DEBUG__
// ------------------------------------------------------------------------------
#     define REPORT_DEBUG(Message, ...) do {DUMMY(dummy); DUMMY(temp);          \
                                            PRINTF(temp, "[Debug]: %s()@%s:%d : %s", __func__, \
                                                   __file__, __LINE__,   \
                                                   Message); \
                                            PRINTF(dummy, temp, ## __VA_ARGS__);                                    \
                                            std::cerr<<dummy<<std::endl; } while ( 0 )
// ------------------------------------------------------------------------------
#     define REPORT_VERBOSE(Message, ...) do {DUMMY(dummy); DUMMY(temp);        \
                                              PRINTF(temp, "[Debug]: %s()@%s:%d : %s", __func__, \
                                                     __file__, __LINE__,   \
                                                     Message); \
                                              PRINTF(dummy, temp, ## __VA_ARGS__);                                    \
                                              std::cerr<<dummy<<std::endl; } while ( 0 )
#  else  // __DEBUG__
#     define REPORT_DEBUG(Message, ...) do {} while ( 0 )
#     define REPORT_VERBOSE(Message, ...) do {} while ( 0 )
#  endif // __DEBUG__
// ------------------------------------------------------------------------------
#  define SHOW_WARNING(Message, ...) do {DUMMY(dummy);                         \
                                         PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                         std::cout<<"[Advertencia]: "<<dummy<<std::endl; \
} while ( 0 )
// ------------------------------------------------------------------------------
#  define SHOW_STATUS(Message, ...) do {DUMMY(dummy);                          \
                                        PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                        std::cout<<"[Estado]: "<<dummy<<std::endl; } while ( 0 )
// ------------------------------------------------------------------------------
#  define SHOW_ERROR(Message, ...) do {DUMMY(dummy);                           \
                                       PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                       std::cerr<<"[Error]: "<<dummy<<std::endl; } while ( 0 )
// ------------------------------------------------------------------------------
#  define SHOW_INFO(Message, ...) do {DUMMY(dummy);                            \
                                      PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                      std::cerr<<"[Info]: "<<dummy<<std::endl; } while ( 0 )
// ------------------------------------------------------------------------------
#else // __WXWIDGETS__
#  include "wx/log.h"
#  include "wx/intl.h"
#  define DUMMY(Var) wxString Var
#  define DUMMYSTR(Var) Var.c_str()
#  define PRINTF(Dest, Message, ...) do { DUMMY(translated) = _(Message);       \
                                          translated.Replace("%n", "%%n", true);                                 \
                                          Dest.Printf(DUMMYSTR(translated), ## __VA_ARGS__, 0); \
} while ( 0 )
#  define FLUSH() wxLog::FlushActive()
// ------------------------------------------------------------------------------
#  ifdef __DEBUG__
// ------------------------------------------------------------------------------
#     define REPORT_DEBUG(Message, ...) do { DUMMY(dummy); DUMMY(temp);        \
                                             PRINTF(temp, "[Debug]: %s()@%s:%d : %s", __func__, \
                                                    __file__, __LINE__,   \
                                                    _( \
                                                       Message) ); \
                                             PRINTF(dummy, temp, ## __VA_ARGS__);                                    \
                                             wxLogDebug(dummy, NULL); } while ( 0 )
// ------------------------------------------------------------------------------
#     define REPORT_VERBOSE(Message, ...) do { DUMMY(dummy); DUMMY(temp);      \
                                               PRINTF(temp, "[Debug]: %s()@%s:%d : %s", __func__, \
                                                      __file__, __LINE__,   \
                                                      _( \
                                                         Message) ); \
                                               PRINTF(dummy, temp, ## __VA_ARGS__);                                    \
                                               wxLogVerbose(dummy, NULL); } while ( 0 )
// ------------------------------------------------------------------------------
#     define REPORT_WARNING(...) SHOW_WARNING(__VA_ARGS__)
// ------------------------------------------------------------------------------
#     define REPORT_STATUS(...) SHOW_STATUS(__VA_ARGS__)
// ------------------------------------------------------------------------------
#     define REPORT_ERROR(...) SHOW_ERROR(__VA_ARGS__)
// ------------------------------------------------------------------------------
#  define format_WINDOW_SIZE_EXPLORING_STRING_sspsdd "D:DebugSize:%s,%s,%p,%s,%d,%d"
#  define DEBUG_SIZE(Title, Window, Size) REPORT_DEBUG(                         \
      format_WINDOW_SIZE_EXPLORING_STRING_sspsdd, Title, # Window, Window,  \
      # Size, (Size).GetWidth(), (Size).GetHeight() )
// ------------------------------------------------------------------------------
#     define REPORT_PROGRESS(...) SHOW_PROGRESS(__VA_ARGS__)
#  else  // __DEBUG__ no definida
#     define REPORT_DEBUG(Message, ...) do {} while ( 0 )
#     define DEBUG_SIZE(...) do {} while ( 0 )
#     define REPORT_VERBOSE(Message, ...) do {} while ( 0 )
#     define REPORT_WARNING(...) do {} while ( 0 )
#     define REPORT_STATUS(...) do {} while ( 0 )
#     define REPORT_ERROR(...) do {} while ( 0 )
#     define REPORT_PROGRESS(...) do {} while ( 0 )
#  endif // __DEBUG__
// ------------------------------------------------------------------------------
#  define SHOW_WARNING(Message, ...) do { DUMMY(dummy);                        \
                                          PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                          wxLogWarning(dummy, NULL); } while ( 0 )
// ------------------------------------------------------------------------------
#  define SHOW_STATUS(Message, ...) do { DUMMY(dummy);                         \
                                         PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                         wxLogStatus(dummy, NULL); } while ( 0 )
// ------------------------------------------------------------------------------
#  define SHOW_ERROR(Message, ...) do { DUMMY(dummy);                          \
                                        PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                        wxLogError(dummy, NULL); } while ( 0 )
// ------------------------------------------------------------------------------
# define SHOW_INFO(Message, ...) do { DUMMY(dummy);                            \
                                      PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                      wxLogMessage(dummy, NULL); } while ( 0 )
// ------------------------------------------------------------------------------
#  define SHOW_PROGRESS(Message, ...) do { DUMMY(dummy);                       \
                                           PRINTF(dummy, Message, ## __VA_ARGS__);                                 \
                                           wxLogMessage(dummy, NULL); } while ( 0 )
#endif   // __WXWIDGETS__
// ------------------------------------------------------------------------------
// --EXCEPTION MACROS------------------------------------------------------------

#ifdef __CAN_THROW_EXCEPTIONS__
#  include <stdexcept>
// ------------------------------------------------------------------------------
#  define TRY try
// ------------------------------------------------------------------------------
#  define THROW(What, ...) do { DUMMY(dummy);                                  \
                                PRINTF(dummy, What, ## __VA_ARGS__);                                       \
                                throw std::exception(DUMMYSTR(dummy) ); } while ( 0 )
// ------------------------------------------------------------------------------
#  ifdef __DEBUG__
// ------------------------------------------------------------------------------
#     define CATCH TRY { catch ( std::exception &e ) {                            \
                            REPORT_DEBUG("D:Excepcion atrapada : Tipo(%s) Desc(%s)",          \
                                         typeid(e).name().c_str(), e.what().c_str() );  \
                            SHOW_ERROR(message_EXCEPTION_s, e.what().c_str() ); throw e; } }     \
   catch ( std::exception &e )
#  else
#     define CATCH catch ( std::exception &e )
#  endif // __DEBUG__
// ------------------------------------------------------------------------------
#else
extern "C" bool fakeExceptionThrown;
#  define TRY fakeExceptionThrown = false;
#  define THROW(What, ...) do { fakeExceptionThrown = true; DUMMY(throwdummy); \
                                PRINTF(throwdummy, message_EXCEPTION_TRY_s, What);                        \
                                REPORT_DEBUG(DUMMYSTR(throwdummy), ## __VA_ARGS__); } while ( 0 )
#  define CATCH if ( fakeExceptionThrown )
#endif   // __CAN_THROW_EXCEPTIONS__
// ------------------------------------------------------------------------------
// --REPORT & RETURN MACROS-----------------------------------------------------

/** Reporta por debug y retorna sin valor */
#define REPORT_AND_RETURN(Message, ...) do {                                   \
      REPORT_DEBUG(Message, ## __VA_ARGS__);                                       \
      return; } while ( 0 )
// ------------------------------------------------------------------------------
/** Reporta por debug y falla (por excepcion o retorno) */
#define REPORT_AND_FAIL(Message, ...) do {                                     \
      THROW(Message, ## __VA_ARGS__);                                              \
      return; } while ( 0 )
// ------------------------------------------------------------------------------
/** Reporta por debug y falla (ya sea por excepcion o valor de retorno) */
#define REPORT_AND_FAIL_VALUE(Message, ReturnValue, ...) do {                   \
      THROW(Message, ## __VA_ARGS__);                                              \
      return ReturnValue; } while ( 0 )
// ------------------------------------------------------------------------------
/** Reporta por debug y retorna error (similar a REPORT_AND_FAIL, pero no fatal) */
#define REPORT_AND_RETURN_VALUE(Message, ReturnValue, ...) do {                 \
      REPORT_DEBUG(Message, ## __VA_ARGS__);                                       \
      return ReturnValue; } while ( 0 )

// EXECUTION TIME MEASSURE MACROS-----------------------------------------------

#ifdef __DEBUG__
#  if (defined(_MSC_VER) || defined(__MINGW32__) )
#     include <time.h>
#  else
#     include <sys/time.h>
#     include <sys/resource.h>
#  endif
// ------------------------------------------------------------------------------
#  if !(defined(_MSC_VER) || defined(__MINGW32__) )
#     define IUSAGE  rusage iusage; getrusage(RUSAGE_SELF, &iusage)
#     define FUSAGE rusage fusage; getrusage(RUSAGE_SELF, &fusage);            \
   double runtime = (double)( (fusage.ru_utime.tv_usec -              \
                               iusage.ru_utime.tv_usec) +             \
                              (fusage.ru_stime.tv_usec -             \
                               iusage.ru_stime.tv_usec) )/1000000.0
#  else  /** (defined(_MSC_VER) || defined(__MINGW32__)) */
#     define IUSAGE
#     define FUSAGE double runtime; runtime = 0;
#  endif /** (defined(_MSC_VER) || defined(__MINGW32__)) */
// ------------------------------------------------------------------------------
#  define PROFILE_CALL(Execution) do {                                        \
      IUSAGE;                                                                 \
      REPORT_DEBUG("D:Iniciando %s", # Execution);                              \
      Execution; FUSAGE;                                                      \
      REPORT_STATUS("D: %.2fsecs ejecucion de %s", runtime, # Execution); } while ( 0 )
#else    // __DEBUG__
#  define PROFILE_CALL(Execution) Execution
#endif   // __DEUBG__

#endif   // LOGMACROS_H_
