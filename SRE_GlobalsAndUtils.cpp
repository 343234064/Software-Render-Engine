//*****************************************************
//
// Software Render Engine
// Version 0.01
//
// File: SRE_GlobalsAndUtils.cpp
// Date: 2016/4/28
// Description:
//       Implements of functions which declared
//       in SRE_GlobalsAndUtils.h
//
//
//*****************************************************
#include "SRE_GlobalsAndUtils.h"
#include "SRE_Math.h"


namespace SRE {
   	//==============================
    //functions
    //
    //==============================
	void OutputAsImage(DECOLOR* colorbuffer, INT width, INT height)
	{
		std::ofstream ofs;
		ofs.open("./outputImage.ppm");
		ofs << "P6\n" << width<< " " << height << "\n255\n";
		ofs.write((char*)colorbuffer, width * height * sizeof(DECOLOR));
		ofs.close();
	}

	DECOLOR* RenderTargetToDeviceBuffer(DECOLOR* dest, const RTCOLOR* renderTarget, INT size)
	{
		if(nullptr == dest || nullptr == renderTarget) return nullptr;
		INT i=0;
		while(i<size)
		{
			dest[i] = renderTarget[i];
			i++;
		}
		return dest;
	}


}
