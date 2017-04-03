//*****************************************************
//
// Software Render Engine
// Version 0.01 by XJL
//
// File: SRE_PipeLine.h
// Date: 2016/11/16
// Description:
//       Defines all PipeLine relative classes and functions
//
//
//
//
//*****************************************************
#ifndef _SRE_PIPELINE_
#define _SRE_PIPELINE_

#include "SRE_Math.h"
#include "SRE_Resources.h"
#include "SRE_Shader.h"
#include "SRE_GlobalsAndUtils.h"

namespace SRE {
    //===========================================
    //Functions
    //
    //magnitude of the cross product:(v1-v0) and (V2-v0).
    //E01(P)=(V2.x-V0.x)*(V1.y-V0.y)-(V2.y-V0.y)*(V1.x-V0.x).
    //v0->v1->v2, CW
    //===========================================
    inline FLOAT EdgeFunction2D(const VEC4& v1, const VEC4& v2, const VEC4& v3)
    {
        return (v3.x - v1.x) * (v2.y - v1.y) - (v3.y - v1.y) * (v2.x - v1.x);
    }

    inline FLOAT EdgeFunction2D(FLOAT v1x, FLOAT v1y, FLOAT v2x, FLOAT v2y, FLOAT v3x, FLOAT v3y)
    {
        return (v3x - v1x) * (v2y - v1y) - (v3y - v1y) * (v2x - v1x);
    }


   //=============================
	//Class VariableBuffer
	//
	//
	//
	//=============================
    class VariableBuffer
    {
    public:
        VariableBuffer():
            ViewportSize(),
            WorldViewProj(),
            WorldView(),
            ViewProj(),
            World(),
            View(),
            Project()
        {}
        virtual ~VariableBuffer(){}

    public:
        VEC2     ViewportSize;
        MAT44  WorldViewProj;
        MAT44  WorldView;
        MAT44  ViewProj;
        MAT44  World;
        MAT44  View;
        MAT44  Project;
    };


    //=============================
	//Class ConstantBuffer
	//
	//
	//
	//=============================
	class ConstantBuffer
	{
    public:
        ConstantBuffer():
            primitiveTopology(SRE_PRIMITIVETYPE_TRIANGLELIST),
            FillMode(SRE_FILLMODE_SOLID),
            CullEnable(SRE_TRUE),
            CullMode(SRE_CULLMODE_CCW),
            ClipEnable(SRE_TRUE),
            ZEnable(SRE_TRUE),
            AlphaTest(SRE_FALSE),
            AlphaBlend(SRE_FALSE),
            OutputChannel(SRE_TYPE_FRAMEBUFFER)
        {}
        virtual ~ConstantBuffer(){}

    public:
        SREVAR primitiveTopology;
        SREVAR FillMode;
        SREVAR CullEnable;
        SREVAR CullMode;
        SREVAR ClipEnable;
        SREVAR ZEnable;
        SREVAR AlphaTest;
        SREVAR AlphaBlend;
        SREVAR OutputChannel;
	};





    //=============================
	//Class CallBackFunctions
	//
	//
	//=============================
    class CallBackFunctions
    {
    public:
        virtual ~CallBackFunctions()
        {}
        virtual void HandleElement()=0;
        virtual void OnCancel()=0;
        virtual void OnPause()=0;
        virtual void OnResume()=0;
        virtual void OnRunError()=0;
        virtual void OnRunFinish()=0;
        virtual void OnStart()=0;

    };

    class ObserverCallBack
    {
	public:
		 virtual ~ObserverCallBack(){}
		 virtual void HandleMessage(SREVAR message, USINT id=0)=0;
    };



    //=============================
	//Class BasicObserver
	//
	//
	//=============================
	class BasicObserver
	{
    public:
        BasicObserver(ObserverCallBack* callback=nullptr):
            m_callback(callback)
        {}
        BasicObserver(const BasicObserver & other):
        	m_callback(other.m_callback)
		{}
        virtual ~BasicObserver(){}

        void SetProcessor(ObserverCallBack* callback)
        {
            m_callback = callback;
        }

        void Notify(SREVAR message, USINT id)
        {
            if(nullptr != m_callback)
             m_callback->HandleMessage(message, id);
        }

        BasicObserver & operator=(const BasicObserver & other)
        {
        	if(this != &other)
			{
				m_callback = other.m_callback;
			}
			return *this;
        }

