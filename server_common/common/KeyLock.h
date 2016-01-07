#ifndef KEYLOCK_H_
#define KEYLOCK_H_

#include "LinuxMutex.h"

namespace server {
namespace xlinux {

class KeyLock
{
    private:
        struct Value
        {
                Value() : m_iRef(0) {}

                int m_iRef;
                LinuxMutex m_mtxSingleKey;
        };

    public:
        void lock(const std::string& key)
        {
            Value* pValue = NULL;

            //global lock
            m_mtx.lock();
            std::pair<std::map<std::string, Value *>::iterator, bool> insRes =
                    m_map.insert(std::pair<std::string, Value *>(key, NULL));

            if (insRes.second)
            {
                (insRes.first)->second = new Value();
            }

            pValue = (insRes.first)->second;
            ++ (pValue->m_iRef);
            m_mtx.unlock();

             pValue->m_mtxSingleKey.lock();
        }

        void unlock(const std::string& key)
        {
            m_mtx.lock();
            std::map<std::string, Value *>::iterator it = m_map.find(key);
            if (it != m_map.end())
            {
                it->second->m_mtxSingleKey.unlock();
                it->second->m_iRef--;

                if (it->second->m_iRef == 0)
                {
                    delete it->second;
                    m_map.erase(it);
                }
            }
            m_mtx.unlock();
        }

    private:
        LinuxMutex m_mtx;
        std::map<std::string, Value *> m_map;
};

class ScopedKLock
{
public:
        ScopedKLock(KeyLock& keyLock, const std::string& strKey) : m_keyLock(keyLock), m_strKey(strKey)
        {
            m_keyLock.lock(m_strKey);
        }

        ~ScopedKLock()
        {
            m_keyLock.unlock(m_strKey);
        }
private:
        KeyLock& m_keyLock;
        const std::string& m_strKey;
};

}
}
#endif
