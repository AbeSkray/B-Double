/*

    Copyright (c) 2003 by Keith M. Knowles.
    Confidential, unpublished property of Keith M. Knowles.
    All rights reserved.


    Program:		config -- build configuration.
    Module:		cygwin.h -- define Cygwin configuration parameters.
    Version:		1
    Author:		Keith M. Knowles.
    Date:		04/04/01


    Changes:

    03/04/01 kmk	Initial version.


    Notes:



    Description:

    Cfg_debug is preferred.  Cfg_release is deprecated.

*/

#ifndef	_Config_cygwin_h
#define	_Config_cygwin_h

#include "cfg/base.h"			// base definitions

#if	defined(CYGWIN)			// safety check

#define Cfg_debug
#define	Cfg_byte_order		Cfg_low_byte_order
#define	Cfg_DOS_drive
#define	Cfg_mkdir_perm_arg
#define	Cfg_path_max		PATH_MAX	// [kmk]
//#define Cfg_printv			// per module: not here
//#define Cfg_release
#define	Cfg_signals
#define	Cfg_stat_h_S_IS_macros
//#define Cfg_stats
//#define Cfg_type_checking		// OO-style checking
#define	Cfg_uint_defined

#else

/*  The build defines one symbol (for the  compilation  command  line)  that
    indicates the particular configuration being built.  That symbol selects
    certain configuration header files that define all  the  pertinent  Cfg_
    configuration symbols.

    This is the Cygwin configuration file and it has been included erroneous-
    ly since the symbol, CYGWIN, is apparently not defined.
*/
build configuration error;

#endif
#endif
