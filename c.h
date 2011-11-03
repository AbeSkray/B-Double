/*

    Copyright (c) 1994-2008, 2010-2011 by Keith M. Knowles.
    Free software licensed under the terms of the
    GNU Lesser General Public Licence.


    Program:		Standard C definitions
    Module:		c.h
    Version:		1
    Author:		Keith M. Knowles.
    Date:		92/02/11


    Definitions to make C programming more palatable.  Our usual  convention
    is to capitalize the first letter of names that  are  not  functions  or
    variables.  In this case, we hold the following to be self-evident,  and
    object to even one letter of upper-case.  Take note...


    Notes:

    - Should __FUNC__ be debug-time only?
    - Modify the Call_ macros to use Std_func.
    - Tags_set is somewhat dangerous if the ampersand is  forgotten:  do  we
      need a binary op?
    - Call_args should support Std_func?
    - after IP Flex, I think I want Flag to work with "bit_xxx" names.  Some
      convention that distinguishes bit numbers from flags.
    - should Align be (pvt)?
    - IfTrace should test 1st arg, after declaring it extern  bool.   Allows
      management of the name space, instead of fixing bits.
    - perhaps New and Kill should be in memory/heap/heap.h since you  should
      include that as well anyway?


    Changes:

    11/08/04 kmk	Define int64 and uint64.
    10/10/08 kmk	Define wchar.
    06/08/05 kmk	Strip out the v1 Locus.
    06/07/22 kmk	Add Rep(eat) macros.
    06/07/22 kmk	Rename REMOVE as DEBRIS.
    03/10/10 kmk	Removed DEBUG.
    03/10/08 kmk	Implemented new Locus scheme that supports an entire
			call-frame trace.
    03/10/03 kmk	Call_here -> Call_locus.
    03/07/25 kmk	Define an  external  __FUNC__  in  case  it  is  not
			defined locally.
    03/06/19 kmk	Add Call_func.
    03/06/12 kmk	Eliminate O_BINARY.
    03/03/06 kmk	Add a standard prefix tag to printv.
    03/03/04 kmk	printv to fflush.
    03/01/24 kmk	Add Tri_unk for completeness.
    03/01/20 kmk	Retract 03/01/07 to be like Func.
    03/01/16 kmk	Add Tags_merge/next.
    03/01/15 kmk	A solution to the Assert debate: no assertions after
			release, BUT the side-effects,  if  any,  are  still
			executed.
    03/01/15 kmk	Add printv.
    03/01/08 kmk	Add Tri_yes/no.
    03/01/07 kmk	Remove # from Unimpl.  Use quotes.
    03/01/06 kmk	Add tri-state: Tri, No, Unknown, Yes.
    02/12/22 kmk	Extend Trip to call debug routine at limit.
    02/12/18 kmk	Add Trip to Func.
    02/12/01 kmk	Define Szint.
    02/11/23 kmk	Replace Func with Std_func.  (The macro name remains
			the same, though.)
    02/11/18 kmk	Add Func_2.
    02/11/01 kmk	Define schar.
    02/04/12 kmk	Change copyright name.
    01/10/08 kmk	Define Double.
    01/12/11 kmk	Use longer path to include config.h.
    01/04/20 kmk	Use Defined_uint.
    01/02/13 kmk	Assert must be unconditional.  (Later, we may  split
			the function.)
    01/02/09 kmk	Eliminate func.
    01/02/08 kmk	Add Bug & make Assert conditional upon DEBUG again.
    00/11/18 kmk	Move Stats macros here.
    00/11/16 kmk	Add Sext/Zext.  (Maybe they belong in bits.h.)
    00/07/27 kmk	Correct the value of Infinity(!).
    00/06/02 kmk	Add Func.
    00/05/12 kmk	Add Arg_range.
    00/04/28 kmk	Add C_decls/end_C_decls.
    00/04/27 kmk	Add REMOVE for flagging debris code  to  be  removed
			before production.
    00/03/31 kmk	Add Dtor.  (Perhaps it should be in heap/heap.h?)
    00/03/30 kmk	Add Infinity.
    00/03/20 kmk	If _DEBUG is defined, then define DEBUG.
    00/03/19 kmk	Fix Zero() by adding an &.
    00/03/18 kmk	Add Call_ tracking debug macros.
    00/03/18 kmk	Add Zeros.  (Zero is dangerous if used with a ptr.)
    00/03/16 kmk	Cast is once more  deprecated.   The  most  readable
			form is to define a cast operator for a type:
			  #define Type_(p) ((Type *) (p))
    00/03/15 kmk	Add Cast for readability.
    00/03/07 kmk	For now, define O_BINARY here for Linux.
    00/03/05 kmk	Changed Log_inf to Log_infinity.
    00/03/01 kmk	Make Assert not conditional upon DEBUG.  (It was too
			dangerous.)
    00/02/29 kmk	Port to DinOSore.
    00/02/12 kmk	Move New/Kill to memory/heap/heap.h.
    00/02/11 kmk	Add New0.
    00/02/11 kmk	Remove FP: found out how to use FP prototypes.
    00/02/09 kmk	Add Export.  Revert New/Kill for DinOSore.
    99/12/25 kmk	Restore Ptr/Tags.
    99/12/14 kmk	Add Flag.
    99/02/24 kmk	Add Reverse_links and Treverse_links.
    98/06/27 kmk	Changed IfDebug to If_debug.
    98/05/23 kmk	OK, finally.  Move the canonical  byte-order  macros
			to bits/bits.h.
    98/05/22 kmk	Alter IfTrace scheme to use isolated, external bools
			for trace detail.  Also, change  the  name  back  to
			IfDebug, since it and Trace are not always used  for
			tracing.
    98/05/17 kmk	Declare tracing without extern.
    97/11/11 kmk	Add arg to non-DEBUG version of IfTrace.
    97/11/01 kmk	Add Zero().
    97/10/20 kmk	Separated the DEBUG macros.
    97/10/16 kmk	Add the Mask macro.
    97/10/05 kmk	Change (If)Debug macros to (If)Trace and  add  first
			arg to select the tracing detail.
    97/10/04 kmk	Clean out deprecated material: bits
    97/10/01 kmk	Define IfDebug.
    97/09/30 kmk	Define Debug and debugging.
    97/08/27 kmk	Redefine Unimpl to raise an exception.
    97/08/25 kmk	Everybody uses New,  so  include  memory/heap/heap.h
			here.
    97/08/16 kmk	Move definition of Loginf here.
    97/08/12 kmk	Eliminate addr and nat.
    97/08/03 kmk	Make Assert usable in a comma expression.
    97/07/15 kmk	Define Advance for address arithmetic.
    97/07/10 kmk	Remove definition of string.
    97/03/30 kmk	Change Nelts to Nr.
    97/01/28 kmk	Define (new) Ptr and Tags.
    96/12/08 kmk	Make Assert conditional upon DEBUG.
    96/12/03 kmk	Upcase primitive macros and add New and Kill.
    96/11/20 kmk	Remove LHS cast from memnew.
    96/11/18 kmk	Define Unimpl.
    96/11/12 kmk	Define newmem macro.
    96/10/10 kmk	Add experimental Alias macro.
    96/10/03 kmk	Add experimental Ptr macro.
    96/09/21 kmk	OK, New() was deprecated, now eliminated.
    96/09/15 kmk	Add source file/line to support E_memory.
    96/09/15 kmk	pack_args() changed to args_pack().
    96/09/13 kmk	Added assert and a definition  of  new  that  raises
			E_memory.
    96/08/25 kmk	Defined string.  Unsure if I'll use it...
    96/07/12 kmk	Introduce addr as synonym for pvt.
    96/07/04 kmk	Unconditionally include <sys/types.h>.
    96/07/02 kmk	Used unsigned constant in Bit macro to avoid warning
			from some ANSI compilers.
    96/06/28 kmk	Define Nelts.
    95/12/31 kmk	Reinstate uchar.
    95/09/09 kmk	The func notation is being restored to its  original
			form because ANSI-dexterity no longer seems  necess-
			ary.  Most C compilers are now ANSI-compliant.
    95/08/26 kmk	Merge latest Glue changes: define ulong synonym, add
			endian macros, add the Bit  macro,  cast  result  of
			Offset to  allow  arithmetic,  delete  private,  add
			ulong, delete uchar, adjust Align to work  with  64-
			bits, delete proc.
    95/08/13 kmk	Add int type cast to Offset.
    95/03/12 kmk	Linux sometimes defines uint for you.  Therefore, we
 			really do have to define our own  notation.   Define
 			nat.
    95/02/22 kmk	Correct error in New.
    95/02/10 kmk	Defined ANSI here on Linux.
    95/02/09 kmk	Restored private to  non-deprecated  status.   Added
 			the New macro.  Added "pvt" synonym (for trial).
    94/07/14 kmk	Use word "deprecated".
    94/06/27 kmk	Add support  for  ANSI-independence.   You  can  use
 			"func" notation to use ANSI function prototypes, yet
 			still  compile  with   a   non-ANSI   compiler,   if
 			necessary.  Also, move "proc"  and  "private"  to  a
 			"deprecated features" section.
    94/06/26 kmk	Provide func notation for ANSI-independence.
 			Proc is deprecated.
    94/06/26 kmk	Add note that 'private' is deprecated.
    94/03/25 kmk	This version.
    92/02/11 kmk	Original version.


    \dir		std
    In c.h, these are details about the std/ directory and its contents.

    \page		std standard stuff
    This is the "implementation" view.


    Conventions:

    - for line comments, we use, (key standard comment convention):
      - //	standard comment
      - ///	indicates meta-code
      - ////	indicates an issue requiring attention

    [111013] are the last two cases undesirably back-to-front?  Probably not.
*/

