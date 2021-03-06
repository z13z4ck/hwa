
/* This file is part of the HWA project.
 * Copyright (c) 2012,2015 Christophe Duparquet.
 * All rights reserved. Read LICENSE.TXT for details.
 */

/**
 * @file
 * @brief HWA definitions that produce C code
 */


/**
 * @ingroup public_ins
 * @brief Trigger an error after code generation.
 * @hideinitializer
 */
#define HWA_ERR(msg)		_HWA_ERR_2(msg, __COUNTER__)
#define _HWA_ERR_2(...)		_HWA_ERR_3(__VA_ARGS__)
#define _HWA_ERR_3(msg, num)						\
  do {									\
    extern void __attribute__((error(msg))) hwa_error_##num(void); \
      hwa_error_##num();						\
  } while(0)


#define HWA_E(s)		_HWA_ERR_2(HW_QUOTE(HWA: s.), __COUNTER__)
#define HWA_E_VL(v,l)		HWA_E(`v` is not `l`)
#define HWA_E_NIL(v,l)		HWA_E(`v` is not in `l`)


/**
 * @ingroup public_ins
 * @brief Insert inline assembler code
 * @hideinitializer
 *
 * @code
 * hw_asm("reti");  // Insert the RETI instruction
 * @endcode
 */
#define hw_asm(...)			__asm__ __volatile__(__VA_ARGS__)


#if !defined hw_waste_cycles
/**
 * @ingroup public_ins
 * @brief Insert a software loop for `n` system clock cycles.
 * @hideinitializer
 *
 * Only works with compile-time constants.
 */
#  define hw_waste_cycles(n)		HW_E_IMP(hw_waste_cycles)
#endif


/**
 * @ingroup public_ins
 * @brief Create a context to memorize what the `hwa(...)` instructions do.
 * @hideinitializer
 *
 * Nothing is written into the hardware until `hwa_commit()` is called.
 */
/*
 * Expands _hwa_begin_all() that must be defined in hwa_<device>_2.h.
 */
#define hwa_begin()							\
  _hwa_check_optimizations(0);						\
  hwa_t hwa_st ; hwa_t *hwa = &hwa_st ;					\
  _hwa_setup_context(hwa) ;						\
  uint8_t hwa_xcommit = 0 /* Will warn if hwa_commit() is not called */


/**
 * @ingroup public_ins
 * @brief Create a context to memorize what the `hwa(...)` instructions do.
 * @hideinitializer
 *
 * The context is initialized with the values the registers have after a
 * system reset.
 *
 * Nothing is written into the hardware until `hwa_commit()` is called.
 */
#define hwa_begin_from_reset()			\
  _hwa_check_optimizations(0);			\
  hwa_t hwa_st ; hwa_t *hwa = &hwa_st ;		\
  _hwa_setup_context(hwa) ;			\
  _hwa_init_context(hwa) ;			\
  hwa_nocommit() ;				\
  uint8_t hwa_xcommit = 0


/**
 * @ingroup public_ins
 * @brief Generate machine code for the configuration stored in the context.
 *
 * Solve the configuration stored into the HWA context, then do the required
 * hardware register writes.
 *
 * @hideinitializer
 */
/* _hwa_commit_all() must be defined somewhere in the device-specific files.
 */
#define hwa_commit()					\
  do {							\
    uint8_t foo __attribute__((unused)) = hwa_xcommit ;	\
    hwa->commit = 1 ;					\
    _hwa_commit_context(hwa);				\
  } while(0)


/**
 * @ingroup public_ins
 * @brief  Same as hwa_commit() but do not write into hardware.
 *
 * This is used to put the HWA context in a known state before modifying it.
 *
 * @hideinitializer
 */
#define hwa_nocommit()				\
  do {						\
    hwa->commit = 0 ;				\
    _hwa_commit_context(hwa);			\
  } while(0)


/**
 * @ingroup private_ins
 * @brief Read one register of an object
 *
 * Syntax: `_hw( read, object, register );`
 * @hideinitializer
 */
