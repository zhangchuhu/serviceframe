
#pragma once

#include "packet.h"

namespace utility
{

inline bool StringToPacket(const char * data, uint32_t size, sox::Marshallable & msg)
{
  sox::Unpack up(data, size);
  try
  {
    msg.unmarshal(up);
  }
  catch(sox::UnpackError e)
  {
    return false;
  }

  return true;
}

// same as sox::StringToPacket
inline bool StringToPacket(const std::string & data, sox::Marshallable & msg)
{
  return StringToPacket(data.data(), data.size(), msg);
}

// todo template 
template <class MarshallableType>
inline MarshallableType StringToPacket(const char * data, uint32_t size)
{
  MarshallableType msg;
  sox::Unpack up(data, size);
  try
  {
    msg.unmarshal(up);
  }
  catch(sox::UnpackError e)
  {
    // todo
  }

  return msg;
}

template <class MarshallableType>
inline MarshallableType StringToPacket(const std::string & data)
{
  return StringToPacket<MarshallableType>(data.data(), data.size());
}


inline std::string PacketToString(const sox::Marshallable & msg)
{
  sox::PackBuffer pb;
  sox::Pack pk(pb);
  msg.marshal(pk);
  return std::string(pk.data(), pk.size());
}

// same as sox::PacketToString
inline void PacketToString(const sox::Marshallable & msg, std::string & data)
{
  sox::PackBuffer pb;
  sox::Pack pk(pb);
  msg.marshal(pk);
  data.assign(pk.data(), pk.size());
}

}

//======================================================================================
// helper packets
template<typename Type1>
struct PContextField1: public sox::Marshallable
{
  PContextField1(){}
  PContextField1(Type1 t1):f1(t1){}
  Type1 f1;
  virtual void marshal(sox::Pack &p) const 
  {
    p << f1;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> f1;
  }
};

template<typename Type1, typename Type2>
struct PContextField2: public sox::Marshallable
{
  PContextField2(){}
  PContextField2(Type1 t1, Type2 t2):f1(t1),f2(t2){}
  Type1 f1;
  Type2 f2;
  virtual void marshal(sox::Pack &p) const 
  {
    p << f1 << f2;
  }
  virtual void unmarshal(const sox::Unpack &p)
  {
    p >> f1 >> f2;
  }
};

// todo : seperate
template <typename KeyType, typename PropertyType>
inline bool getPropertyByKey(const std::map<KeyType, std::string> & pmap, const KeyType & key, PropertyType * pProp)
{
  typename std::map<KeyType, std::string>::const_iterator it = pmap.find(key);
  if(it == pmap.end())
  {
    return false;
  }
  
  PContextField1<PropertyType> msg;
  if(!utility::StringToPacket(it->second, msg))
  {
    return false;
  }

  *pProp = msg.f1;
  return true;
}

template <typename KeyType, typename PropertyType>
inline PropertyType getPropertyByKey(const std::map<KeyType, std::string> & pmap, const KeyType & key)
{
  PropertyType prop(0);
  getPropertyByKey(pmap, key, &prop);
  return prop;
}

template <typename KeyType, typename PropertyType>
inline void setPropertyByKey(std::map<KeyType, std::string> & pmap, const KeyType & key, const PropertyType & prop)
{
  pmap[key] = utility::PacketToString(PContextField1<PropertyType>(prop));
}

template <typename KeyType, typename PropertyType>
inline bool addPropertyByKey(std::map<KeyType, std::string> & pmap, const KeyType & key, const PropertyType & prop)
{
  typename std::map<KeyType, std::string>::iterator it = pmap.find(key);
  if(it != pmap.end())
  {
    return false;
  }
  setPropertyByKey(pmap, key, prop);
  return true;
}

