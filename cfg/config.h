/*

    Copyright (c) 2001-2006, 2010 by Keith M. Knowles.
    Confidential, unpublished property of Keith M. Knowles.
    All rights reserved.


    Program:		config -- build configuration.
    Module:		config.h -- define all configuration parameters.
    Version:		1
    Author:		Keith M. Knowles.
    Date:		01/04/21


    Changes:

    10/10/05 kmk	Add WIN32 configuration.
    06/08/05 kmk	Use #error.
    03/04/01 kmk	Merely include configuration leaves.
    03/01/15 kmk	Define Cfg_release/stats.
    02/11/15 kmk	Add Cfg_signals.
    02/11/08 kmk	Started making matrix definitions.
    02/11/07 kmk	Added Cygwin config.
    01/04/21 kmk	Initial version.


    Notes:

    . assert mode?  (I think there is no middle ground, though.)


    Description:

    All configuration parameters are defined here.

    We think the scheme should be that the configuration selects an  include
    file herein.  That include file is the leaf in a tree of  includes  that
    builds the configuration in  terms  of  defined  symbols.   Each  symbol
    controls one selection alone.  I.e. the  file-path  separator  character
    should be specified directly by a symbol that defines it, rather than by
    some indirect attribute such as which is the  operating  system  target.
    The result is a kind of matrix of definition propagation.

    Until we acquire further experience, we will make all the definitions in
    this file.

*/

#ifndef	_Config_h
#define	_Config_h

#include "cfg/base.h"			// base definitions

#if	defined(LINUX)
#include "cfg/linux.h"
#elif	defined(CYGWIN)
#include "cfg/cygwin.h"
#elif	defined(WIN32)
#include "cfg/win32.h"
#elif	defined(WINUX)
#error WINUX build configuration is undefined!
#else
#error no build configuration is defined!
#endif
#endif
