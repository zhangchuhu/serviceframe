#ifndef SERVER_COMMON___COMMON___STAGE_H
#define SERVER_COMMON___COMMON___STAGE_H
#include <set>
#include <deque>
#include <sstream>

#include "server_common/common/PThread.h"
#include "server_common/common/TMessageQueue.h"
#include "server_common/common/LinuxMutex.h"
#include "server_common/common/ScopedLock.h"

namespace server
{
namespace xlinux
{

class IStageTask
{
public:
        virtual ~IStageTask() {}

        virtual void doTask() = 0;
};

//TODO: 自动增减线程数
class IStage
{
public:
        virtual ~IStage() {}

        virtual void setInputQueueLimit(uint64_t sz) = 0;
        virtual void setWorkerThreadNum(uint32_t sz) = 0;

        // 0 success, -1 fail(maybe queue is full.)
        // pTask will be deleted by IStage.
        virtual int pushTask(IStageTask* pTask) = 0;
};

class IStageAware
{
public:
        IStageAware() : m_pStage(NULL) {}

        void setStage(IStage* p) { m_pStage = p; }
        IStage* m_pStage;
};

class StageImp : public IStage
{
public:
        StageImp(uint64_t u64ThreadNum, uint32_t sz)
            : m_u64ActiveThreadNum(0)
        {
            m_u64ThreadNum = u64ThreadNum;
            m_tmqInput.setQueueLimit(sz);

            for (uint32_t i = u64ThreadNum; i != 0; --i)
            {
                PThread* pThread = new PThread(&StageImp::threadRun, this);
                assert(pThread != NULL);
                m_setWorker.insert(pThread);
            }
        }

        ~StageImp()
        {
            for (std::set<PThread*>::iterator it = m_setWorker.begin(); it != m_setWorker.end(); ++it)
            {
                delete *it;
            }
        }

        virtual void setInputQueueLimit(uint64_t sz)
        {
            m_tmqInput.setQueueLimit(sz);
        }

        virtual void setWorkerThreadNum(uint32_t sz)
        {
            m_u64ThreadNum = sz;

            if (sz > m_setWorker.size())
            {
                for (uint32_t i = (sz - m_setWorker.size()); i != 0; --i)
                {
                    PThread* pThread = new PThread(&StageImp::threadRun, this);
                    assert(pThread != NULL);
                    m_setWorker.insert(pThread);
                }
            }
        }

        // 0 success, -1 fail(maybe queue is full.)
        virtual int pushTask(IStageTask* pTask)
        {
            return m_tmqInput.push(pTask);
        }

        static void* threadRun(void* p)
        {
            StageImp* pStage = (StageImp*)p;
            while (true)
            {
                IStageTask* pTask = pStage->m_tmqInput.pop();

                {
                    ScopedLock<LinuxMutex> lk(pStage->m_mtxActive);
                    ++(pStage->m_u64ActiveThreadNum);
                }

                pTask->doTask();
                delete pTask;

                {
                    ScopedLock<LinuxMutex> lk(pStage->m_mtxActive);
                    --(pStage->m_u64ActiveThreadNum);
                }
            }

            return NULL;
        }

        // "thread active/all: 5/10, input queue cur_size/limit_size: 2000"
        std::string dumpStatistic()
        {
            std::stringstream ss;
            m_mtxActive.lock();
            ss << "thread active/all: " << m_u64ActiveThreadNum;
            m_mtxActive.unlock();
            ss << "/" << m_u64ThreadNum << ", input queue cur_size/limit_size: " <<
                    m_tmqInput.size() << "/" << m_tmqInput.getQueueLimit();

            return ss.str();
        }

        uint32_t getActiveNum()
        {
            ScopedLock<LinuxMutex> lk(m_mtxActive);
            return m_u64ActiveThreadNum;
        }

        uint32_t m_u64ThreadNum;
        server::xlinux::LinuxMutex m_mtxActive;
        uint32_t m_u64ActiveThreadNum;

        server::xlinux::TMessageQueue<std::deque<IStageTask*> > m_tmqInput;

        std::set<PThread*> m_setWorker;
private:
        StageImp();
};

}
}


#endif /* STAGE_H_ */
