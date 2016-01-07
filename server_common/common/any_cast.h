#ifndef _ANY_CAST_H_
#define _ANY_CAST_H_

#include <sstream>

namespace JLexical
{
       namespace
       {
               template <class T>
               struct RMconst
               {
                       typedef T type;
               };
               template <class T>
               struct RMconst<const T>
               {
                       typedef T type;
               };
               template <class T>
               struct RMvolatile
               {
                       typedef T type;
               };
               template <class T>
               struct RMvolatile<volatile T>
               {
                       typedef T type;
               };
               template <class T>
               struct RMarray
               {
                       typedef T type;
               };
               template <class T,int N>
               struct RMarray<T[N]>
               {
                       typedef T* type;
               };
               template <class T>
               struct Origin
               {
                       typedef typename RMarray<typename RMvolatile<typename
RMconst<T>::type>::type>::type type;
               };
               template <class T,class U>
               struct Convert
               {
                       typedef typename Origin<T>::type FT;
                       typedef typename Origin<U>::type FU;
                       struct Big
                       {
                               char d[2];
                       };
                       static FT makeT();
                       static char test(FU o);
                       static Big test(...);
               public:
                       enum {OK = (sizeof(test(makeT()))==sizeof(char)?1:0) };
               };
               template <class T,class U,int C>
               struct CastImpl;
               template <class T,class U>
               struct CastImpl<T,U,0>
               {
                       static void Do(const T& source,U& dest)
                       {
								std::stringstream ss;
								ss<<source;
								ss>>dest;
                       }
               };
               template <class T>
               struct CastImpl<T,std::string,0>
               {
                       static void Do(const T& source,std::string& dest)
                       {
								std::stringstream ss;
								ss<<source;
								ss.str().swap(dest);
                       }
               };
               template <class T>
               struct CastImpl<std::string,T,0>
               {
                       static void Do(const std::string& source,T& dest)
                       {
                               std::stringstream ss(source);
                               ss>>dest;
                       }
               };
               template <class T,class U>
               struct CastImpl<T,U,1>
               {
                       static void Do(const T& source,U& dest)
                       {
                               dest=source;
                       }
               };
       }
       template <class T,class U>
       void Cast(const T& source,U& dest)
       {
               CastImpl<T,U,Convert<T,U>::OK>::Do(source,dest);
       }
}

#endif

