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
            CullMode(SRE_CULLMODE_CW),
            ClipEnable(SRE_TRUE),
            ZEnable(SRE_TRUE),
            AlphaTest(SRE_FALSE),
            AlphaBlend(SRE_FALSE)
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
		 virtual void HandleMessage(SREVAR message, USINT id=0, void* data=nullptr)=0;
    };



    //=============================
	//Class BasicObserver
	//
	//
	//=============================
	class BasicObserver
	{
    public:
        BasicObserver(ObserverCallBack* callback):
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
             m_callback->HandleMessage(message, id);
        }

        void Notify(SREVAR message, USINT id, void* data)
        {
             m_callback->HandleMessage(message, id, data);
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
	//Class BasicStage
	//
	//
	//=============================
	class BasicStage:public BaseTask
	{
   public:
        BasicStage(USINT id=0,
                         BasicStage* nextstage=nullptr,
                         BasicObserver* observer=nullptr):
             m_id(id),
             m_pNextStage(nextstage),
             m_pObserver(observer)
         {}
        virtual ~BasicStage(){}

        virtual void Reset()=0;
        virtual void SetInput(void* data, INT index)=0;

        void SetNextStage(BasicStage* next)
        {
           if(nullptr != next)
              m_pNextStage = next;
        }

        void SetObserver(BasicObserver* observer)
        {
           if(nullptr != observer)
            m_pObserver = observer;
        }

        BasicStage(const BasicStage & other) = delete;
        BasicStage & operator=(const BasicStage & other) = delete;

   protected:
        USINT              m_id;
        BasicStage*      m_pNextStage;
        BasicObserver* m_pObserver;
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
        m_pObserver->Notify(10, m_id);
        std::shared_ptr<BasicIOElement> out = m_pInputQueue->wait_and_pop();
        m_pObserver->Notify(9, m_id);
        return out;
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
   class _vertex_
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
        _Triangle_(VSOutput * v1=nullptr, VSOutput * v2=nullptr, VSOutput * v3=nullptr)
        {
           if(nullptr != v1)
                v[0]=*v1;
           if(nullptr != v2)
                v[1]=*v2;
           if(nullptr != v3)
                v[2]=*v3;
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
        _pixelBlock_(USINT _sx=0, USINT _sy=0, USINT _distX=0, USINT _distY=0, FLOAT _area=0.0f, bool _isEnd=false,
                     std::shared_ptr<T> _data=nullptr):
            sx(_sx), sy(_sy), distX(_distX), distY(_distY),
            area(_area),
            isEnd(_isEnd),
            tempData(_data)
        {}
        _pixelBlock_(const _pixelBlock_<T> & other):
            sx(other.sx), sy(other.sy),
            distX(other.distX), distY(other.distY),
            area(other.area),
            isEnd(other.isEnd),
            tempData(other.tempData)
        {}
        ~_pixelBlock_()
        {}

    public:
	    USINT   sx, sy;
	    USINT   distX, distY;
	    FLOAT  area;
	    bool    isEnd;
	    std::shared_ptr<T> tempData;
	};


    //=============================
	//Class InputAssembler
	//
	//
	//
	//=============================
	class InputAssembler:public BasicStage
	{
    public:
        InputAssembler(USINT id=0,
                               BasicStage* nextstage=nullptr,
                               BasicObserver *  observer=nullptr):
            BasicStage(id, nextstage, observer),
            m_vertexBuffers(),
            m_indexBuffers(),
            m_pConstantBuffer(nullptr),
            m_pCurrentHandleVbuffer(nullptr),
            m_pCurrentHandleIbuffer(nullptr),
            m_currentIndex(0),
            m_currentIndex2(0)
        {}
        virtual ~InputAssembler()
        {}

        void Run();
        void Reset();
        void SetInput(void* data, INT index);
        void ClearVertexbufferAndIndexbuffer();
        inline void SetVertexAndIndexBuffers(VBUFFER* vertexbuffer, IBUFFER* indexbuffer=nullptr);
        inline void SetConstantBuffer(const ConstantBuffer * cbuffer);

        InputAssembler(const InputAssembler & other) = delete;
        InputAssembler & operator=(const InputAssembler & other) = delete;

    private:
        void SendVertex(INT actualIndex1, INT actualIndex2, INT actualIndex3);

    private:
        std::queue<VBUFFER*>       m_vertexBuffers;
        std::queue<IBUFFER*>        m_indexBuffers;
        const ConstantBuffer *        m_pConstantBuffer;

        VBUFFER*                             m_pCurrentHandleVbuffer;
        IBUFFER*                              m_pCurrentHandleIbuffer;

        INT                                 m_currentIndex;
        INT                                 m_currentIndex2;

	};

    void InputAssembler::SetConstantBuffer(const ConstantBuffer * cbuffer)
    {
        if(nullptr != cbuffer)
           m_pConstantBuffer = cbuffer;
    }

    void InputAssembler::SetVertexAndIndexBuffers(VBUFFER* vertexbuffer, IBUFFER* indexbuffer)
    {
        if(nullptr == vertexbuffer) return;
        m_vertexBuffers.push(vertexbuffer);
        m_indexBuffers.push(indexbuffer);

    }
    //=============================
	//Class VertexProcessor
	//
	//
	//
	//=============================
    class VertexProcessor:public BasicStage
    {
    public:
        VertexProcessor(USINT id=0,
                                 BasicStage* nextstage=nullptr,
                                 BasicObserver * observer=nullptr):
            BasicStage(id, nextstage, observer),
            m_pVertexShader(nullptr),
            m_pVariableBuffer(nullptr),
            m_curinput(0)
			{
            m_cachedVertexIndex[0]=-1;
            m_cachedVertexIndex[1]=-1;
            m_cachedVertexIndex[2]=-1;
            m_cachedPriority[0]=4;
            m_cachedPriority[1]=2;
            m_cachedPriority[2]=1;
            m_inputvertex[0]=nullptr;
            m_inputvertex[1]=nullptr;
            m_inputvertex[2]=nullptr;
            m_inputvertexid[0]=-1;
            m_inputvertexid[1]=-1;
            m_inputvertexid[2]=-1;
        }
        virtual ~VertexProcessor()
        {
        }


        void Run();
        void Reset();
        void SetInput(void* data, INT index);
        inline void SetVertexShader(VertexShader * vshader);
        inline void SetVariableBuffer(VariableBuffer* varbuffer);

        VertexProcessor(const VertexProcessor & other) = delete;
        VertexProcessor & operator=(const VertexProcessor & other) = delete;

	protected:
      void   SetOutputVertex(VSOutput* out);
      VSOutput* SendToVS(BYTE* vertex, USINT index);

    protected:
        VertexShader*   m_pVertexShader;
        VariableBuffer*  m_pVariableBuffer;

        VSOutput          m_cachedVertex[3];
        USINT               m_cachedVertexIndex[3];
        USINT               m_cachedPriority[3];

        BYTE*               m_inputvertex[3];
        SINT                 m_inputvertexid[3];
        SINT                 m_curinput;
    };

    void VertexProcessor::SetVertexShader(VertexShader * vshader)
    {
        if(nullptr != vshader)
        m_pVertexShader = vshader;
    }

    void VertexProcessor::SetVariableBuffer(VariableBuffer* varbuffer)
    {
        if(nullptr != varbuffer)
        m_pVariableBuffer = varbuffer;
    }



   //=============================
	//Class VertexPostProcessor
	//
	//=============================
    class VertexPostProcessor:public BasicStage
    {
    public:
        VertexPostProcessor(USINT id=0,
                                       BasicStage* nextstage=nullptr,
                                       BasicObserver * observer=nullptr):
            BasicStage(id, nextstage, observer),
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
        {

        }

        void Run();
        void Reset();
        void SetInput(void* data, INT index);

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
       const ConstantBuffer*   m_pConstantBuffer;
        _Triangle_*                   m_pCurrentTriangle;
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
        m_clipPlaneDistance[0] = -_min;
        m_clipPlaneDistance[1] =   _max;
    }

    void VertexPostProcessor::SetClipYValue(FLOAT _min, FLOAT _max)
    {
        m_clipPlaneDistance[2] = -_min;
        m_clipPlaneDistance[3] =   _max;
    }

    void VertexPostProcessor::SetClipZValue(FLOAT _min, FLOAT _max)
    {
        m_clipPlaneDistance[4] = -_min;
        m_clipPlaneDistance[5] =   _max;
    }

	void VertexPostProcessor::SetConstantBuffer(const ConstantBuffer * cbuffer)
	{
	    if(nullptr != cbuffer)
        m_pConstantBuffer = cbuffer;
	}


   //=============================
	//Class GeometryProcessor
	//
	//
	//
	//=============================
   class GeometryProcessor:public BasicProcessor, public CallBackFunctions, public ObserverCallBack
   {
   public:
      GeometryProcessor(USINT id=0,
						                  PIOBUFFER * output=nullptr,
                                    BasicObserver * observer=nullptr):
            BasicProcessor(id, nullptr, output, observer, this),
            m_subObserver(this),
            m_ia(1, nullptr, &m_subObserver),
            m_vp(2, nullptr, &m_subObserver),
            m_vpp(3, nullptr, &m_subObserver),
            m_valve(false)
      {
         m_ia.SetNextStage(&m_vp);
         m_vp.SetNextStage(&m_vpp);
      }
      virtual ~GeometryProcessor(){}

      void HandleElement();
      void HandleMessage(SREVAR message, USINT id=0, void* data=nullptr);
      void OnCancel();
      void OnPause();
      void OnResume();
      void OnRunError();
      void OnRunFinish();
      void OnStart();

      void BeginScene();
      void EndScene();
      inline void SetVariableBuffer(VariableBuffer* varbuffer);
      inline void SetConstantBuffer(const ConstantBuffer* conbuffer);
      inline void SetVertexBufferAndIndexBuffer(VBUFFER* vertexbuffer, IBUFFER* indexbuffer=nullptr);
      inline void SetVertexShader(VertexShader * vshader);
      inline void SetClipXValue(FLOAT x_min, FLOAT x_max);
      inline void SetClipYValue(FLOAT y_min, FLOAT y_max);
      inline void SetClipZValue(FLOAT z_min, FLOAT z_max);
      inline void SetClipTolerance(FLOAT tolerance);
      inline void SetViewportHeight(USINT height);
      inline void SetViewportWidth(USINT width);

      GeometryProcessor(const GeometryProcessor & other) = delete;
      GeometryProcessor & operator=(const GeometryProcessor & other) = delete;

   protected:
      BasicObserver            m_subObserver;

   protected:
      InputAssembler          m_ia;
      VertexProcessor        m_vp;
      VertexPostProcessor  m_vpp;

   protected:
      bool                         m_valve;

   };

   void GeometryProcessor::SetVariableBuffer(VariableBuffer* varbuffer)
   {
      m_vp.SetVariableBuffer(varbuffer);
   }

   void GeometryProcessor::SetConstantBuffer(const ConstantBuffer* conbuffer)
   {
      m_ia.SetConstantBuffer(conbuffer);
      m_vpp.SetConstantBuffer(conbuffer);
   }

   void GeometryProcessor::SetVertexBufferAndIndexBuffer(VBUFFER* vertexbuffer, IBUFFER* indexbuffer)
   {
      m_ia.SetVertexAndIndexBuffers(vertexbuffer, indexbuffer);
   }

   void GeometryProcessor::SetVertexShader(VertexShader * vshader)
   {
      m_vp.SetVertexShader(vshader);
   }

   void GeometryProcessor::SetClipXValue(FLOAT x_min, FLOAT x_max)
   {
      m_vpp.SetClipXValue(x_min, x_max);
   }

   void GeometryProcessor::SetClipYValue(FLOAT y_min, FLOAT y_max)
   {
      m_vpp.SetClipYValue(y_min, y_max);
   }

   void GeometryProcessor::SetClipZValue(FLOAT z_min, FLOAT z_max)
   {
      m_vpp.SetClipZValue(z_min, z_max);
   }

   void GeometryProcessor::SetClipTolerance(FLOAT tolerance)
   {
      m_vpp.SetClipTolerance(tolerance);
   }

   void GeometryProcessor::SetViewportHeight(USINT height)
   {
      m_vpp.SetViewportHeight(height);
   }

   void GeometryProcessor::SetViewportWidth(USINT width)
   {
      m_vpp.SetViewportWidth(width);
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
            m_pPixelShader(nullptr)
        {}
        virtual ~Rasterizer()
        {
           if(nullptr != m_pCurrentTriangle)
             delete m_pCurrentTriangle;
        }

        void HandleElement();
        void HandleMessage(SREVAR message, USINT id=0, void* data=nullptr);
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

        void  SetProcessorArgus(USINT id);
        void  SetAllProcessorArgus();

        Rasterizer(const Rasterizer & other) = delete;
        Rasterizer & operator=(const Rasterizer & other) = delete;

    protected:
        bool BackFaceCulling();//Using cross product(CCW, CW mode will give different result) to calculate face normal
                                           //and then test the face normal with the eye ray
                                           //if BackFaceCulling return true, we send this triangle to rasterizer

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
                              USINT sampleStep=0):
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

        std::shared_ptr<BasicIOElement> GetTask(FLOAT& triArea);
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
            m_GP(1), m_RZ(2),
            m_GPoutputBuffer(),
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

        void HandleMessage(SREVAR message, USINT id=0, void* data=nullptr);

        SREPipeLine(const SREPipeLine & other) = delete;
        SREPipeLine & operator=(const SREPipeLine & other) = delete;

    public:
        void BeginScene();
        void EndScene();
        inline void SetVertexBufferAndIndexBuffer(VBUFFER* vertexbuffer, IBUFFER* indexbuffer=nullptr);
        inline void SetVertexShader(VertexShader * vshader);
        inline void SetClipXValue(FLOAT x_min, FLOAT x_max);
        inline void SetClipYValue(FLOAT y_min, FLOAT y_max);
        inline void SetClipZValue(FLOAT z_min, FLOAT z_max);
        inline void SetClipTolerance(FLOAT tolerance);
        inline void SetPixelShader(PixelShader* pshader);
        inline void SetSamplePixelBlockSize(USINT size);
        inline void SetSampleStep(USINT step);
        inline void SetRenderTarget(RenderTexture* target);
        inline void SetObserver(BasicObserver* observer);
        void SetSubProcessors(USINT num);
        RESULT ResetFrameSize(USINT width, USINT height);
        void ResetZbuffer(FLOAT maxDepth);

    protected:
        GeometryProcessor       m_GP;
        Rasterizer                      m_RZ;

    protected:
        PIOBUFFER m_GPoutputBuffer;

    protected:
        ZBUFFER                          m_zbuffer;
        RenderTexture*              m_pRender;

    public:
        ConstantBuffer                 constbuffer;
        VariableBuffer                   varbuffer;



	};



   void SREPipeLine::SetVertexBufferAndIndexBuffer(VBUFFER* vertexbuffer, IBUFFER* indexbuffer)
   {
      m_GP.SetVertexBufferAndIndexBuffer(vertexbuffer, indexbuffer);
   }

   void SREPipeLine::SetVertexShader(VertexShader * vshader)
   {
      m_GP.SetVertexShader(vshader);
   }

   void SREPipeLine::SetClipXValue(FLOAT x_min, FLOAT x_max)
   {
      if(m_started) return;
      m_GP.SetClipXValue(x_min, x_max);
   }

   void SREPipeLine::SetClipYValue(FLOAT y_min, FLOAT y_max)
   {
      if(m_started) return;
      m_GP.SetClipYValue(y_min, y_max);
   }

   void SREPipeLine::SetClipZValue(FLOAT z_min, FLOAT z_max)
   {
      if(m_started) return;
      m_GP.SetClipZValue(z_min, z_max);
   }

   void SREPipeLine::SetClipTolerance(FLOAT tolerance)
   {
      if(m_started) return;
      m_GP.SetClipTolerance(tolerance);
   }

   void SREPipeLine::SetPixelShader(PixelShader* pshader)
   {
      m_RZ.SetPixelShader(pshader);
   }

   void SREPipeLine::SetSamplePixelBlockSize(USINT size)
   {
      m_RZ.SetSamplePixelBlockSize(size);
   }

   void SREPipeLine::SetSampleStep(USINT step)
   {
      m_RZ.SetSampleStep(step);
   }

   void SREPipeLine::SetRenderTarget(RenderTexture* target)
   {
      m_RZ.SetRenderTarget(target);
   }

   void SREPipeLine::SetObserver(BasicObserver* observer)
   {
      m_pExternObserver = observer;
   }



}

#endif // SRE_PipeLine_H_INCLUDED
