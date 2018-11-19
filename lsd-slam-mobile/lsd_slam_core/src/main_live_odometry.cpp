/**
* This file is part of LSD-SLAM.
*
* Copyright 2013 Jakob Engel <engelj at in dot tum dot de> (Technical University of Munich)
* For more information see <http://vision.in.tum.de/lsdslam> 
*
* LSD-SLAM is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* LSD-SLAM is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with LSD-SLAM. If not, see <http://www.gnu.org/licenses/>.
*/

#include "LiveSLAMWrapper.h"

#include <boost/thread.hpp>
#include "util/settings.h"
#include "util/globalFuncs.h"
#include "SlamSystem.h"


#include "IOWrapper/OpenCVIO/OpenCVImageStreamThread.h"
#include "IOWrapper/OpenCVIO/opencvReconfigure.h"
#include "IOWrapper/NetworkOutput/NetworkOutput3DWrapper.h"

//#include <X11/Xlib.h>

using namespace lsd_slam;
int main( int argc, char** argv )
{
    //XInitThreads();
    
    char* pathToStream = argv[1];    
	//ros::init(argc, argv, "LSD_SLAM");

	//dynamic_reconfigure::Server<lsd_slam_core::LSDParamsConfig> srv(ros::NodeHandle("~"));
	//srv.setCallback(dynConfCb);

	//dynamic_reconfigure::Server<lsd_slam_core::LSDDebugParamsConfig> srvDebug(ros::NodeHandle("~Debug"));
	//srvDebug.setCallback(dynConfCbDebug);

    ConfigMain();
	packagePath = ".";// ros::package::getPath("lsd_slam_core")+"/";

	InputImageStream* inputStream = new OpenCVImageStreamThread(pathToStream);

	std::string calibFile(argv[2]);
	/*if(ros::param::get("~calib", calibFile))
	{
		ros::param::del("~calib");
		inputStream->setCalibration(calibFile);
	}
	else*/

    inputStream->setCalibration(calibFile);
	inputStream->run();
    printf("Staring the outpud thread!\n"); fflush(stdout);
	Output3DWrapper* outputWrapper =new NetworkOutput3DWrapper(inputStream->width(), inputStream->height(), "127.0.0.1","6660");
	//Output3DWrapper* outputWrapper =new OpenCVOutput3DWrapper(inputStream->width(), inputStream->height());
	//Output3DWrapper* outputWrapper = NULL;
    printf("Staring the slamNode!\n"); fflush(stdout);
	LiveSLAMWrapper slamNode(inputStream, outputWrapper);
	slamNode.Loop();

	if (inputStream != nullptr)
		delete inputStream;
	if (outputWrapper != nullptr)
		delete outputWrapper;

	return 0;
}
