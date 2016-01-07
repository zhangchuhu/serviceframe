#pragma once 

#include <string>
#include "common/int_types.h"


namespace protocol 
{
	namespace imstatistics 
	{
		/****************************************************
		 *	                  Type Define                   *
		 ****************************************************/
		enum E_P2P_ERRCODE
		{
			E_P2P_IO_ERR = 0,
			E_P2P_IO_ERR_OPEN,
			E_P2P_IO_ERR_CLOSE,
			E_P2P_IO_ERR_WRITE,
			E_P2P_IO_ERR_READ,
			E_P2P_NET_ERR,
			E_P2P_NET_CONN_TIMEOUT,
			E_P2P_NET_NODATA_TIMEOUT,
			E_P2P_NET_TRANSMIT_BRIDGE,
			E_P2P_FAIL_TRANSMIT,
			E_P2P_NO_FILES_SENT,
			E_P2P_DUPLICATE_STATE,
			E_P2P_IO_ERR_RENAME,
			E_P2P_MISMATCH_BRIDGE_METHOD,
			E_P2P_IO_ERR_CREATE,
			E_P2P_DUPLICATE_TCP_LOGIN,
			E_P2P_TRANSMIT_SUCCESS,
			E_P2P_TRANSMIT_CANCEL,

			E_P2P_TRANSMIT_REFUSE,
			E_P2P_PCONN_NOT_SETUP,
			E_P2P_PCONN_SUCCESS,
			E_P2P_PCONN_FAILED,
			E_P2P_PCONN_TIMEOUT,
			E_P2P_USER_CONN_WAITTIME,
			E_P2P_ACTUAL_CONN_COST,
			
			E_P2P_ERR_UNKNOWN = 0xff,
			E_P2P_ERR_INVALID
		};

		enum E_P2P_ERRMODULE
		{
			E_P2P_MD_IMP = 0,
			E_P2P_MD_FILE,
			E_P2P_MD_STREAM,
			E_P2P_MD_TRANSM,
			E_P2P_MD_RSMMGR,
			E_P2P_MD_UNKNOWN
		};

		enum E_P2P_TRANSOBJ
		{
			E_P2P_FILE = 0,
			E_P2P_DIR
		};

		/****************************************************
		 *					  ScreenShot 		    		*
		 *--------------------------------------------------*
		 *					  Type Define					*
		 ****************************************************/
		enum E_IMAGE_MY_ISP
		{
			E_MYISP_CTL = 0,
			E_MYISP_CNC,

			E_MYISP_UNKNOWN
		};
		
		enum E_IMAGE_ACTION
		{
			E_IMAGE_DOWNLOAD = 0,
			E_IMAGE_UPLOAD
		};

		struct CImageTroubleShot
		{
			//member list.
			uint32_t 		m_uUid;				//who raise
			E_IMAGE_ACTION 	m_eImageAction;		//image behaviour (download, upload)
			E_IMAGE_MY_ISP  m_eMyIspType;		//my isp type
			std::string		m_strImageName;		//image name
			uint32_t		m_uImageSize;		//image size
			uint32_t 		m_uCTLTimeUsed;		//CTL time used (ms)
			uint32_t 		m_uCNCTimeUsed;		//CNC time used (ms)

			std::string 	m_strCTLImageUrl;	//CTL URL
			std::string 	m_strCNCImageUrl;	//CNC URL
			
			uint32_t		m_uCTLErrorCode;	//CTL error code
			uint32_t 		m_uCNCErrorCode;	//CNC error code	
			std::string 	m_strMyLastWords;	//my last words
			
			std::string 	m_strReserve;		//the reserved msg
			
			//Structor & Destructor.
			CImageTroubleShot(void) : 
				m_uUid(0),
				m_eImageAction(E_IMAGE_DOWNLOAD),
				m_eMyIspType(E_MYISP_CTL),
				m_strImageName(""),
				m_uImageSize(0),
				m_uCTLTimeUsed(0),
				m_uCNCTimeUsed(0),
				m_strCTLImageUrl(""),
				m_strCNCImageUrl(""),
				m_uCTLErrorCode(0),
				m_uCNCErrorCode(0),
				m_strMyLastWords(""),
				m_strReserve("")
				{}
			
