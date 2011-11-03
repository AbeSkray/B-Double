/*

    Copyright (c) 2001, 2010 by Keith M. Knowles.
    Confidential, unpublished property of Keith M. Knowles.
    All rights reserved.


    Program:		Configuration.
    Module:		base.h -- common configuration definitions.
    Version:		1
    Author:		Keith M. Knowles.
    Date:		01/05/17


    Changes:

    10/09/07 kmk	Use O_BINARY; not O_binary.
    03/04/20 kmk	Define Cfg_file_mode_binary.
    03/04/01 kmk	Constants must be defined with #define, not enum.
    01/05/17 kmk	Initial version.


    Notes:


    Description:

    Configuration parameters are controlled here.

*/

/*! \file base.h
    \brief Common configuration definitions.
    All configuration symbols are always defined, even if only as  true  (1)
    or false (0).
*/

#ifndef	_Cfg_base_h
#define	_Cfg_base_h

/*! A note about byte-order.  Canonical order, or network  order,  is  high-
    byte order.  But most machines (numerically) are low-byte  order,  being
    Intel.  So we think our standard  should  be  low-byte  order  for  file
    formats we design.  We should have the capability to  process  high-byte
    order.  And most preferably, the format should include the byte-order in
    the header.  This allows for natural, no-overhead processing among homo-
    genous machines.
*/
#define Cfg_low_byte_order	0	///< low-byte stored first
#define Cfg_high_byte_order	1	///< high-byte stored first
#ifdef O_BINARY
#define Cfg_file_mode_binary	O_BINARY
#else
#define Cfg_file_mode_binary	0
#endif

#define	Cfg_trace_calls

#endif
