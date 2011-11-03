/*

    Copyright (c) 2003 by Keith M. Knowles.
    Confidential, unpublished property of Keith M. Knowles.
    All rights reserved.


    Program:		config -- build configuration.
    Module:		linux.h -- define Linux configuration parameters.
    Version:		1
    Author:		Keith M. Knowles.
    Date:		04/04/01


    Changes:

    03/10/10 kmk	DEBUG -> Cfg_debug.
    03/04/01 kmk	Initial version.


    Notes:



    Description:


*/

#ifndef	_Config_linux_h
#define	_Config_linux_h

#include "cfg/base.h"			// base definitions

#if	defined(LINUX)			// safety check

#define	Cfg_debug
#define	Cfg_byte_order		Cfg_low_byte_order
#define	Cfg_mkdir_perm_arg
#define	Cfg_path_max		PATH_MAX
#define	Cfg_signals
#define	Cfg_stat_h_S_IS_macros
#define	Cfg_stats
#define	Cfg_trace_calls
#define	Cfg_type_checking
#define	Cfg_uint_defined

#else

/*  The build defines one symbol (for the  compilation  command  line)  that
    indicates the particular configuration being built.  That symbol selects
    certain configuration header files that define all  the  pertinent  Cfg_
    configuration symbols.

    This is the Linux configuration file and it has been included erroneous-
    ly since the symbol, LINUX, is apparently not defined.
*/
build configuration error;

#endif

#endif
