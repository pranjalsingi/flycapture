//=============================================================================
// Copyright © 2009 Point Grey Research, Inc. All Rights Reserved.
//
// This software is the confidential and proprietary information of
// Point Grey Research, Inc. ("Confidential Information"). You shall not
// disclose such Confidential Information and shall use it only in
// accordance with the terms of the "License Agreement" that you
// entered into with PGR in connection with this software.
//
// UNLESS OTHERWISE SET OUT IN THE LICENSE AGREEMENT, THIS SOFTWARE IS
// PROVIDED ON AN “AS-IS” BASIS AND POINT GREY RESEARCH INC. MAKES NO
// REPRESENTATIONS OR WARRANTIES ABOUT THE SOFTWARE, EITHER EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTIES OR
// CONDITIONS OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, OR
// NON-INFRINGEMENT. POINT GREY RESEARCH INC. SHALL NOT BE LIABLE FOR ANY
// DAMAGES, INCLUDING BUT NOT LIMITED TO ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, PUNITIVE, OR CONSEQUENTIAL DAMAGES, OR ANY LOSS OF PROFITS,
// REVENUE, DATA OR DATA USE, ARISING OUT OF OR IN CONNECTION WITH THIS
// SOFTWARE OR OTHERWISE SUFFERED BY YOU AS A RESULT OF USING, MODIFYING
// OR DISTRIBUTING THIS SOFTWARE OR ITS DERIVATIVES.
//=============================================================================
//=============================================================================
// $Id: MultipleCameraEx.cpp 301446 2016-10-06 23:46:49Z erich $
//=============================================================================

#include "stdafx.h"

#include "FlyCapture2.h"
#include <iostream>
#include <sstream>

using namespace FlyCapture2;
using namespace std;

void PrintBuildInfo()
{
    FC2Version fc2Version;
    Utilities::GetLibraryVersion(&fc2Version);

    ostringstream version;
    version << "FlyCapture2 library version: " << fc2Version.major << "."
            << fc2Version.minor << "." << fc2Version.type << "."
            << fc2Version.build;
    cout << version.str() << endl;

    ostringstream timeStamp;
    timeStamp << "Application build date: " << __DATE__ << " " << __TIME__;
    cout << timeStamp.str() << endl << endl;
}

void PrintCameraInfo(CameraInfo *pCamInfo)
{
    cout << endl;
    cout << "*** CAMERA INFORMATION ***" << endl;
    cout << "Serial number - " << pCamInfo->serialNumber << endl;
    cout << "Camera model - " << pCamInfo->modelName << endl;
    cout << "Camera vendor - " << pCamInfo->vendorName << endl;
    cout << "Sensor - " << pCamInfo->sensorInfo << endl;
    cout << "Resolution - " << pCamInfo->sensorResolution << endl;
    cout << "Firmware version - " << pCamInfo->firmwareVersion << endl;
    cout << "Firmware build time - " << pCamInfo->firmwareBuildTime << endl
         << endl;
}

void PrintError(Error error) { error.PrintErrorTrace(); }

int main(int /*argc*/, char ** /*argv*/)
{
    PrintBuildInfo();

    const int k_numImages = 50;
    Error error;

    //
    // Initialize BusManager and retrieve number of cameras detected
    //
    BusManager busMgr;
    unsigned int numCameras;
    error = busMgr.GetNumOfCameras(&numCameras);
    if (error != PGRERROR_OK)
    {
        PrintError(error);
        return -1;
    }

    cout << "Number of cameras detected: " << numCameras << endl;

    //
    // Check to make sure at least two cameras are connected before
    // running example
    //
    if (numCameras < 2)
    {
        cout << "Insufficient number of cameras." << endl;
        cout << "Make sure at least two cameras are connected for example to "
                "run."
             << endl;
        cout << "Press Enter to exit." << endl;
        cin.ignore();
        return -1;
    }

    //
    // Initialize an array of cameras
    //
    // *** NOTES ***
    // The size of the array is equal to the number of cameras detected.
    // The array of cameras will be used for connecting, configuring,
    // and capturing images.
    //
    Camera *pCameras = new Camera[numCameras];

    //
    // Prepare each camera to acquire images
    //
    // *** NOTES ***
    // For pseudo-simultaneous streaming, each camera is prepared as if it
    // were just one, but in a loop. Notice that cameras are selected with
    // an index. We demonstrate pseduo-simultaneous streaming because true
    // simultaneous streaming would require multiple process or threads,
    // which is too complex for an example.
    //
    for (unsigned int i = 0; i < numCameras; i++)
    {
        PGRGuid guid;
        error = busMgr.GetCameraFromIndex(i, &guid);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            cout << "Press Enter to exit." << endl;
            delete[] pCameras;
            cin.ignore();
            return -1;
        }

        // Connect to a camera
        error = pCameras[i].Connect(&guid);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            delete[] pCameras;
            cout << "Press Enter to exit." << endl;
            cin.ignore();
            return -1;
        }

        // Get the camera information
        CameraInfo camInfo;
        error = pCameras[i].GetCameraInfo(&camInfo);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            delete[] pCameras;
            cout << "Press Enter to exit." << endl;
            cin.ignore();
            return -1;
        }

        PrintCameraInfo(&camInfo);

        // Turn trigger mode off
        TriggerMode trigMode;
        trigMode.onOff = false;
        error = pCameras[i].SetTriggerMode(&trigMode);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            delete[] pCameras;
            cout << "Press Enter to exit." << endl;
            cin.ignore();
            return -1;
        }

        // Turn Timestamp on
        EmbeddedImageInfo imageInfo;
        imageInfo.timestamp.onOff = true;
        error = pCameras[i].SetEmbeddedImageInfo(&imageInfo);
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            delete[] pCameras;
            cout << "Press Enter to exit." << endl;
            cin.ignore();
            return -1;
        }

        // Start streaming on camera
        error = pCameras[i].StartCapture();
        if (error != PGRERROR_OK)
        {
            PrintError(error);
            delete[] pCameras;
            cout << "Press Enter to exit." << endl;
            cin.ignore();
            return -1;
        }
    }

    //
    // Retrieve images from all cameras
    //
    // *** NOTES ***
    // In order to work with simultaneous camera streams, nested loops are
    // needed. It is important that the inner loop be the one iterating
    // through the cameras; otherwise, all images will be grabbed from a
    // single camera before grabbing any images from another.
    //
    for (int j = 0; j < k_numImages; j++)
    {
        for (unsigned int i = 0; i < numCameras; i++)
        {
            Image image;
            error = pCameras[i].RetrieveBuffer(&image);
            if (error != PGRERROR_OK)
            {
                PrintError(error);
                delete[] pCameras;
                cout << "Press Enter to exit." << endl;
                cin.ignore();
                return -1;
            }

            // Display the timestamps of the images grabbed for each camera
            TimeStamp timestamp = image.GetTimeStamp();
            cout << "Camera " << i << " - Frame " << j << " - TimeStamp ["
                 << timestamp.cycleSeconds << " " << timestamp.cycleCount << "]"
                 << endl;
        }
    }

    //
    // Stop streaming for each camera
    //
    for (unsigned int i = 0; i < numCameras; i++)
    {
        pCameras[i].StopCapture();
        pCameras[i].Disconnect();
    }

    delete[] pCameras;

    cout << "Press Enter to exit..." << endl;
    cin.ignore();

    return 0;
}