#ifndef	_C_h
#define	_C_h

/*  This must be included first to configure what follows.
*/
#include "cfg/config.h"

/*  This is to define uint, ulong, and ushort.  [kmk] should move to ./cfg.
*/
//#ifdef	LINUX
#include <sys/types.h>
//#endif

/*  General constant definitions.
*/
#ifdef	__cplusplus
#define	nil		0
#else
enum { false = 0, true };
#define nil		((void *) 0)
#endif

/** Unknown is 0 for easy initialization.

    The Tri_yes/no macros may be more efficient than testing a Tri  directly
    against Yes or No.  Tri_unk exists only for completeness (and  the  name
    is not suitable yet).

    tri > Unknown => Yes
    tri < Unknown => No
*/
enum { No = -1, Unknown = 0, Yes = 1 };	// Tri-state, initially Unknown

#define	Infinity	0x7fffffff
#define	Log_infinity	64		// log2(infinity)
#define	Szint		sizeof(int)

/*  This is not necessary!
*/
#ifdef	OBSOLETE
#ifdef	LINUX
#define	O_BINARY	0		// stupid DinOSore file mode
#endif
#endif

/*  General type definitions.
*/
#ifndef	__cplusplus
typedef unsigned char	bool;
#endif
typedef void		Dtor(void *elt);
typedef void *		pvt;
typedef signed char	Tri;		// tri-state