#define _hw_read_reg(o,r)		_HW_SPEC(_hw_read, _HW_M(o,r))


/**
 * @ingroup private_ins
 * @brief Read one register of an object with interrupts disabled
 *
 * Syntax: `_hw_atomic_read( object, register );`
 * @hideinitializer
 */
#define _hw_atomic_read_reg(o,r)	_HW_SPEC(_hw_atomic_read, _HW_M(o,r))


/**
 * @ingroup private_ins
 * @brief Write one register of an object
 *
 * Syntax: `_hw_write_reg( object, register, value );`
 * @hideinitializer
 */
#define _hw_write_reg(o,r,v)		_HW_SPEC(_hw_write, _HW_M(o,r), v,)


/**
 * @ingroup private_ins
 * @brief Write one register of an object
 *
 * Syntax: `_hwa_write_reg( object, register, value );`
 * @hideinitializer
 */
#define _hwa_write_reg(o,r,v)		_HW_SPEC(_hwa_write, _HW_M(o,r), v)


#define _hw_mtd_hw_read__m1		, _hw_read__m1
#define _hw_mtd_hw_read__m2		, _hw_read__m2
#define _hw_mtd_hw_write__m1		, _hw_write__m1
#define _hw_mtd_hw_write__m2		, _hw_write__m2
#define _hw_mtd_hwa_write__m1		, _hwa_write__m1
#define _hw_mtd_hwa_write__m2		, _hwa_write__m2
#define _hw_mtd_hwa_write__r8		, _hwa_write__r8 /* FIXME */
#define _hw_mtd_hwa_write__oreg		, _hwa_write_oreg
#define _hw_mtd_hwa_write__xob1		, _hwa__write__xob1


/**
 * @ingroup private_ins
 * @brief Read one group of consecutive bits from one hardware register
 * @hideinitializer
 */
/* #define _hw_read__m1(o,a, r,rc,ra,rwm,rfm, rbn,rbp)	\ */
/*   _hw_read_##rc(a+ra,rbn,rbp) */

#define _hw_read__m1(...)		_hw_read__m1_x(__VA_ARGS__,)

#define _hw_read__m1_x(o,a, r,rc,ra,rwm,rfm, rbn,rbp, ...)	\
  _hw_read_##rc(a+ra,rbn,rbp)


/**
 * @ingroup private_ins
 * @brief Read one group of consecutive bits from one hardware register with interrupts disabled
 * @hideinitializer
 */
#define _hw_atomic_read__m1(o,a, r,rc,ra,rwm,rfm, rbn,rbp)	\
  _hw_atomic_read_##rc(a+ra,rbn,rbp)


/**
 * @ingroup private_ins
 * @brief Read two groups of consecutive bits from two hardware registers
 * @hideinitializer
 */
