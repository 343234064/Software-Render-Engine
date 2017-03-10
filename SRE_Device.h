//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_Device.h
// Date: 2017/03/06
// Description:
//       Defines all Device and platform relative classes and functions
//
//
//
//
//*****************************************************
#ifndef SRE_DEVICE_H_INCLUDED
#define SRE_DEVICE_H_INCLUDED

#include "SRE_GlobalsAndUtils.h"

namespace SRE {

    //display image must consider platform problem
    //=============================
	//Class Device
	//
	//
	//=============================
	class Device
	{
    public:
        Device();
        virtual ~Device();

    public:
        USINT  m_winWidth;
        USINT  m_winHeight;


	};

}
#endif // SRE_DEVICE_H_INCLUDED
