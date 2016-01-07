#pragma once

#include "int_types.h"

#define UID_LOW32
//#define USE_SIGNAL_UID

namespace protocol
{
namespace signal
{
//===========================================================================// 

#ifdef UID_LOW32
  // format:
  // | 8bit(reserve) | 16bit(type) | 8bit(inst) | 32bit(uid) |
  // high                                                  low
  inline uint32_t getUidFrom64(uint64_t uid){return (uint32_t)uid;}
  inline uint16_t getTypeFrom64(uint64_t uid){return (uint16_t)(uid>>40);}
  inline uint8_t getInstFrom64(uint64_t uid){return (uint8_t)(uid>>32);}

  inline uint64_t gen64ByUidTypeInst(uint32_t uid, uint16_t type, uint8_t inst)
  {return ((uint64_t)type<<40) | ((uint64_t)inst<<32) | ((uint64_t)uid);}

  inline bool compare64ByUid(uint64_t a, uint64_t b)
  {
    if((uint32_t)a == (uint32_t)b)
      return (uint32_t)(a>>32) < (uint32_t)(b>>32);
    else
      return (uint32_t)a < (uint32_t)b;
  }
#else
  // format:
  // | 32bit(uid) | 8bit(reserve) | 16bit(type) | 8bit(inst) |
  // high                                                  low
  inline uint32_t getUidFrom64(uint64_t uid){return (uint32_t)(uid>>32);}
  inline uint16_t getTypeFrom64(uint64_t uid){return (uint16_t)(uid>>8);}
  inline uint8_t getInstFrom64(uint64_t uid){return (uint8_t)(uid);}

  inline uint64_t gen64ByUidTypeInst(uint32_t uid, uint16_t type, uint8_t inst)
  {return ((uint64_t)type<<8) | ((uint64_t)inst) | ((uint64_t)uid<<32);}

  inline bool compare64ByUid(uint64_t a, uint64_t b){return a < b;}  
#endif

//===========================================================================// 
#ifdef USE_SIGNAL_UID

  class SignalUid
  {
  public:
    SignalUid() : m_uid(0), m_type(0), m_inst(0) {}
    /*no explicit?!*/ SignalUid(uint32_t uid) : m_uid(uid), m_type(0), m_inst(0){}
    /*no explicit?!*/ SignalUid(uint64_t uid) : m_uid(getUidFrom64(uid)), m_type(getTypeFrom64(uid)), m_inst(getInstFrom64(uid)){}

  public:
    // !!
    operator uint32_t() const
    {
      return m_uid;
    }
    operator uint64_t() const
    {
      return gen64ByUidTypeInst(m_uid, m_type, m_inst);
    }

  public:

    friend struct OrderByUid;

  private:

    //bool operator < (const SignalUid & );
    
  private:

    uint32_t m_uid;
    uint16_t m_type;
    uint8_t m_inst;
  };
  
  struct OrderByUid
  {
    bool operator() (const SignalUid & a, const SignalUid & b) const
    {
      if(a.m_uid == b.m_uid)
        if(a.m_type == b.m_type)
          return a.m_inst < b.m_inst;
        else
          return a.m_type < b.m_type;
      else
        return a.m_uid < b.m_uid;
    }
  };

  typedef SignalUid yyuid_t;
  
#else
  
  struct OrderByUid
  {
    bool operator() (uint64_t a, uint64_t b) const
    {
      return compare64ByUid(a, b);
    }
  };

  typedef uint64_t yyuid_t;
#endif

typedef uint64_t serverid_t;

//===========================================================================//
//

//===========================================================================// 
}
}