#define _hw_read__m2(o,a,					\
		     r1,rc1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,	\
		     r2,rc2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2)	\
  ((_hw_read_##rc1(a+ra1,rbn1,rbp1)<<vbp1) |			\
   (_hw_read_##rc2(a+ra2,rbn2,rbp2)<<vbp2))


/**
 * @ingroup private_ins
 * @brief Write something
 *
 * Syntax: `_hw_write( ..., value );`
 * @hideinitializer
 */
#define _hw_write(...)			_hw_write_2(__VA_ARGS__) /* Internal use */
#define _hw_write_2(x,...)		_hw_write_##x(__VA_ARGS__)

/**
 * @ingroup private_ins
 * @brief Write one group of consecutive bits into one hardware register
 * @hideinitializer
 */
#define _hw_write__m1(o,a, r,rc,ra,rwm,rfm, rbn,rbp, v,...)	\
  _hw_write_##rc(a+ra,rwm,rfm,rbn,rbp,v)


/**
 * @ingroup private_ins
 * @brief Write one group of consecutive bits into one hardware register with interrupts disabled
 * @hideinitializer
 */
#define _hw_atomic_write__m1(o,a, r,rc,ra,rwm,rfm, rbn,rbp, v)	\
  _hw_atomic_write_##rc(a+ra,rwm,rfm,rbn,rbp,v)

/**
 * @ingroup private_ins
 * @brief Write two groups of consecutive bits into two hardware registers
 * @hideinitializer
 */
#define _hw_write__m2(o,a,						\
		      rn1,rc1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,		\
		      rn2,rc2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2, v, ...)	\
  do {									\
    if ( ra1==ra2 ) {							\
      /* HWA_ERR("That should not happen"); */				\
      _hw_write##rc1( a+ra1, rwm1, rfm1,				\
		      (((1U<<rbn1)-1)<<rbp1) | (((1U<<rbn2)-1)<<rbp2),	\
		      (((v>>vbp1)&((1<<rbn1)-1))<<rbp1) | (((v>>vbp2)&((1<<rbn2)-1))<<rbp2)); \
    }									\
    else {								\
      _hw_write_##rc1(a+ra1,rwm1,rfm1,rbn1,rbp1, (v>>vbp1)&((1<<rbn1)-1)); \
	_hw_write_##rc2(a+ra2,rwm2,rfm2,rbn2,rbp2, (v>>vbp2)&((1<<rbn2)-1)); \
    }									\
  } while(0)


#define _hw_write__xob1(o,r,bn,bp,v)	_hw_write__xob1_2(_hw_reg_##o##_##r,o,r,bn,bp,v)
#define _hw_write__xob1_2(...)		_hw_write__xob1_3(__VA_ARGS__)
#define _hw_write__xob1_3(rc,ra,rwm,rfm,o,r,bn,bp,v)	\
  _hw_write_##rc( ra, rwm,rfm, bn,bp, v )


#define _hwa_write__m1(...)	_hwa_write__m1_x(__VA_ARGS__,)

#define _hwa_write__m1_x(o,a, r,rc,ra,rwm,rfm, bn,bp, v, ...)	\
  _hwa_write_##rc( &hwa->o.r, rwm,rfm, bn,bp, v )

#define _hwa_write__m2(o,a,						\
		       r1,rc1,ra1,rwm1,rfm1,rbn1,rbp1,vbp1,		\
		       r2,rc2,ra2,rwm2,rfm2,rbn2,rbp2,vbp2, v)		\
  do {									\
    _hwa_write_##rc1(&hwa->o.r1, rwm1,rfm1, rbn1,rbp1, ((v)>>(vbp1))&((1U<<rbn1)-1)); \
      _hwa_write_##rc2(&hwa->o.r2, rwm2,rfm2, rbn2,rbp2, ((v)>>(vbp2))&((1U<<rbn2)-1)); \
  } while(0)


/**
 * @brief  Write the register of an object
 */
#define _hwa_write_oreg(c,i,o,r,v,...)	_hwa_write_reg(o,r,v) HW_EOL(__VA_ARGS__)


#define _hwa_write__xob1(t,o,r,bn,bp,v)	_hwa_write__xob1_2(_hw_reg_##o##_##r,o,r,bn,bp,v)
#define _hwa_write__xob1_2(...)		_hwa_write__xob1_3(__VA_ARGS__)
#define _hwa_write__xob1_3(rc,ra,rwm,rfm,o,r,bn,bp,v)	\
  _hwa_write_##rc( &hwa->o.r, rwm,rfm, bn,bp, v )

/*  Added for io1a_2.h:
 *    _hwa_write(_##o##_##did, 1);
 *	_hwa__write__xob1(hw_shared, did, 1, 0, 1,);
 */
#define _hwa__write__xob1(o,r,bn,bp,v,...)	_hwa_write__xob1(,o,r,bn,bp,v)


/**
 * @ingroup private_ins
 * @brief Write some bits of a hardware register
 * @hideinitializer
 *
 *	_hw_write_reg_m( o, r, m, v );
 */
#define _hw_write_reg_m(o,r,m,v)			_HW_SPEC(_hw_wrrm,_HW_M(o,r),m,v)
#define _hw_wrrm__m1(o,a,r,rc,ra,rwm,rfm,rbn,rbp,m,v)	_hw_write##rc(a+ra,rwm,rfm,m,v)