/*  We expect the following to be defined by <sys/types.h>, included above.
*/
typedef signed char	schar;
typedef unsigned char	uchar;
typedef unsigned short	wchar;
typedef unsigned long long uint64;
typedef long long	int64;
#ifndef	Cfg_uint_defined
typedef unsigned int	uint;
typedef unsigned long	ulong;
typedef unsigned short	ushort;
#endif

/*  Basic useful operators.
*/
#define	Advance(ptr, inc)						\
			((void *) ((char *) (ptr) + (inc)))
#define Align(n, q)	(((long) (n) + (q) - 1) & ~((q) - 1))
#define	Arg_range(arg, min, max)					\
			Debug(						\
			  if ((arg) < (min) || (arg > max)) {		\
			    fail(					\
			      exception(				\
				&exc_arg_range,				\
				#arg,					\
				min,					\
				max));					\
			  })
#define	Bit(n)		(1u << (n))
#define	Bug(cause)	trap_fail(exception(&exc_bug, #cause),		\
			  __FILE__, __LINE__)
#define	Double(qty, inc)						\
			((qty) <= (inc) ? 2 * (qty) : (qty) + (inc))
#define	Export		__declspec(dllexport)
#define	Flag(f)		Bit(_##f)
#define	Mask(n)		(Bit(n) - 1)
#define	Nr(vec)		(sizeof(vec) / sizeof(vec[0]))
#define	Offset(type, field)						\
			((int) (&((type *) 0)->field))
#define	Ptr(ptr)	((pvt) ((int) (ptr) & ~3))
#define	Ptr_set(ptr, ptrval)						\
			(*(int *) (ptr) =				\
			  (int) *(ptr) & 3 | (int) (ptrval))
/** \brief repeat \a v 16 times.
*/
#define	Rep16(v)	v,v,v,v,v,v,v,v,v,v,v,v,v,v,v,v
#define	Rep256(v)	Rep16(v),Rep16(v),Rep16(v),Rep16(v),		\
			Rep16(v),Rep16(v),Rep16(v),Rep16(v),		\
			Rep16(v),Rep16(v),Rep16(v),Rep16(v),		\
			Rep16(v),Rep16(v),Rep16(v),Rep16(v)
#define	Rep1K(v)	Rep256(v),Rep256(v),Rep256(v),Rep256(v)
#define	Rep2K(v)	Rep256(v),Rep256(v),Rep256(v),Rep256(v),	\
			Rep256(v),Rep256(v),Rep256(v),Rep256(v)