    protected:
        ObserverCallBack* m_callback;

	};




    //=============================
	//Class BasicProcessor
	//
	//
	//=============================
	class BasicProcessor:public BaseTask
	{
    public:
        BasicProcessor(USINT id=0,
							         PIOBUFFER * input=nullptr,
                              PIOBUFFER * output=nullptr,
                              BasicObserver     * observer=nullptr,
                              CallBackFunctions * callbacks=nullptr):
            BaseTask(),
			   m_id(id),
            m_pInputQueue(input),
            m_pOutputQueue(output),
            m_thread(),
            m_callBacks(callbacks),
            m_mutex(),
		    	m_pObserver(observer),
            m_cond(),
            m_resMutex(),
            m_Cancel(false),
            m_Pause(false),
            m_Started(false)
        {}
        virtual ~BasicProcessor()
        {}


        void Run();
		  void Start();

        inline void Pause();
        inline void Resume();
        inline void Cancel();

        inline std::shared_ptr<BasicIOElement> GetInput();
        inline void Output(BasicIOElement* out);
        inline void Output(std::shared_ptr<BasicIOElement> out);
        inline void SetInputQueue(PIOBUFFER * inputQueue);
        inline void SetOutputQueue(PIOBUFFER * outpueQueue);
        inline void SetObserver(BasicObserver  * observer);
        inline void Input(BasicIOElement* element);
        inline void SetCallBacks(CallBackFunctions * callbacks);
        inline void Join();


        BasicProcessor(const BasicProcessor & other) = delete;
        BasicProcessor & operator=(const BasicProcessor & other) = delete;

	public:
		  USINT                                             m_id;

    protected:
        PIOBUFFER *   m_pInputQueue;
        PIOBUFFER *   m_pOutputQueue;
        std::thread                                        m_thread;
        CallBackFunctions   *                          m_callBacks;
        std::mutex                                        m_mutex;

    protected:
        BasicObserver  *                              m_pObserver;
        std::condition_variable                m_cond;
        std::mutex                                     m_resMutex;

        bool m_Cancel;
        bool m_Pause;
        bool m_Started;





	};

    void BasicProcessor::Pause()
    {
        m_Pause = true;
    }

    void BasicProcessor::Cancel()
    {
        m_Cancel = true;
        if(nullptr != m_pInputQueue)
           m_pInputQueue->StopWait();
		if(nullptr != m_pOutputQueue)
           m_pOutputQueue->StopWait();

        m_cond.notify_one();
    }

    void BasicProcessor::Resume()
    {
        if(nullptr != m_callBacks)
           m_callBacks->OnResume();
        m_Pause = false;
        m_cond.notify_one();

    }