/**
 * @ingroup private_ins
 * @brief Write some bits of a hardware register
 * @hideinitializer
 *
 *	_hwa_write_reg_m( o, r, m, v );
 */
#define _hwa_write_reg_m(o,r,m,v)			_HW_SPEC(_hwa_wrrm,_HW_M(o,r),m,v)
#define _hwa_wrrm__m1(o,a,r,rc,ra,rwm,rfm,rbn,rbp,m,v)	_hwa_write##rc(&hwa->o.r,rwm,rfm,m,v)


/**
 * @ingroup private_mac
 * @brief Specialize instruction `f` for class `c`: _HW_SP(f,c,...) -> f_c(...)
 * @hideinitializer
 *
 *  This is an alternative to _HW_SPEC() that is useful to allow _HW_SPEC() expansion.
 */
#define _HW_SP(...)			__HW_SP_2(__VA_ARGS__)
#define __HW_SP_2(f,c,...)		f##_##c(__VA_ARGS__)


/**
 * @brief Initialize the HWA context registers addresses of an object
 */
#define _hwa_setup(o)			_HW_SP(_hwa_setup,_HW_O(o))


/**
 * @brief Initialize the HWA context registers of an object with their reset value
 */
#define _hwa_init(o)			_HW_SP(_hwa_init,_HW_O(o))


/**
 * @brief Solve the configuration of an object
 */
#define _hwa_solve(o)			_HW_SP(_hwa_solve,_HW_O(o))


/**
 * @brief Commit the registers of an object
 */
#define _hwa_commit(o)			_HW_SP(_hwa_commit,_HW_O(o))


/**
 * @ingroup private_ins
 * @brief Create a HWA register
 * @hideinitializer
 */
#define _hwa_setup_reg(o,r)			_hwa_setup_reg_2(_HW_R(o,r))
#define _hwa_setup_reg_2(...)			_hwa_setup_reg_3(__VA_ARGS__)
#define _hwa_setup_reg_3(rt,ra,rwm,rfm,o,c,a,r)	_hwa_setup_##rt(&hwa->o.r, a+ra)


/**
 * @ingroup private_ins
 * @brief Initialize an HWA register of an object with its reset value
 * @hideinitializer
 */
#define _hwa_init_reg(o,r,v)			_hwa_init_reg_2(_HW_R(o,r),v)
#define _hwa_init_reg_2(...)			_hwa_init_reg_3(__VA_ARGS__)
#define _hwa_init_reg_3(rt,ra,rwm,rfm,o,c,a,r,v)	_hwa_set_##rt( &hwa->o.r, v )


/**
 * @ingroup private_ins
 * @brief Commit one object hardware register
 * @hideinitializer
 */
#define _hwa_commit_reg(o,r)			_hwa_commit_reg_2(_HW_M(o,r))
#define _hwa_commit_reg_2(...)			_hwa_commit_reg_3(__VA_ARGS__)
#define _hwa_commit_reg_3(rt,...)		_hwa_commit_reg_##rt(__VA_ARGS__)

#define _hwa_commit_reg__m1(o,a,r,rt,ra,rwm,rfm,bn,bp)	\
  _hwa_commit_##rt(&hwa->o.r,rwm,rfm,hwa->commit)

#define _hwa_nocommit_reg(o,r)			_hwa_nocommit_reg_2(_HW_M(o,r))
#define _hwa_nocommit_reg_2(...)		_hwa_nocommit_reg_3(__VA_ARGS__)
#define _hwa_nocommit_reg_3(rt,...)		_hwa_nocommit_reg_##rt(__VA_ARGS__)
#define _hwa_nocommit_reg__m1(o,a,r,rt,ra,rwm,rfm,bn,bp)	\
  _hwa_commit_##rt(&hwa->o.r,rwm,rfm,0)