#define	Rep4K(v)	Rep2K(v),Rep2K(v)
#define	Reverse_links(head, type, link)					\
			{						\
			  pvt	this, next, prev;			\
			  for (this = head, prev = nil;			\
			      this;					\
			      next = ((type) this)->link,		\
				((type) this)->link = prev,		\
				prev = this, this = next);		\
			  head = prev;					\
			}
#define	Sext(v,b,s)	((int) ((v) << (32 - (b) - (s))) >> (32 - (s)))
#define	Tags(ptr)	((int) (ptr) & 3)
#define Tags_merge(ptr, tags)						\
			((pvt) ((int) (ptr) & ~3 | (tags)))
#define Tags_next(ptr)	(Tags(ptr) ? nil : (ptr))
#define	Tags_set(ptr, tags)						\
			(*(int *) (ptr) = *(int *) (ptr) & ~3 | (tags))
#define	Treverse_links(head, type, ptr, link, action)			\
			{						\
			  type	ptr;					\
			  pvt	next, prev;				\
			  for (ptr = head, prev = nil;			\
			      ptr;					\
			      next = ptr->link,				\
				ptr->link = prev,			\
				prev = ptr, ptr = next);		\
			  for (ptr = prev, prev = nil;			\
			      ptr;					\
			      next = ptr->link,				\
				ptr->link = prev,			\
				prev = ptr, ptr = next) {		\
			    action;					\
			  }						\
			}
#define	Tri_no(tri)	((tri) < Unknown)
#define Tri_unk(tri)	((tri) == Unknown) // for completeness
#define	Tri_yes(tri)	((tri) > Unknown)
#define	Unimpl(what)	trap_fail(					\
			  exception(&exc_unimpl, #what),		\
			  __FILE__,					\
			  __LINE__)
#define	Zero(mem)	(memset((&mem), 0, sizeof(mem)))
#define	Zeros(mem,sz)	(memset((mem), 0, (sz)))
#define	Zext(v,b,s)	((v) >> (b) & Mask(s))

/*  Debugging operators:
*/
#ifndef Cfg_release
#define	Assert(exp)	((exp) ? 0 :					\
			  trap_fail(					\
			    exception(&exc_assert, #exp),		\
			    __FILE__,					\
			    __LINE__))
#define	Debug(action)	action
#define	If_debug(detail, action)					\
			{						\
			  extern bool detail;				\
			  if (detail) { action; }			\
			}
#define	DEBRIS		       // mark for code removal before release
#else
#define	Assert(exp)	(exp)	      // just the side-effects, please
#define	Debug(action)
#define	If_debug(detail, action)
#define	DEBRIS		please remove this code;
#endif

/*  Verbose printing for debugging.

    Note firstly that we  are  moving  toward  C++-style  printing  so  that
    objects can be streamed.  And secondly, toward using the Log facility in
    a streaming mode.

    But for now, we remain with the C RTL printf style.

    printvt is a verbose print that prefixes a standard tag  to  the  print.
    The Func macro must be used in the calling function.  Note that at least
    one argument must be supplied, even if it is a dummy "0".  The tag takes
    the form:

	[<thread-id>:<func-name>:<trip-count>]

    printv is a verbose print without the tag.
*/
#ifdef	CFG_Visual_Studio
#error why is this compiling???
#if Cfg_printv
#ifdef Cfg_threaded
#define Printv_pfx_fmt	"[%x:%s:%d] "
#define Printv_pfx_info	pthread_self(), __FUNC__.name, __FUNC__.trip.count++
#else
#define Printv_pfx_fmt	"[%s:%d] "
#define Printv_pfx_info	__FUNC__.name, __FUNC__.trip.count++
#endif
#define Obj_print(o)	obj_print(o)
#define printv(args...)	( printf(args), debug_fflush() )
#define printvt(fmt, args...)						\
			printv(Printv_pfx_fmt fmt, Printv_pfx_info, args)
#else
#define Obj_print(o)
#define printv(args...)			// suppress verbosity
#define printvt(fmt, args...)
#endif
#endif

/*  Statistics collection.
*/
#ifdef	Cfg_stats
#define	Stats_max(m,q)		{ if ((q) > (m)) (m) = (q); }
#define	Stats_tally(s,q)	((s) += (q))
#else
#define	Stats_max(m,q)
#define	Stats_tally(s,q)
#endif

/*  Multi-platform operators.
*/
#ifdef	DinOSore			// doesn't work on DinOSore [kmk]
#ifdef	__cplusplus
#define	C_decl		extern "C"
#define	C_decls		extern "C" {
#define	end_C_decls	};
#else
#define	C_decl
#define	C_decls
#define	end_C_decls
#endif
#endif

#endif