    void BasicProcessor::SetInputQueue(PIOBUFFER * inputQueue)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pInputQueue = inputQueue;
    }

    void BasicProcessor::SetOutputQueue(PIOBUFFER * outpueQueue)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pOutputQueue = outpueQueue;
    }

    void BasicProcessor::SetObserver(BasicObserver  * observer)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pObserver = observer;
    }

    void BasicProcessor::Input(BasicIOElement* element)
    {
        m_pInputQueue->push(element);
    }

    void BasicProcessor::SetCallBacks(CallBackFunctions * callbacks)
    {
        m_callBacks = callbacks;
    }

    std::shared_ptr<BasicIOElement> BasicProcessor::GetInput()
    {
        return m_pInputQueue->wait_and_pop();
    }

    void BasicProcessor::Output(BasicIOElement* out)
    {
        m_pOutputQueue->push(out);
    }

    void BasicProcessor::Output(std::shared_ptr<BasicIOElement> out)
    {
        m_pOutputQueue->push(out);
    }

    void BasicProcessor::Join()
    {
        if(m_thread.joinable())
           m_thread.join();

    }


   //=============================
	//Temp structures for IO
	//=============================
	class _index_ver_:public BasicIOElement
	{//A temp data structure used for skipping vertex process
     public:
	   INT index;
	   BYTE* vertex;

     public:
	   _index_ver_(INT i, BYTE* v):
	       index(i),
	       vertex(v)
       {}
       _index_ver_():
	       index(-2),
	       vertex(nullptr)
       {}
       ~_index_ver_()
       {
           delete[] vertex;
       }

       _index_ver_(const _index_ver_ & other) = delete;
       _index_ver_ & operator=(const _index_ver_ & other) = delete;
	};

    class _vertex_:public BasicIOElement
	{//A temp data structure used for clipping
	public:
	    VERTEX4 v;
	    FLOAT   t;
	    INT     s;
	    INT     e;

        _vertex_(VERTEX4& _v, FLOAT _t, INT _s, INT _e):
            v(_v),t(_t),s(_s),e(_e)
        {}
        _vertex_():
            v(),t(0),s(0),e(0)
        {}
	};


	class _Triangle_:public BasicIOElement
	{//A temp data structure for primitive output
    public:
        _Triangle_(VSOutput * v1, VSOutput * v2, VSOutput * v3)
        {
            v[0]=*v1;v[1]=*v2;v[2]=*v3;
        }
        _Triangle_(VSOutput & v1, VSOutput & v2, VSOutput & v3)
        {
            v[0]=v1;v[1]=v2;v[2]=v3;
        }

        ~_Triangle_()
        {

        }

    public:
        VSOutput  v[3];
	};

	template<typename T>
	class _pixelBlock_:public BasicIOElement
	{
    public:
        _pixelBlock_(USINT _sx=0, USINT _sy=0, USINT _distX=0, USINT _distY=0, FLOAT _area,bool _isEnd=false, bool _setArg=false,
                     std::shared_ptr<T> _data=nullptr):
            sx(_sx), sy(_sy), distX(_distX), distY(_distY),
            area(_area),
            isEnd(_isEnd),
            setArg(_setArg),
            tempData(_data)
        {}
        _pixelBlock_(const _pixelBlock_<T> & other):
            sx(other.sx), sy(other.sy),
            distX(other.distX), distY(other.distY),
            area(other.area),
            isEnd(other.isEnd),
            setArg(other.setArg),
            tempData(other.tempData)
        {}
        ~_pixelBlock_()
        {}

    public:
	    USINT   sx, sy;
	    USINT   distX, distY;
	    FLOAT  area;
	    bool    isEnd;
	    bool    setArg;
	    std::shared_ptr<T> tempData;
	};


    //=============================
	//Class InputAssembler
	//
	//
	//
	//=============================
	class InputAssembler:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
	{
    public:
        InputAssembler(USINT id=0,
					            PIOBUFFER * output=nullptr,
                           BasicObserver *  observer=nullptr):
            BasicProcessor(id, nullptr, output, observer, this),
            m_vertexBuffers(),
            m_indexBuffers(),
            m_pConstantBuffer(nullptr),
            m_pCurrentHandleVbuffer(nullptr),
            m_pCurrentHandleIbuffer(nullptr),
            m_currentIndex(0),
            m_currentIndex2(0),
            m_valve(false)
        {}
        virtual ~InputAssembler()
        {}


        void HandleElement();
        void HandleMessage(SREVAR message, USINT id=0);
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void ClearVertexbufferAndIndexbuffer();
        void SetVertexAndIndexBuffers(VBUFFER* vertexbuffer, IBUFFER* indexbuffer=nullptr);
        inline void SetConstantBuffer(const ConstantBuffer * cbuffer);
        inline void BeginSceneSetting();
        inline void EndSceneSetting();

        InputAssembler(const InputAssembler & other) = delete;
        InputAssembler & operator=(const InputAssembler & other) = delete;

    private:
        void SendVertex(INT & actualIndex);

    private:
        std::queue<VBUFFER*>       m_vertexBuffers;
        std::queue<IBUFFER*>        m_indexBuffers;
        const ConstantBuffer *        m_pConstantBuffer;

        VBUFFER*                             m_pCurrentHandleVbuffer;
        IBUFFER*                              m_pCurrentHandleIbuffer;

        INT                                 m_currentIndex;
        INT                                 m_currentIndex2;

        bool                               m_valve;

	};

    void InputAssembler::SetConstantBuffer(const ConstantBuffer * cbuffer)
    {
        if(nullptr != cbuffer)
           m_pConstantBuffer = cbuffer;
    }

    void InputAssembler::BeginSceneSetting()
    {
		std::lock_guard<std::mutex> lock(m_resMutex);
        m_vertexBuffers.push(nullptr);
        m_valve = false;
    }

    void InputAssembler::EndSceneSetting()
    {
		std::lock_guard<std::mutex> lock(m_resMutex);
        m_vertexBuffers.push(nullptr);
    	m_valve = true;

    	m_cond.notify_one();
    }

    //=============================
	//Class VertexProcessor
	//
	//
	//
	//=============================
    class VertexProcessor:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
    {
    public:
        VertexProcessor(USINT id=0,
                                 PIOBUFFER * input=nullptr,
                                 PIOBUFFER * output=nullptr,
                                 BasicObserver * observer=nullptr):
            BasicProcessor(id, input, output, observer, this),
            m_pVertexShader(nullptr),
            m_pVariableBuffer(nullptr),
			   m_pCVertexShader(nullptr),
            m_pCVariableBuffer(nullptr),
            m_cachedOutVertexIdx(-1)
        {
            m_cachedOutVertex[0] = nullptr;
            m_cachedOutVertex[1] = nullptr;
            m_cachedVertexIndex[0]=-1;
            m_cachedVertexIndex[1]=-1;
            m_cachedVertexIndex[2]=-1;
            m_cachedPriority[0]=4;
            m_cachedPriority[1]=2;
            m_cachedPriority[2]=1;
        }
        virtual ~VertexProcessor()
        {
           if(nullptr != m_cachedOutVertex[0])
             delete m_cachedOutVertex[0];
           if(nullptr != m_cachedOutVertex[1])
             delete m_cachedOutVertex[1];
        }

        void HandleElement();
        void HandleMessage(SREVAR message, USINT id=0);
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        inline void SetVertexShader(VertexShader * vshader);
        inline void SetVariableBuffer(VariableBuffer* varbuffer);

        VertexProcessor(const VertexProcessor & other) = delete;
        VertexProcessor & operator=(const VertexProcessor & other) = delete;

	protected:
		void   SetArgus();
      void   SetOutputVertex(VSOutput* out);

    protected:
        VertexShader*   m_pVertexShader;
        VariableBuffer*  m_pVariableBuffer;
        VertexShader*   m_pCVertexShader;
        VariableBuffer*  m_pCVariableBuffer;

        SINT                  m_cachedOutVertexIdx;
        VSOutput*         m_cachedOutVertex[2];
        VSOutput          m_cachedVertex[3];
        USINT               m_cachedVertexIndex[3];
        USINT               m_cachedPriority[3];

    };

    void VertexProcessor::SetVertexShader(VertexShader * vshader)
    {
        if(nullptr != vshader)
        m_pCVertexShader = vshader;
    }

    void VertexProcessor::SetVariableBuffer(VariableBuffer* varbuffer)
    {
        if(nullptr != varbuffer)
        m_pCVariableBuffer = varbuffer;
    }

   //=============================
	//Class PrimitiveAssembler
	//*Unused*
	//
	//
	//=============================
	/**************************************************************
    class PrimitiveAssembler:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
    {
    public:
        PrimitiveAssembler(USINT id=0,
						             BasicIOBuffer<BasicIOElement> * input=nullptr,
                                     BasicIOBuffer<BasicIOElement>    * output=nullptr,
                                     BasicObserver * observer=nullptr):
            BasicProcessor(id, input, output, observer, this)
        {}
        virtual ~PrimitiveAssembler()
        {}

        void HandleElement();
        void HandleMessage(SREVAR message, USINT id=0);
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        PrimitiveAssembler(const PrimitiveAssembler & other) = delete;
        PrimitiveAssembler & operator=(const PrimitiveAssembler & other) = delete;


    };
    *******************************************************************************/


    //=============================
	//Class VertexPostProcessor
	//
	//ccv:
	//x: (-1,1)
	//y: (-1,1)
	//z: ( 0,1)
	//=============================
    class VertexPostProcessor:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
    {
    public:
        VertexPostProcessor(USINT id=0,
							                   PIOBUFFER * input=nullptr,
                                        PIOBUFFER * output=nullptr,
                                        BasicObserver * observer=nullptr):
            BasicProcessor(id, input, output, observer, this),
            m_pConstantBuffer(nullptr),
            m_pCurrentTriangle(nullptr),
            m_clipEpsilon(0.00),
            m_viewportHeightHalf(300),
            m_viewportWidthHalf(400)
        {
            m_clipPlaneDistance[0]= 1.0f;m_clipPlaneNormal[0].x=  1.0f;
            m_clipPlaneDistance[1]= 1.0f;m_clipPlaneNormal[1].x=-1.0f;
            m_clipPlaneDistance[2]= 1.0f;m_clipPlaneNormal[2].y=  1.0f;
            m_clipPlaneDistance[3]= 1.0f;m_clipPlaneNormal[3].y=-1.0f;
            m_clipPlaneDistance[4]= 1.0f;m_clipPlaneNormal[4].z=  1.0f;
            m_clipPlaneDistance[5]= 1.0f;m_clipPlaneNormal[5].z=-1.0f;
        }
        virtual ~VertexPostProcessor()
        {}

        void HandleElement();
        void HandleMessage(SREVAR message, USINT id=0);
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        inline void SetClipXValue(FLOAT _min, FLOAT _max);
        inline void SetClipYValue(FLOAT _min, FLOAT _max);
        inline void SetClipZValue(FLOAT _min, FLOAT _max);

        inline void SetClipTolerance(FLOAT tolerance);
        inline void SetViewportHeight(USINT height);
        inline void SetViewportWidth(USINT width);
        inline void SetConstantBuffer(const ConstantBuffer* cbuffer);

        VertexPostProcessor(const VertexPostProcessor & other) = delete;
        VertexPostProcessor & operator=(const VertexPostProcessor & other) = delete;

    protected:
        void TriangleClipping(); //clip in cvv
        void OtherTranforms(VSOutput & input);  //then divided by w
                                                //transform x to range:(0,width)
                                                //transform y to range:(0,height)
                                                //origin (0,0) is on left-top corner
                                                //(width, height) is on right-bottom corner
        void SendTriangle(VSOutput & v1, VSOutput & v2, VSOutput & v3);

    protected:
       const ConstantBuffer*                    m_pConstantBuffer;
        std::shared_ptr<BasicIOElement> m_pCurrentTriangle;
        FLOAT         m_clipPlaneDistance[6];//-x, x, -y, y, -z, z
        VEC3           m_clipPlaneNormal[6];
        FLOAT         m_clipEpsilon;
        FLOAT         m_viewportHeightHalf;
        FLOAT         m_viewportWidthHalf;//data type should be considered

    private:
        CLList<_vertex_>    m_vlist[2];

    };

    void VertexPostProcessor::SetClipTolerance(FLOAT tolerance)
    {
        m_clipEpsilon = tolerance;
    }

    void VertexPostProcessor::SetViewportHeight(USINT height)
    {
        if(height>0) m_viewportHeightHalf = ((FLOAT)height-1)/2;
    }

    void VertexPostProcessor::SetViewportWidth(USINT width)
    {
        if(width>0) m_viewportWidthHalf = ((FLOAT)width-1)/2;
    }

    void VertexPostProcessor::SetClipXValue(FLOAT _min, FLOAT _max)
    {
        m_clipPlaneDistance[0] = fabs(_min);
        m_clipPlaneDistance[1] = fabs(_max);
    }

    void VertexPostProcessor::SetClipYValue(FLOAT _min, FLOAT _max)
    {
        m_clipPlaneDistance[2] = fabs(_min);
        m_clipPlaneDistance[3] = fabs(_max);
    }

    void VertexPostProcessor::SetClipZValue(FLOAT _min, FLOAT _max)
    {
        m_clipPlaneDistance[4] = fabs(_min);
        m_clipPlaneDistance[5] = fabs(_max);
    }

	void VertexPostProcessor::SetConstantBuffer(const ConstantBuffer * cbuffer)
	{
	    if(nullptr != cbuffer)
        m_pConstantBuffer = cbuffer;
	}


    //=============================
	//Class Rasterizer
	//
	//
	//
	//=============================
    class Rasterizer:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
    {
    public:
        Rasterizer(USINT id=0,
						      PIOBUFFER * input=nullptr,
                        BasicObserver * observer=nullptr,
                        USINT samplePixelBlockSize=192):
            BasicProcessor(id, input, nullptr, observer, this),
            m_subObserver(this),
            m_pConstantBuffer(nullptr),
            m_pCurrentTriangle(nullptr),
            m_subProcessors(),
            m_subProcessorNum(0),
            m_subIndex(0),
            m_perPixelBlockSize(samplePixelBlockSize),
            m_finishCount(0),
            m_sampleStep(1),
            m_pZbuffer(nullptr),
            m_pRenderTarget(nullptr),
            m_pPixelShader(nullptr),
			   m_sigCount(0)
        {}
        virtual ~Rasterizer()
        {
           if(nullptr != m_pCurrentTriangle)
             delete m_pCurrentTriangle;
        }

        void HandleElement();
        void HandleMessage(SREVAR message, USINT id=0);
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void AddSubProcessors(USINT num=4);
        void RemoveSubProcessors(USINT num);
        void CancelSubProcessors();

        inline INT   GetSubProcessorNum();
        inline void SetZbuffer(ZBUFFER* zbuffer);
        inline void SetRenderTarget(RenderTexture* target);
        inline void SetPixelShader(PixelShader* pshader);
        inline void SetConstantBuffer(const ConstantBuffer * cbuffer);
        inline void SetSampleStep(USINT sampleStep);
        inline void SetSamplePixelBlockSize(USINT blockSize);

        Rasterizer(const Rasterizer & other) = delete;
        Rasterizer & operator=(const Rasterizer & other) = delete;

    protected:
        bool BackFaceCulling();//Using cross product(CCW, CW mode will give different result) to calculate face normal
                                           //and then test the face normal with the eye ray
                                           //if BackFaceCulling return true, we send this triangle to rasterizer
		  void  SetProcessorArgus(USINT id);

    protected:
    	  BasicObserver               m_subObserver;
        const ConstantBuffer*  m_pConstantBuffer;
        _Triangle_ *                  m_pCurrentTriangle;

        std::vector<PixelProcessor> m_subProcessors;
        INT                                        m_subProcessorNum;

        USINT                           m_subIndex;
        USINT                           m_perPixelBlockSize;
        USINT                           m_finishCount;

        USINT                                  m_sampleStep;
        ZBUFFER*                             m_pZbuffer;
        RenderTexture*                   m_pRenderTarget;
        PixelShader *                       m_pPixelShader;

		  USINT                                  m_sigCount;

		  friend class PixelProcessor;

    };

    INT Rasterizer::GetSubProcessorNum()
    {
    	return m_subProcessors.size();
    }

	void Rasterizer::SetZbuffer(ZBUFFER* zbuffer)
	{
		if(nullptr != zbuffer)
		m_pZbuffer = zbuffer;
	}

    void Rasterizer::SetRenderTarget(RenderTexture* target)
    {
        if(nullptr != target)
		m_pRenderTarget = target;
    }

    void Rasterizer::SetPixelShader(PixelShader* pshader)
    {
    	if(nullptr != pshader)
		m_pPixelShader = pshader;
    }

	void Rasterizer::SetConstantBuffer(const ConstantBuffer * cbuffer)
	{
	    if(nullptr != cbuffer)
        m_pConstantBuffer = cbuffer;
	}

	void Rasterizer::SetSampleStep(USINT sampleStep)
	{
		m_sampleStep = sampleStep;
	}

	void Rasterizer::SetSamplePixelBlockSize(USINT blockSize)
	{
      m_perPixelBlockSize = blockSize;
   }


    //=============================
	//Class PixelProcessor
	//
	//Default run on 16x16 pixel block
	//=============================
	class PixelProcessor
	{
    public:
        PixelProcessor(USINT id,
					               BasicObserver* observer=nullptr,
                              USINT sampleStep=0,
                              Rasterizer* parent=nullptr):
            m_id(id),
            m_observer(observer),
            m_inputQueue(),
            m_thread(),
            m_cond(),
            m_mutex(),
            m_cancel(false),
            m_started(false),
            m_pTri(nullptr),
            m_sampleStep(sampleStep),
            m_sx(0), m_sy(0),
            m_distX(0), m_distY(0),
            m_end(false),
            m_pParent(parent),
            m_pConstantBuffer(nullptr),
            m_pZbuffer(nullptr),
            m_pRenderTarget(nullptr),
            m_pPixelShader(nullptr)
        {}
        PixelProcessor(const PixelProcessor & other):
        	m_id(other.m_id),
            m_observer(other.m_observer),
            m_inputQueue(),
            m_thread(),
            m_cond(),
            m_mutex(),
            m_cancel(other.m_cancel),
            m_started(other.m_started),
            m_pTri(nullptr),
            m_sampleStep(other.m_sampleStep),
            m_sx(0), m_sy(0),
            m_distX(0), m_distY(0),
            m_end(false),
            m_pParent(other.m_pParent),
            m_pConstantBuffer(other.m_pConstantBuffer),
            m_pZbuffer(other.m_pZbuffer),
            m_pRenderTarget(other.m_pRenderTarget),
            m_pPixelShader(other.m_pPixelShader)
        {}
        virtual ~PixelProcessor()
        {}

        void Start();
        void Cancel();

        void PushTask(_pixelBlock_<BasicIOElement> & task);
        void ClearTask();

        inline RenderTexture* GetRenderTarget();
        inline void SetSampleStep(USINT sampleStep);
        inline void SetConstantBuffer(const ConstantBuffer * cbuffer);
        inline void SetZBuffer(ZBUFFER* zbuffer);
        inline void SetRenderTarget(RenderTexture* renderTarget);
        inline void SetPixelShader(PixelShader* pshader);
        inline void Join();

        PixelProcessor & operator=(const PixelProcessor & other) = delete;

    protected:
        void  ScanConversion();

        void  Scan_ZOn_IOff(FLOAT  W00[3], FLOAT W10[3], FLOAT W01[3], FLOAT W11[3]);
        void  Scan_ZOff_IOff(FLOAT W00[3],  FLOAT W01[3], FLOAT W10[3], FLOAT W11[3]);
        void  Scan_ZOn_IOn(FLOAT  W00[3],  FLOAT wTri);
        void  Scan_ZOff_IOn(FLOAT  W00[3], FLOAT wTri);

        std::shared_ptr<BasicIOElement>
                             GetTask();
        //inline bool   ZTest(INT pos, FLOAT interpolatedZ);

    protected:
    	  USINT                            m_id;
        BasicObserver*               m_observer;
        std::queue<_pixelBlock_<BasicIOElement>>
                                             m_inputQueue;
        std::thread                    m_thread;
        std::condition_variable
                                             m_cond;
        std::mutex                    m_mutex;

        bool                             m_cancel;
        bool                             m_started;

    protected:
        _Triangle_*                 m_pTri;
        USINT                         m_sampleStep;
        USINT                         m_sx, m_sy;
        USINT                         m_distX, m_distY;
        FLOAT                        m_wStepX[3];
        FLOAT                        m_wStepY[3];
        bool                          m_end;

        Rasterizer*                           m_pParent;
        const ConstantBuffer*         m_pConstantBuffer;
        ZBUFFER*                             m_pZbuffer;
        RenderTexture*                   m_pRenderTarget;
        PixelShader *                       m_pPixelShader;





	};

	RenderTexture* PixelProcessor::GetRenderTarget()
	{
		return m_pRenderTarget;
	}

    void PixelProcessor::SetSampleStep(USINT sampleStep)
    {
        m_sampleStep = sampleStep;
    }

    void PixelProcessor::SetConstantBuffer(const ConstantBuffer * cbuffer)
    {
        if(nullptr != cbuffer)
        m_pConstantBuffer = cbuffer;
    }

    void PixelProcessor::SetZBuffer(ZBUFFER* zbuffer)
    {
        if(nullptr != zbuffer)
        m_pZbuffer = zbuffer;
    }

    void PixelProcessor::SetRenderTarget(RenderTexture* renderTarget)
    {
        if(nullptr != renderTarget)
        m_pRenderTarget = renderTarget;
    }

    void PixelProcessor::SetPixelShader(PixelShader* pshader)
    {
        if(nullptr != pshader)
        m_pPixelShader = pshader;
    }

    void PixelProcessor::Join()
    {
    	if(m_thread.joinable())
			 m_thread.join();
    }


    /*
	bool PixelProcessor::ZTest(INT pos, FLOAT interpolatedZ)
	{
        if(interpolatedZ < m_pZbuffer->GetData(pos))
        {
            m_pZbuffer->SetData(pos, interpolatedZ);
            return true;
        }
        else
        {
            return false;
        }
	}*/




    //=============================
	//Class OutputMerger
	//*Unused*
	//
	//
	//=============================
	/**********************************************
    class OutputMerger:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
    {
    public:
        OutputMerger(USINT id=0,
					          BasicIOBuffer<BasicIOElement> * input=nullptr,
                              BasicObserver * observer=nullptr):
             BasicProcessor(id, input, nullptr, observer, this),
			 m_pConstantBuffer(nullptr),
			 m_pFrame(nullptr),
			 m_pCurTarget(nullptr),
			 m_toFrame(false)
        {}
        virtual ~OutputMerger()
        {}


        void HandleElement();
        void HandleMessage(SREVAR message, USINT id);
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        inline void SetFrameBuffer(DECOLOR* frameBuffer);
        inline void SetConstantBuffer(const ConstantBuffer*  cbuffer);
        inline void SetOutputToFrameBuffer(bool toFrame);

        OutputMerger(const OutputMerger & other) = delete;
        OutputMerger & operator=(const OutputMerger & other) = delete;

    protected:
    	const ConstantBuffer*  m_pConstantBuffer;
        DECOLOR*                    m_pFrame;
        RenderTexture*            m_pCurTarget;

        bool                            m_toFrame;


    };


    void OutputMerger::SetFrameBuffer(DECOLOR* frameBuffer)
    {
    	if(nullptr != frameBuffer)
			m_pFrame = frameBuffer;
    }


    void OutputMerger::SetConstantBuffer(const ConstantBuffer*  cbuffer)
    {
    	if(nullptr != cbuffer)
			m_pConstantBuffer = cbuffer;
    }


    void OutputMerger::SetOutputToFrameBuffer(bool toFrame)
    {
    	m_toFrame = toFrame;
    }
	 *****************************************************************/





   //=============================
	//Class BasePipeLine
	//
	//
	//=============================
	class BasePipeLine:public CallBackFunctions, public ObserverCallBack
	{
    public:
        BasePipeLine(BasicObserver* observer=nullptr):
            m_started(false),
        	   m_waited(false),
        	   m_observer(this),
        	   m_pExternObserver(observer)
        {}
        virtual ~BasePipeLine(){}

        virtual void Run() =0;
        virtual void Pause() =0;
        virtual void Cancel() =0;
        virtual void Resume() =0;

        void SetObserver(BasicObserver* observer)
        {
           m_pExternObserver = observer;
        }

        BasePipeLine(const BasePipeLine & other) = delete;
        BasePipeLine & operator=(const BasePipeLine & other) = delete;

    protected:
        bool  m_started;
        bool  m_waited;

        BasicObserver               m_observer;
        BasicObserver*              m_pExternObserver;

	};


    //=============================
	//Class SREPipeLine
	//
	//
	//
	//=============================
	class SREPipeLine:public BasePipeLine
	{
    public:
        SREPipeLine(BasicObserver* observer=nullptr):
            BasePipeLine(observer),
            m_IA(1), m_VP(2), m_VPP(3), m_RZ(4),
            m_IAoutputBuffer(),
            m_VPoutputBuffer(),
            m_VPPoutputBuffer(),
            m_zbuffer(),
            m_pRender(nullptr),
            constbuffer(),
            varbuffer()
        {}

        virtual ~SREPipeLine(){}

        RESULT Init(USINT frameWidth, USINT frameHeight, bool createZbuffer) ;
        void Run() ;
        void Pause();
        void Cancel();
        void Resume();

        void HandleElement(){}

        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void HandleMessage(SREVAR message, USINT id=0);

    public:
        void SceneBegin();
        void SceneEnd();
        void SetVertexBufferAndIndexBuffer(VBUFFER* vertexbuffer, IBUFFER* indexbuffer=nullptr);
        void SetVertexShader(VertexShader * vshader);
        void SetClipXValue(FLOAT x_min, FLOAT x_max);
        void SetClipYValue(FLOAT y_min, FLOAT y_max);
        void SetClipZValue(FLOAT z_min, FLOAT z_max);
        void SetClipTolerance(FLOAT tolerance);
        void SetPixelShader(PixelShader* pshader);
        void SetSubProcessors(USINT num);
        void SetSamplePixelBlockSize(USINT size);
        void SetSampleStep(USINT step);
        void SetRenderTarget(RenderTexture* target);
        RESULT ResetFrameSize(USINT width, USINT height);
        void SetObserver(BasicObserver* observer);
        void ResetZbuffer(FLOAT maxDepth);

    protected:
        InputAssembler              m_IA;
        VertexProcessor             m_VP;
        VertexPostProcessor      m_VPP;
        Rasterizer                      m_RZ;

    protected:
        PIOBUFFER m_IAoutputBuffer;
        PIOBUFFER m_VPoutputBuffer;
        PIOBUFFER m_VPPoutputBuffer;

    protected:
        ZBUFFER                          m_zbuffer;
        RenderTexture*              m_pRender;

    public:
        ConstantBuffer                 constbuffer;
        VariableBuffer                   varbuffer;



	};






}

#endif // SRE_PipeLine_H_INCLUDED