/**
 * @ingroup private_ins
 * @brief Get the mmask of the logical register `r` of object `o`.
 *
 * The mmask is set each time a hwa_write() is performed. It is reset after the
 * value has been committed.
 *
 * @hideinitializer
 */
#define _hwa_mmask(o,r)					_HW_SPEC(_hwa_mmask,_HW_M(o,r))
#define _hwa_mmask__m1(o,a,r,rc,ra,rwm,rfm,bn,bp)	(((hwa->o.r.mmask)>>bp)&((1U<<bn)-1))


/**
 * @ingroup private_ins
 * @brief Get the value to be committed for the logical register `r` of object `o`.
 * @hideinitializer
 */
#define _hwa_mvalue(o,r)				_HW_SPEC(_hwa_mvalue,_HW_M(o,r))
#define _hwa_mvalue__m1(o,a,r,rc,ra,rwm,rfm,bn,bp)	(((hwa->o.r.mvalue)>>bp)&((1U<<bn)-1))


/**
 * @ingroup private_ins
 * @brief Get the last committed value for the logical register `r` of object `o`.
 * @hideinitializer
 */
#define _hwa_ovalue(o,r)				_HW_SPEC(_hwa_ovalue,_HW_M(o,r))
#define _hwa_ovalue__m1(o,a,r,rc,ra,rwm,rfm,bn,bp)	(((hwa->o.r.ovalue)>>bp)&((1U<<bn)-1))


/**
 * @ingroup private_ins
 * @brief Set the value of the logical register `r` of the object `o` in the context to `v` without setting the modification mask (mmask).
 * @hideinitializer
 */
#define _hwa_set_reg(o,r,v)				_HW_SPEC(_hwa_set_reg,_HW_M(o,r),v)
#define _hwa_set_reg__m1(o,a,r,rc,ra,rwm,rfm,bn,bp,v)			\
    hwa->o.r.mvalue = (((hwa->o.r.mvalue)>>bp) & ~((1U<<bn)-1)) | (v<<bp)



HW_INLINE void _hwa_setup__r8 ( hwa_r8_t *r, intptr_t a )
{
  r->a		= a ;
  r->mmask	= 0 ;
  r->mvalue	= 0 ;
  r->omask	= 0 ;
  r->ovalue	= 0 ;
}

HW_INLINE void _hwa_setup__r16 ( hwa_r16_t *r, intptr_t a )
{
  r->a		= a ;
  r->mmask	= 0 ;
  r->mvalue	= 0 ;
  r->omask	= 0 ;
  r->ovalue	= 0 ;
}

HW_INLINE void _hwa_setup__r32 ( hwa_r32_t *r, intptr_t a )
{
  r->a		= a ;
  r->mmask	= 0 ;
  r->mvalue	= 0 ;
  r->omask	= 0 ;
  r->ovalue	= 0 ;
}


/** 
 * @ingroup private_ins
 * @brief  Initialize a HWA register to a specific value (usually the reset value).
 * @hideinitializer
 */
HW_INLINE void _hwa_set__r8 ( hwa_r8_t *r, uint8_t v )
{
  if ( r->mmask )
    HWA_ERR("commit required before resetting.");

  r->mmask = 0xFF ;
  r->mvalue = v ;
}

HW_INLINE void _hwa_set__r16 ( hwa_r16_t *r, uint16_t v )
{
  if ( r->mmask )
    HWA_ERR("commit required before resetting.");

  r->mmask = 0xFFFF ;
  r->mvalue = v ;
}

HW_INLINE void _hwa_set__r32 ( hwa_r32_t *r, uint32_t v )
{
  if ( r->mmask )
    HWA_ERR("commit required before resetting.");

  r->mmask = 0xFFFFFFFF ;
  r->mvalue = v ;
}


/**
 * @ingroup private_ins
 * @brief  Write into one 8-bit context register.
 *
 * Write value `v` into `msk` bits of the context register pointed by
 * `r`. Trying to write `1`s into non-writeable bits triggers an error.
 *
 * The mask of modified values `mmask` is set according to `msk` even if
 * the value is not modified. `_hwa_commit__r8()` will check if the register has
 * effectively been modified.
 *
 * @param r	register pointer.
 * @param rwm	writeable bits mask of the register.
 * @param rfm	flag bits mask of the register.
 * @param msk	mask of bits concerned.
 * @param v	value to write.
 */
