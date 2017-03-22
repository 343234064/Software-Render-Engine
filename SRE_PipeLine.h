//*****************************************************
//
// Software Render Engine
// Version 0.01
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
    //=============================
	//Class Basic I/O Buffer
	//
	//
	//=============================
	template<typename T>
	class BasicIOBuffer
	{
    public:
        BasicIOBuffer():
            m_queue(),
            m_mutex(),
            m_cond()
        {}
        virtual ~BasicIOBuffer()
        {}

        void push(T data)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(std::make_shared(data));
            m_cond.notify_one();
        }

        void push(T* pdata)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(std::shared_ptr<T>(pdata));
            m_cond.notify_one();
        }

        void push(std::shared_ptr<T> pdata)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stopwait = false;
            m_queue.push(pdata);
            m_cond.notify_one();
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty()||m_stopwait;});
            if(m_stopwait) {return std::shared_ptr<T>();}

            std::shared_ptr<T> res = m_queue.front();
            m_queue.pop();

            return res;
        }

        std::shared_ptr<T> top()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            std::shared_ptr<T> res = m_queue.front();
            return res;
        }

        void wait_and_pop(T ** out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty()||m_stopwait;});
            if(m_stopwait) {*out=nullptr;return;}

            std::shared_ptr<T> p = m_queue.front();
            *out = p.get();
            m_queue.pop();
        }

        void top(T ** out)
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            std::shared_ptr<T> p = m_queue.front();
            *out = p.get();
        }

        bool empty() const
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        void StopWait()
        {
        	std::lock_guard<std::mutex> lock(m_mutex);
        	m_stopwait = true;
        	m_cond.notify_all();
        }

        BasicIOBuffer(const BasicIOBuffer & other) = delete;
        BasicIOBuffer & operator=(const BasicIOBuffer & other) = delete;

    protected:
        std::queue<std::shared_ptr<T>>
                                         m_queue;
        std::mutex                m_mutex;
        std::condition_variable
                                         m_cond;
		bool                         m_stopwait;



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
							  BasicIOBuffer<BasicIOElement> * input=nullptr,
                              BasicIOBuffer<BasicIOElement> * output=nullptr,
                              BasicObserver     * observer=nullptr,
                              CallBackFunctions * callbacks=nullptr):
            BaseTask(),
            m_pInputQueue(input),
            m_pOutputQueue(output),
            m_thread(),
            m_callBacks(callbacks),
            m_mutex(),
            m_id(id),
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
        inline void Output(std::shared_ptr<BasicIOElement> out);
        inline void Output(BasicIOElement* out);
        inline void SetInputQueue(BasicIOBuffer<BasicIOElement> * inputQueue);
        inline void SetOutputQueue(BasicIOBuffer<BasicIOElement> * outpueQueue);
        inline void SetObserver(BasicObserver  * observer);
        inline void Input(BasicIOElement* element);
        inline void SetCallBacks(CallBackFunctions * callbacks);
        inline void Join();

        BasicProcessor(const BasicProcessor & other) = delete;
        BasicProcessor & operator=(const BasicProcessor & other) = delete;

    private:
        BasicIOBuffer<BasicIOElement> *     m_pInputQueue;
        BasicIOBuffer<BasicIOElement> *     m_pOutputQueue;
        std::thread                                     m_thread;
        CallBackFunctions   *                       m_callBacks;
        std::mutex                                     m_mutex;

    protected:
    	USINT                                             m_id;
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

    void BasicProcessor::SetInputQueue(BasicIOBuffer<BasicIOElement> * inputQueue)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_pInputQueue = inputQueue;
    }

    void BasicProcessor::SetOutputQueue(BasicIOBuffer<BasicIOElement> * outpueQueue)
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

    void BasicProcessor::Output(std::shared_ptr<BasicIOElement> out)
    {
        m_pOutputQueue->push(out);
    }

    void BasicProcessor::Output(BasicIOElement* out)
    {
        m_pOutputQueue->push(out);
    }

    void BasicProcessor::Join()
    {
        if(m_thread.joinable())
           m_thread.join();

    }


    //=============================
	//Class BasePipeLine
	//
	//
	//
	//=============================
	/*
	template<typename ProcessorType, typename IOBufferType>
	class BasePipeLine
	{
    public:
        BasePipeLine()
        {}
        virtual ~BasePipeLine(){}


        virtual void BuildPipeLine()=0;
        virtual void AddProcessor(INT index, BasicProcessor * processor)=0;
        virtual void RemoveProcessor(INT index)=0;
        virtual void AddProcessorBack(BasicProcessor * processor)=0;
        virtual void RemoveProcessorBack()=0;
        virtual void OnInit()=0;
        virtual void OnEnd()=0;
        virtual void OnPause()=0;
        virtual void OnResume()=0;


        void Run();
        void Pause();
        void Cancel();
        void Resume();


        BasePipeLine(const BasePipeLine & other) = delete;
        BasePipeLine & operator=(const BasePipeLine & other) = delete;


    protected:
        BasicObserver               m_processorObserver;
        std::list<IOBufferType>
                                    m_IOBufferArray;
        std::list<ProcessorType>    m_ProcessorArray;
        //the type should be considered again

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
        SREPipeLine():
            BasePipeLine()
        {}

        virtual ~SREPipeLine(){}

        void     BuildPipeLine();
        void     AddProcessor(INT index, BasicProcessor * processor);
        void     RemoveProcessor(INT index);


    protected:
        //RunTimeData     m_runTimeData;

	};
	*/



    //=============================
	//Temp structures for IO
	//=============================
	class _index_ver_
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
           delete vertex;
       }

       _index_ver_(const _index_ver_ & other) = delete;
       _index_ver_ & operator=(const _index_ver_ & other) = delete;
	};

    struct _vertex_
	{//A temp data structure used for clipping
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
        _Triangle_(VSOutput & v1, VSOutput & v2, VSOutput & v3)
        {
            v[0]=v1;v[1]=v2;v[2]=v3;
        }
        virtual ~_Triangle_()
        {
        }

    public:
        VSOutput  v[3];
	};

	template<typename T>
	class _pixelBlock_:public BasicIOElement
	{
    public:
        _pixelBlock_(USINT _sx=0, USINT _sy=0, USINT _distX=0, USINT _distY=0, bool _isEnd=false,
                     std::shared_ptr<T> _data=nullptr):
            sx(_sx), sy(_sy), distX(_distX), distY(_distY),
            isEnd(_isEnd),
            tempData(_data)
        {}
        _pixelBlock_(const _pixelBlock_<T> & other):
            sx(other.sx), sy(other.sy),
            distX(other.distX), distY(other.distY),
            isEnd(other.isEnd),
            tempData(other.tempData)
        {}
        virtual ~_pixelBlock_()
        {}

    public:
	    USINT sx, sy;
	    USINT distX, distY;
	    bool  isEnd;
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
					            BasicIOBuffer<BasicIOElement> * output=nullptr,
                                BasicObserver *                 observer=nullptr):
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

        void SetVertexAndIndexBuffers(VertexBuffer* vertexbuffer, Buffer<INT>* indexBuffer=nullptr);
        inline void SetConstantBuffer(const ConstantBuffer * cbuffer);
        inline void BeginSceneSetting();
        inline void EndSceneSetting();

        InputAssembler(const InputAssembler & other) = delete;
        InputAssembler & operator=(const InputAssembler & other) = delete;

    private:
        void SendVertex(INT & actualIndex);

    private:
        std::queue<VertexBuffer*>      m_vertexBuffers;
        std::queue<Buffer<INT>*>      m_indexBuffers;
        const ConstantBuffer *              m_pConstantBuffer;

        VertexBuffer*                  m_pCurrentHandleVbuffer;
        Buffer<INT>*                  m_pCurrentHandleIbuffer;

        INT                                 m_currentIndex;
        INT                                 m_currentIndex2;

        bool                              m_valve;

	};

    void InputAssembler::SetConstantBuffer(const ConstantBuffer * cbuffer)
    {
        if(nullptr != cbuffer)
           m_pConstantBuffer = cbuffer;
    }

    void InputAssembler::BeginSceneSetting()
    {
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
						         BasicIOBuffer<BasicIOElement> * input=nullptr,
                                 BasicIOBuffer<BasicIOElement> * output=nullptr,
                                 BasicObserver * observer=nullptr):
            BasicProcessor(id, input, output, observer, this),
            m_pVertexShader(nullptr),
            m_pVariableBuffer(nullptr),
			m_pCVertexShader(nullptr),
            m_pCVariableBuffer(nullptr)
        {
            m_cachedVertexIndex[0]=-1;
            m_cachedVertexIndex[1]=-1;
            m_cachedVertexIndex[2]=-1;
            m_cachedPriority[0]=4;
            m_cachedPriority[1]=2;
            m_cachedPriority[2]=1;
        }
        virtual ~VertexProcessor()
        {
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

    protected:
        VertexShader*   m_pVertexShader;
        VariableBuffer*  m_pVariableBuffer;
        VertexShader*   m_pCVertexShader;
        VariableBuffer*  m_pCVariableBuffer;


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
	//
	//
	//
	//=============================
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
							            BasicIOBuffer<BasicIOElement> * input=nullptr,
                                        BasicIOBuffer<BasicIOElement> * output=nullptr,
                                        BasicObserver * observer=nullptr):
            BasicProcessor(id, input, output, observer, this),
            m_pCurrentTriangle(nullptr),
            m_clipEpsilon(0.00),
            m_viewportHeightHalf(300),
            m_viewportWidthHalf(400)
        {
            m_clipPlaneDistance[0]= 1;m_clipPlaneNormal[0].x=  1;
            m_clipPlaneDistance[1]= 1;m_clipPlaneNormal[1].x=-1;
            m_clipPlaneDistance[2]= 1;m_clipPlaneNormal[2].y=  1;
            m_clipPlaneDistance[3]= 1;m_clipPlaneNormal[3].y=-1;
            m_clipPlaneDistance[4]= 0;m_clipPlaneNormal[4].z=  1;
            m_clipPlaneDistance[5]= 1;m_clipPlaneNormal[5].z=-1;
        }
        virtual ~VertexPostProcessor()
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

        inline void SetClipPlaneX(FLOAT Distance_PlaneToOrigin, VEC3& normal);
        inline void SetClipPlaneY(FLOAT Distance_PlaneToOrigin, VEC3& normal);
        inline void SetClipPlaneZ(FLOAT Distance_PlaneToOrigin, VEC3& normal);
        inline void SetClipTolerance(FLOAT tolerance);
        inline void SetViewportHeight(USINT height);
        inline void SetViewportWidth(USINT width);

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
        _Triangle_*  m_pCurrentTriangle;
        FLOAT         m_clipPlaneDistance[6];//-x, x, -y, y, -z, z
        VEC3           m_clipPlaneNormal[6];
        FLOAT         m_clipEpsilon;
        USINT          m_viewportHeightHalf;
        USINT          m_viewportWidthHalf;//data type should be considered

    private:
        CLList<_vertex_>    m_vlist[2];

    };

    void VertexPostProcessor::SetClipTolerance(FLOAT tolerance)
    {
        m_clipEpsilon = tolerance;
    }

    void VertexPostProcessor::SetViewportHeight(USINT height)
    {
        if(height>0) m_viewportHeightHalf = height/2;
    }

    void VertexPostProcessor::SetViewportWidth(USINT width)
    {
        if(width>0) m_viewportWidthHalf = width/2;
    }

    void VertexPostProcessor::SetClipPlaneX(FLOAT Distance_PlaneToOrigin, VEC3& normal)
    {
        m_clipPlaneDistance[0] = Distance_PlaneToOrigin;
        m_clipPlaneDistance[1] = Distance_PlaneToOrigin;

        m_clipPlaneNormal[0] = normal;
        m_clipPlaneNormal[1] = -normal;
    }

    void VertexPostProcessor::SetClipPlaneY(FLOAT Distance_PlaneToOrigin, VEC3& normal)
    {
        m_clipPlaneDistance[2] = Distance_PlaneToOrigin;
        m_clipPlaneDistance[3] = Distance_PlaneToOrigin;

        m_clipPlaneNormal[2] = normal;
        m_clipPlaneNormal[3] = -normal;
    }

    void VertexPostProcessor::SetClipPlaneZ(FLOAT Distance_PlaneToOrigin, VEC3& normal)
    {
        m_clipPlaneDistance[4] = Distance_PlaneToOrigin;
        m_clipPlaneDistance[5] = Distance_PlaneToOrigin;

        m_clipPlaneNormal[4] = normal;
        m_clipPlaneNormal[5] = -normal;
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
						BasicIOBuffer<BasicIOElement> * input=nullptr,
                        BasicIOBuffer<BasicIOElement> * output=nullptr,
                        BasicObserver * observer=nullptr,
                        USINT samplePixelBlockSize=16):
            BasicProcessor(id, input, output, observer, this),
            m_subObserver(this),
            m_pConstantBuffer(nullptr),
            m_pCurrentTriangle(nullptr),
            m_subProcessors(),
            m_subIndex(0),
            m_perPixelBlockSize(samplePixelBlockSize),
            m_finishCount(0),
            m_sampleStep(1),
            m_pZbuffer(nullptr),
            m_pRenderTarget(nullptr),
            m_pPixelShader(nullptr)
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
        inline void SetZbuffer(SynMatBuffer<FLOAT>* zbuffer);
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

        USINT                       m_subIndex;
        USINT                       m_perPixelBlockSize;
        USINT                       m_finishCount;

        USINT                                  m_sampleStep;
        SynMatBuffer<FLOAT>*        m_pZbuffer;
        RenderTexture*                   m_pRenderTarget;
        PixelShader *                       m_pPixelShader;


    };

    INT Rasterizer::GetSubProcessorNum()
    {
    	return m_subProcessors.size();
    }

	void Rasterizer::SetZbuffer(SynMatBuffer<FLOAT>* zbuffer)
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
	    if(!m_Started)
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
                              const ConstantBuffer* cbuffer=nullptr,
							  SynMatBuffer<FLOAT>* zbuffer=nullptr,
                              RenderTexture* renderTarget=nullptr,
                              PixelShader* pshader=nullptr):
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
            m_pConstantBuffer(cbuffer),
            m_pZbuffer(zbuffer),
            m_pRenderTarget(renderTarget),
            m_pPixelShader(pshader)
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

        inline RenderTexture* GetRenderTarget();
        inline void SetSampleStep(USINT sampleStep);
        inline void SetConstantBuffer(const ConstantBuffer * cbuffer);
        inline void SetZBuffer(SynMatBuffer<FLOAT>* zbuffer);
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
        inline FLOAT EdgeFunction2D(const VEC4& v1, const VEC4& v2, const VEC4& v3);
        inline FLOAT EdgeFunction2D(FLOAT v1x, FLOAT v1y, FLOAT v2x, FLOAT v2y, FLOAT v3x, FLOAT v3y);
        inline bool   ZTest(USINT sx, USINT sy, FLOAT interpolatedZ);

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

        const ConstantBuffer*         m_pConstantBuffer;
        SynMatBuffer<FLOAT>*        m_pZbuffer;
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

    void PixelProcessor::SetZBuffer(SynMatBuffer<FLOAT>* zbuffer)
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

    //magnitude of the cross product:(v2-v1) and (V3-v1).
    FLOAT PixelProcessor::EdgeFunction2D(const VEC4& v1, const VEC4& v2, const VEC4& v3)
    {
        return (v3.x - v1.x) * (v2.y - v1.y) - (v3.y - v1.y) * (v2.x - v1.x);
    }

    FLOAT PixelProcessor::EdgeFunction2D(FLOAT v1x, FLOAT v1y, FLOAT v2x, FLOAT v2y, FLOAT v3x, FLOAT v3y)
    {
        return (v3x - v1x) * (v2y - v1y) - (v3y - v1y) * (v2x - v1x);
    }

	bool PixelProcessor::ZTest(USINT sx, USINT sy, FLOAT interpolatedZ)
	{
        if(interpolatedZ < m_pZbuffer->GetData(sx, sy))
        {
            m_pZbuffer->SetData(sx, sy, interpolatedZ);
            return true;
        }
        else
        {
            return false;
        }
	}



    //=============================
	//Class OutputMerger
	//
	//
	//
	//=============================
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

        inline void SetFrameBuffer(BYTE* frameBuffer);
        inline void SetConstantBuffer(const ConstantBuffer*  cbuffer);
        inline void SetOutputToFrameBuffer(bool toFrame);

        OutputMerger(const OutputMerger & other) = delete;
        OutputMerger & operator=(const OutputMerger & other) = delete;

    protected:
    	const ConstantBuffer*  m_pConstantBuffer;
        BYTE*                           m_pFrame;
        RenderTexture*            m_pCurTarget;

        bool                            m_toFrame;


    };


    void OutputMerger::SetFrameBuffer(BYTE* frameBuffer)
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
        VEC2   ViewportSize;
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
            ZEnable(SRE_TRUE)
        {}
        virtual ~ConstantBuffer(){}

    public:
        SREVAR primitiveTopology;
        SREVAR FillMode;
        SREVAR CullEnable;
        SREVAR CullMode;
        SREVAR ZEnable;
	};





}

#endif // SRE_PipeLine_H_INCLUDED
