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
            m_queue.push(std::make_shared(data));
            m_cond.notify_one();
        }

        void push(T* pdata)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(std::shared_ptr<T>(pdata));
            m_cond.notify_one();
        }

        void push(std::shared_ptr<T> pdata)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(pdata);
            m_cond.notify_one();
        }

        std::shared_ptr<T> wait_and_pop()
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_cond.wait(lock, [this]{return !m_queue.empty();});
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
            m_cond.wait(lock, [this]{return !m_queue.empty();});
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

        BasicIOBuffer(const BasicIOBuffer & other) = delete;
        BasicIOBuffer & operator=(const BasicIOBuffer & other) = delete;

    protected:
        std::queue<std::shared_ptr<T>>
                                         m_queue;
        std::mutex                       m_mutex;
        std::condition_variable
                                         m_cond;


	};


    //=============================
	//Class BasicObserver
	//
	//
	//=============================
	class BasicObserver
	{
    public:
        BasicObserver(){}
        virtual ~BasicObserver(){}

        void Notify(SREVAR message)
        {
           HandleMessage(message);
        }

    protected:
        virtual void HandleMessage(SREVAR message)=0;

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



    //=============================
	//Class BasicProcessor
	//
	//
	//=============================
	class BasicProcessor:public BaseTask
	{
    public:
        BasicProcessor(BasicIOBuffer<BasicIOElement> * input=nullptr,
                       BasicIOBuffer<BasicIOElement> * output=nullptr,
                       BasicObserver     * observer=nullptr,
                       CallBackFunctions * callbacks=nullptr):
            BaseTask(),
            m_pInputQueue(input),
            m_pOutputQueue(output),
            m_pObserver(observer),
            m_thread(),
            m_callBacks(callbacks),
            m_mutex(),
            m_cond(),
            m_resMutex(),
            m_Cancel(false),
            m_Pause(false),
            m_Started(false)
        {}
        virtual ~BasicProcessor()
        {}


        void Start();
        void Run();
        void Pause();
        void Resume();
        void Cancel();

        std::shared_ptr<BasicIOElement> GetInput();
        void            Output(std::shared_ptr<BasicIOElement> out);
        void            Output(BasicIOElement* out);

        void SetInputQueue(BasicIOBuffer<BasicIOElement> * inputQueue)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_pInputQueue = inputQueue;
        }
        void SetOutputQueue(BasicIOBuffer<BasicIOElement> * outpueQueue)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_pOutputQueue = outpueQueue;
        }
        void SetObserver(BasicObserver  * observer)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_pObserver = observer;
        }


        void Input(BasicIOElement* element)
        {
            m_pInputQueue->push(element);
        }


        void SetCallBacks(CallBackFunctions * callbacks)
        {
            m_callBacks = callbacks;
        }

        void Join();

        BasicProcessor(const BasicProcessor & other) = delete;
        BasicProcessor & operator=(const BasicProcessor & other) = delete;

    private:
        BasicIOBuffer<BasicIOElement> *     m_pInputQueue;
        BasicIOBuffer<BasicIOElement> *     m_pOutputQueue;
        BasicObserver  *                    m_pObserver;
        std::thread                         m_thread;
        CallBackFunctions   *               m_callBacks;
        std::mutex                          m_mutex;

    protected:
        std::condition_variable             m_cond;
        std::mutex                          m_resMutex;

        bool m_Cancel;
        bool m_Pause;
        bool m_Started;



	};




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
        _pixelBlock_(USINT _x=0, USINT _y=0, std::shared_ptr<T> _data=nullptr):
            x(_x),
            y(_y),
            tempData(_data)
        {}
        _pixelBlock_(const _pixelBlock_<T> & other):
            x(other.x),
            y(other.y),
            tempData(other.tempData)
        {}
        virtual ~_pixelBlock_()
        {}

    public:
	    USINT x;
	    USINT y;
	    std::shared_ptr<T> tempData;
	};


    //=============================
	//Class InputAssembler
	//
	//
	//
	//=============================
	class InputAssembler:public BasicProcessor, public CallBackFunctions
	{
    public:
        InputAssembler(BasicIOBuffer<BasicIOElement> * output=nullptr,
                       BasicObserver *                 observer=nullptr,
                       const ConstantBuffer *          constbuffer=nullptr):
            BasicProcessor(nullptr, output, observer, this),
            m_vertexBuffers(),
            m_indexBuffers(),
            m_pConstantBuffer(constbuffer),
            m_pCurrentHandleVbuffer(nullptr),
            m_pCurrentHandleIbuffer(nullptr),
            m_currentIndex(0),
            m_currentIndex2(0)
        {}
        virtual ~InputAssembler()
        {}


        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        InputAssembler(const InputAssembler & other) = delete;
        InputAssembler & operator=(const InputAssembler & other) = delete;

        void SetVertexAndIndexBuffers(VertexBuffer* vertexbuffer, Buffer<INT>* indexBuffer);
        void SetConstantBuffer(const ConstantBuffer * cbuffer);

    private:
        void SendVertex(INT & actualIndex);

    private:
        std::queue<VertexBuffer*>     m_vertexBuffers;
        std::queue<Buffer<INT>*>      m_indexBuffers;
        const ConstantBuffer *        m_pConstantBuffer;

        VertexBuffer*                 m_pCurrentHandleVbuffer;
        Buffer<INT>*                  m_pCurrentHandleIbuffer;

        INT                           m_currentIndex;
        INT                           m_currentIndex2;

	};



    //=============================
	//Class VertexProcessor
	//
	//
	//
	//=============================
    class VertexProcessor:public BasicProcessor, public CallBackFunctions
    {
    public:
        VertexProcessor(BasicIOBuffer<BasicIOElement> * input=nullptr,
                        BasicIOBuffer<BasicIOElement> * output=nullptr,
                        BasicObserver * observer=nullptr,
                        VertexShader*   vshader=nullptr,
                        VariableBuffer* variablebuffer=nullptr):
            BasicProcessor(input, output, observer, this),
            m_pVertexShader(vshader),
            m_pVariableBuffer(variablebuffer)
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
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void SetVertexShader(VertexShader * vshader);
        void SetVariableBuffer(VariableBuffer* varbuffer);

        VertexProcessor(const VertexProcessor & other) = delete;
        VertexProcessor & operator=(const VertexProcessor & other) = delete;

    private:
        VertexShader*     m_pVertexShader;
        VariableBuffer*   m_pVariableBuffer;

        VSOutput          m_cachedVertex[3];
        USINT             m_cachedVertexIndex[3];
        USINT             m_cachedPriority[3];
    };



    //=============================
	//Class PrimitiveAssembler
	//
	//
	//
	//=============================
    class PrimitiveAssembler:public BasicProcessor, public CallBackFunctions
    {
    public:
        PrimitiveAssembler(BasicIOBuffer<BasicIOElement> * input=nullptr,
                        BasicIOBuffer<BasicIOElement>    * output=nullptr,
                        BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer, this)
        {}
        virtual ~PrimitiveAssembler()
        {}

        void HandleElement();
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
    class VertexPostProcessor:public BasicProcessor, public CallBackFunctions
    {
    public:
        VertexPostProcessor(BasicIOBuffer<BasicIOElement> * input=nullptr,
                            BasicIOBuffer<BasicIOElement> * output=nullptr,
                            BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer, this),
            m_pCurrentTriangle(nullptr),
            m_clipEpsilon(0.00),
            m_viewportHeightHalf(300),
            m_viewportWidthHalf(400)
        {
            m_clipPlaneDistance[0]= 1;m_clipPlaneNormal[0].x= 1;
            m_clipPlaneDistance[1]= 1;m_clipPlaneNormal[1].x=-1;
            m_clipPlaneDistance[2]= 1;m_clipPlaneNormal[2].y= 1;
            m_clipPlaneDistance[3]= 1;m_clipPlaneNormal[3].y=-1;
            m_clipPlaneDistance[4]= 0;m_clipPlaneNormal[4].z= 1;
            m_clipPlaneDistance[5]= 1;m_clipPlaneNormal[5].z=-1;
        }
        virtual ~VertexPostProcessor()
        {
            if(nullptr != m_pCurrentTriangle)
                delete m_pCurrentTriangle;
        }

        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void SetClipPlaneX(FLOAT Distance_PlaneToOrigin, VEC3& normal);
        void SetClipPlaneY(FLOAT Distance_PlaneToOrigin, VEC3& normal);
        void SetClipPlaneZ(FLOAT Distance_PlaneToOrigin, VEC3& normal);
        void SetClipTolerance(FLOAT tolerance){m_clipEpsilon = tolerance;}
        void SetViewportHeight(USINT height)
        {
            if(height>0) m_viewportHeightHalf = height/2;
        }
        void SetViewportWidth(USINT width)
        {
            if(width>0) m_viewportWidthHalf = width/2;
        }


        VertexPostProcessor(const VertexPostProcessor & other) = delete;
        VertexPostProcessor & operator=(const VertexPostProcessor & other) = delete;

    protected:
        void TriangleClipping();                //clip in cvv
        void OtherTranforms(VSOutput & input);  //then divided by w
                                                //transform x to range:(0,width)
                                                //transform y to range:(0,height)
                                                //origin (0,0) is on left-top corner
                                                //(width, height) is on right-bottom corner
        void SendTriangle(VSOutput & v1, VSOutput & v2, VSOutput & v3);

    protected:
        _Triangle_*  m_pCurrentTriangle;
        FLOAT        m_clipPlaneDistance[6];//-x, x, -y, y, -z, z
        VEC3         m_clipPlaneNormal[6];
        FLOAT        m_clipEpsilon;
        USINT        m_viewportHeightHalf;
        USINT        m_viewportWidthHalf;//data type should be considered

    private:
        CLList<_vertex_>    m_vlist[2];

    };



    //=============================
	//Class Rasterizer
	//
	//
	//
	//=============================
    class Rasterizer:public BasicProcessor, public CallBackFunctions
    {
    public:
        Rasterizer(BasicIOBuffer<BasicIOElement> * input=nullptr,
                   BasicIOBuffer<BasicIOElement> * output=nullptr,
                   BasicObserver * observer=nullptr,
                   const ConstantBuffer * cbuffer=nullptr,
                   USINT samplePixelBlockSize=16):
            BasicProcessor(input, output, observer, this),
            m_pConstantBuffer(cbuffer),
            m_pCurrentTriangle(nullptr),
            m_subProcessors(),
            m_subIndex(0),
            m_perPixelBlockSize(samplePixelBlockSize)
        {}
        virtual ~Rasterizer()
        {
            if(nullptr != m_pCurrentTriangle)
                delete m_pCurrentTriangle;
        }

        void HandleElement();
        void OnCancel();
        void OnPause();
        void OnResume();
        void OnRunError();
        void OnRunFinish();
        void OnStart();

        void AddSubProcessor(USINT num=4, USINT sampleStep=1);
        void RemoveSubProcessor(USINT num);
        void SetConstantBuffer(const ConstantBuffer * cbuffer);
        void SetSamplePixelBlockSize(USINT blockSize);
        void SetSampleStep(USINT sampleStep);
        void CancelSubProcessor();

        Rasterizer(const Rasterizer & other) = delete;
        Rasterizer & operator=(const Rasterizer & other) = delete;

    protected:
        bool BackFaceCulling();//Using cross product(CCW, CW mode will give different result) to calculate face normal
                               //and then test the face normal with the eye ray
                               //if BackFaceCulling return true, we send this triangle to rasterizer

    protected:
        const ConstantBuffer *      m_pConstantBuffer;
        _Triangle_ *                m_pCurrentTriangle;

        std::vector<SubRasterizer>  m_subProcessors;

        USINT                       m_subIndex;
        USINT                       m_perPixelBlockSize;


    };



    //=============================
	//Class SubRasterizer
	//
	//Default run on 16x16 pixel block
	//=============================
	class SubRasterizer
	{
    public:
        SubRasterizer(USINT sampleStep=0):
            m_inputQueue(),
            m_thread(),
            m_cond(),
            m_mutex(),
            m_cancel(false),
            m_started(false),
            m_spTriangle(),
            m_sampleStep(sampleStep)
        {}
        SubRasterizer(const SubRasterizer & other):
            m_inputQueue(other.m_inputQueue),
            m_thread(),
            m_cond(),
            m_mutex(),
            m_cancel(other.m_cancel),
            m_started(other.m_started),
            m_spTriangle(other.m_spTriangle),
            m_sampleStep(other.m_sampleStep)
        {}
        virtual ~SubRasterizer()
        {}

        void Start();
        void Cancel();

        void SetSampleStep(USINT sampleStep){m_sampleStep = sampleStep;}
        void PushTask(_pixelBlock_<BasicIOElement> & task);

        SubRasterizer & operator=(const SubRasterizer & other) = delete;
//////////////////////////////////////////
        USINT                        m_num;
//////////////////////////////////////////
    protected:
        void ScanConversion();
        void GetTask(USINT & x, USINT & y);

    protected:
        std::queue<_pixelBlock_<BasicIOElement>>
                                     m_inputQueue;
        std::thread                  m_thread;
        std::condition_variable      m_cond;
        std::mutex                   m_mutex;

        bool                         m_cancel;
        bool                         m_started;

        std::shared_ptr<BasicIOElement>
                                     m_spTriangle;
        USINT                        m_sampleStep;


	};



    /*
    //=============================
	//Class PixelProcessor
	//
	//
	//
	//=============================
    class PixelProcessor:public BasicProcessor
    {
    public:
        PixelProcessor(BasicIOBuffer<BasicIOElement> * input=nullptr,
                       BasicIOBuffer<BasicIOElement> * output=nullptr,
                       BasicObserver * observer=nullptr):
             BasicProcessor(input, output, observer)
        {}
        virtual ~PixelProcessor();

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);
    };


    //=============================
	//Class OutputMerger
	//
	//
	//
	//=============================
    class OutputMerger:public BasicProcessor
    {
    public:
        OutputMerger(BasicIOBuffer<BasicIOElement> * input=nullptr,
                     BasicIOBuffer<BasicIOElement> * output=nullptr,
                     BasicObserver * observer=nullptr):
            BasicProcessor(input, output, observer)
        {}
        virtual ~OutputMerger();

        void Run();
        void NextStage();
        void PassArgument(SREVAR usage, void * argu);

    };

    */



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