HW_INLINE void _hwa_write_r8 ( hwa_r8_t *r, uint8_t rwm, uint8_t rfm, uint8_t msk, uint8_t v )
{
  if (v & ~msk)
    HWA_ERR("value overflows the mask.");

  if ((rwm & msk) != msk)
    HWA_ERR("trying to modify bits that are not writeable.");

  if ((r->mmask & msk) != 0 && (r->mvalue & msk) != v)
    HWA_ERR("committing is required before setting a new value.");

  if ( msk & rfm )
    if ( v == 0 )
      HWA_ERR("flag bit can only be cleared by writing 1 into it.");

  r->mmask |= msk ;
  r->mvalue = (r->mvalue & ~msk) | (msk & v) ;
}

HW_INLINE void _hwa_write_r16 ( hwa_r16_t *r, uint16_t rwm, uint16_t rfm, uint16_t msk, uint16_t v )
{
  if (v & ~msk)
    HWA_ERR("value overflows the mask.");

  if ((rwm & msk) != msk)
    HWA_ERR("trying to modify bits that are not writeable.");

  if ((r->mmask & msk) != 0 && (r->mvalue & msk) != v)
    HWA_ERR("committing is required before setting a new value.");

  if ( msk & rfm )
    if ( v == 0 )
      HWA_ERR("flag bit can only be cleared by writing 1 into it.");

  r->mmask |= msk ;
  r->mvalue = (r->mvalue & ~msk) | (msk & v) ;
}

HW_INLINE void _hwa_write_r32 ( hwa_r32_t *r, uint32_t rwm, uint32_t rfm, uint32_t msk, uint32_t v )
{
  if ( (v & msk) != v )
    HWA_ERR("value overflows the mask.");

  if ((rwm & msk) != msk)
    HWA_ERR("trying to modify bits that are not writeable.");

  if ((r->mmask & msk) != 0 && (r->mvalue & msk) != v)
    HWA_ERR("committing is required before setting a new value.");

  if ( msk & rfm )
    if ( v == 0 )
      HWA_ERR("flag bit can only be cleared by writing 1 into it.");

  r->mmask |= msk ;
  r->mvalue = (r->mvalue & ~msk) | (msk & v) ;
}


/**
 * @ingroup private_ins
 * @brief Trigger an error if optimizers failed to remove this code
 * @hideinitializer
 */
HW_INLINE void _hwa_check_optimizations ( uint8_t x )
{
  if (x) { HWA_ERR("you may have forgotten to turn optimizations on."); }
}


#define _hw_write__r8(ra,rwm,rfm,bn,bp,v)	_hw_write_r8(ra,rwm,rfm,((1UL<<bn)-1)<<bp,((uint8_t)(v))<<bp)
#define _hw_write__r16(ra,rwm,rfm,bn,bp,v)	_hw_write_r16(ra,rwm,rfm,((1UL<<bn)-1)<<bp,(v)<<bp)
#define _hw_write__r32(ra,rwm,rfm,bn,bp,v)	_hw_write_r32(ra,rwm,rfm,((1UL<<bn)-1)<<bp,(v)<<bp)

#define _hwa_write__r8(ra,rwm,rfm,bn,bp,v)	_hwa_write_r8(ra,rwm,rfm,((1UL<<bn)-1)<<bp,((uint8_t)(v))<<bp)
#define _hwa_write__r16(ra,rwm,rfm,bn,bp,v)	_hwa_write_r16(ra,rwm,rfm,((1UL<<bn)-1)<<bp,(v)<<bp)
#define _hwa_write__r32(ra,rwm,rfm,bn,bp,v)	_hwa_write_r32(ra,rwm,rfm,((1UL<<bn)-1)<<bp,(v)<<bp)