			CImageTroubleShot(const CImageTroubleShot &ImageTroubleShotObj) : 
				m_uUid(ImageTroubleShotObj.m_uUid),
				m_eImageAction(ImageTroubleShotObj.m_eImageAction),
				m_eMyIspType(ImageTroubleShotObj.m_eMyIspType),
				m_strImageName(ImageTroubleShotObj.m_strImageName),
				m_uImageSize(ImageTroubleShotObj.m_uImageSize),
				m_uCTLTimeUsed(ImageTroubleShotObj.m_uCTLTimeUsed),
				m_uCNCTimeUsed(ImageTroubleShotObj.m_uCNCTimeUsed),
				m_strCTLImageUrl(ImageTroubleShotObj.m_strCTLImageUrl),
				m_strCNCImageUrl(ImageTroubleShotObj.m_strCNCImageUrl),
				m_uCTLErrorCode(ImageTroubleShotObj.m_uCTLErrorCode),
				m_uCNCErrorCode(ImageTroubleShotObj.m_uCNCErrorCode),
				m_strMyLastWords(ImageTroubleShotObj.m_strMyLastWords),
				m_strReserve(ImageTroubleShotObj.m_strReserve)
				{}
				
			~CImageTroubleShot(void){}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << (uint32_t)m_eImageAction << (uint32_t)m_eMyIspType << m_strImageName << m_uImageSize;
				p << m_uCTLTimeUsed << m_uCNCTimeUsed << m_strCTLImageUrl << m_strCNCImageUrl;
				p << m_uCTLErrorCode << m_uCNCErrorCode << m_strMyLastWords << m_strReserve;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uImageAction;
				uint32_t uMyIspType;
			
				p >> m_uUid >> uImageAction >> uMyIspType >> m_strImageName >> m_uImageSize;
				p >> m_uCTLTimeUsed >> m_uCNCTimeUsed >> m_strCTLImageUrl >> m_strCNCImageUrl;
				p >> m_uCTLErrorCode >> m_uCNCErrorCode >> m_strMyLastWords >> m_strReserve;
			
				m_eImageAction = (E_IMAGE_ACTION)uImageAction;
				m_eMyIspType = (E_IMAGE_MY_ISP)uMyIspType;
			}
			
		};

		enum E_ACTION_RESULT
		{
			E_ACTION_SUCCESS = 0,
			E_ACTION_FAILED
		};

		enum E_IMAGE_TYPE
		{
			E_IMAGE_SCRRENSHOT = 0,
			E_IMAGE_SMILE,
			E_IMAGE_VOICE,

			E_IMAGE_UNKNOWN = 0xff
		};

		/**
		 * 截图统计的公共基本信息.
		 **/
		struct CImageStaticsShotBase : public sox::Marshallable
		{
			E_IMAGE_ACTION	  m_eImageAction;	//image behaviour (download, upload)
			E_IMAGE_TYPE	  m_eImageType; 	//image service type(screenshot, smile, voice)
			std::string 	  m_strImageName;	//image name
			uint32_t		  m_uImageSize; 	//image size
			bool			  m_isFromPc;		//indicate from pc or phone. default: pc
			E_IMAGE_MY_ISP	  m_eMyIspType; 	//my isp type
			uint64_t		  m_uImageAge;		//Image age(ms)
			std::string 	  m_strReserve; 	//the reserved msg


			//Structor & Destructor.
			CImageStaticsShotBase(void) : 
			m_eImageAction(E_IMAGE_DOWNLOAD),
			m_eImageType(E_IMAGE_UNKNOWN),
			m_strImageName(""),
			m_uImageSize(0),
			m_isFromPc(true),
			m_eMyIspType(E_MYISP_UNKNOWN),
			m_uImageAge(0),
			m_strReserve("")
			{}
			
			CImageStaticsShotBase(const CImageStaticsShotBase &ImageStaticShotObj) : 
				m_eImageAction(ImageStaticShotObj.m_eImageAction),
				m_eImageType(ImageStaticShotObj.m_eImageType),
				m_strImageName(ImageStaticShotObj.m_strImageName),
				m_uImageSize(ImageStaticShotObj.m_uImageSize),
				m_isFromPc(ImageStaticShotObj.m_isFromPc),
				m_eMyIspType(ImageStaticShotObj.m_eMyIspType),
				m_uImageAge(ImageStaticShotObj.m_uImageAge),
				m_strReserve(ImageStaticShotObj.m_strReserve)
				{}
			
