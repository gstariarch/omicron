/******************************************************************************
 * THE OMICRON PROJECT
 *-----------------------------------------------------------------------------
 * Copyright 2010-2014		Electronic Visualization Laboratory, 
 *							University of Illinois at Chicago
 * Authors:										
 *  Arthur Nishimoto	    anishimoto42@gmail.com
 *  Alessandro Febretti		febret@gmail.com
 *-----------------------------------------------------------------------------
 * Copyright (c) 2010-2014, Electronic Visualization Laboratory,  
 * University of Illinois at Chicago
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this 
 * list of conditions and the following disclaimer. Redistributions in binary 
 * form must reproduce the above copyright notice, this list of conditions and 
 * the following disclaimer in the documentation and/or other materials provided 
 * with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO THE 
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE  GOODS OR 
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *-----------------------------------------------------------------------------
 * What's in this file:
 *  A service that reads motion capture data through the VRPN protocol.
 ******************************************************************************/
#ifndef __VRPN__SERVICE_H__
#define __VRPN__SERVICE_H__

#include "omicron/osystem.h"
#include "omicron/ServiceManager.h"

#include <stdlib.h>
#include <stdio.h>
#include <vrpn_tracker.h> // Remember to set Linker directories and Input/Dependencies to vrpn.lib
#include <vrpn_Button.h>

namespace omicron
{
struct VRPNStruct;
///////////////////////////////////////////////////////////////////////////////
class OMICRON_API VRPNService: public Service
{
public:
    // Allocator function
    static VRPNService* New() { return new VRPNService(); }

public:
    void setup(Setting& settings);
    virtual void initialize();
    virtual void poll();
    virtual void dispose();

    void generateTrackerEvent(vrpn_TRACKERCB, int, unsigned short userId, int jointId);
	void generateButtonEvent(VRPNStruct*, int, int buttonID, int stateFlag);

    //! Sets the data update interval, in seconds. This is the interval at which this service will generate events
    //! If set to zero, the service will generate events as fast as possible.
    void setUpdateInterval(float value);
    //! @see setUpdateInterval
    float getUpdateInterval();

private:
    static VRPNService* mysInstance;
    
    const char* server_ip; // Tracker IP for all objects (unless marked otherwise)

    struct TrackerInfo
    {
        const char* server_ip;
        const char* object_name;
		const char* object_type;
        int trackableId;
        unsigned short userId;
        int jointId;
    };

    Vector<TrackerInfo> trackerNames; // Vector of the TrackerInfo struct
    Vector<vrpn_Tracker_Remote*> trackerRemotes; // Vector of actual vrpn tracker remote objects

    float myUpdateInterval;
};
///////////////////////////////////////////////////////////////////////////////
struct VRPNStruct
{
    const char* server_ip;
    const char* object_name;
	const char* object_type;
    int object_id;
    unsigned short userId;
    int jointId;
    VRPNService* vrnpService;
};
///////////////////////////////////////////////////////////////////////////////
inline void VRPNService::setUpdateInterval(float value) 
{ myUpdateInterval = value; }

///////////////////////////////////////////////////////////////////////////////
inline float VRPNService::getUpdateInterval() 
{ return myUpdateInterval; }
}; // namespace omicron

#endif