			~CImageStaticsShotBase(void){}
			
			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				p << (uint32_t)m_eImageAction << (uint32_t)m_eImageType << m_strImageName << m_uImageSize;
				p << m_isFromPc << (uint32_t)m_eMyIspType << m_uImageAge << m_strReserve;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uImageAction;
				uint32_t uImageType;
				uint32_t uMyIspType;

				p >> uImageAction >> uImageType >> m_strImageName >> m_uImageSize;
				p >> m_isFromPc >> uMyIspType >> m_uImageAge >> m_strReserve;

				m_eImageAction = (E_IMAGE_ACTION)uImageAction;
				m_eImageType = (E_IMAGE_TYPE)uImageType;
				m_eMyIspType = (E_IMAGE_MY_ISP)uMyIspType;
			}
		};

		/**
		 * 最终下载,上传成功统计.
		 * 下载成功:遇到第一个下载成功时成功.
		 * 上传成功:遇到第一个上传成功时成功.
		 **/
		struct CImageStaticsShotFinalSuccess : public CImageStaticsShotBase
		{
			uint32_t  m_uTimeUsed;		//download,upload time used (ms)

			CImageStaticsShotFinalSuccess(void):
				CImageStaticsShotBase(),
				m_uTimeUsed(0)
				{}

			~CImageStaticsShotFinalSuccess(void){}
	
			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				CImageStaticsShotBase::marshal(p);
				p << m_uTimeUsed;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				CImageStaticsShotBase::unmarshal(p);
				p >> m_uTimeUsed;
			}
		};

		/**
		 * 单次下载,上传行为统计.
		 **/
		struct CImageStaticsShotSingle : public CImageStaticsShotBase
		{
			E_ACTION_RESULT   m_eActionResult;	//success or failure	
			E_IMAGE_MY_ISP	  m_eImageIspType;	//target image isp type(ctl, cnc)
			uint32_t		  m_uErrorCode;     //error code

			CImageStaticsShotSingle(void) : 
				CImageStaticsShotBase(),
				m_eActionResult(E_ACTION_SUCCESS),
				m_eImageIspType(E_MYISP_UNKNOWN),
				m_uErrorCode(0)
				{}

			~CImageStaticsShotSingle(void){}

			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				CImageStaticsShotBase::marshal(p);
				p << (uint32_t)m_eActionResult << (uint32_t)m_eImageIspType << m_uErrorCode;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				uint32_t uActionResult;
				uint32_t uImageIspType;
				
				CImageStaticsShotBase::unmarshal(p);
				p >> uActionResult >> uImageIspType >> m_uErrorCode;

				m_eActionResult = (E_ACTION_RESULT)uActionResult;
				m_eImageIspType = (E_IMAGE_MY_ISP)uImageIspType;
			}
			
		};

		/**
		 * 最终下载,上传失败统计.
		 * 下载失败:最终CTL,CNC两边尝试下载都失败.
		 * 上传失败:最终CTL,CNC两边尝试上传都失败.
		 **/
		struct CImageStaticsShotFinalFail : public CImageStaticsShotBase
		{
			uint32_t	m_uCTLErrorCode;	//CTL error code
			uint32_t	m_uCNCErrorCode;	//CNC error code

			CImageStaticsShotFinalFail(void) : 
				CImageStaticsShotBase(),
				m_uCTLErrorCode(0),
				m_uCNCErrorCode(0)
				{}
			
			~CImageStaticsShotFinalFail(void){}
			
			//Marshal & Unmarshal
			virtual void marshal(sox::Pack &p) const
			{
				CImageStaticsShotBase::marshal(p);
				p << m_uCTLErrorCode << m_uCNCErrorCode;
			}
			
			virtual void unmarshal(const sox::Unpack &p)
			{
				CImageStaticsShotBase::unmarshal(p);
				p >> m_uCTLErrorCode >> m_uCNCErrorCode;
			}
		};

	}
}

