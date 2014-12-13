// Copyright (C) 2014 Eric Hebert (ayebear)
// This code is licensed under GPLv3, see LICENSE.txt for details.

#ifndef NETWORKUPDATER_H
#define NETWORKUPDATER_H

// All of the different packets that can be sent
namespace Packets
{
    enum Types
    {
        Unknown = 0,
        ToolUpdate,
        BoardUpdate,
        ToolId,
        NumTypes
    };
}

/*
ToolUpdate:
    Client/tool ID, Tool fields, ...
    There can be more than 1 tool update per packet.
    This is the only packet type the client is allowed to send out.
BoardUpdate:
    Contains the entire board texture
    Only sent initially from the server
        Or maybe only once in a while
ToolId:
    Server tells a client what its tool is
*/

/*
This class handles all of the networking.
    Making a client (and sometimes a server)
    Finding a server and connecting to it
    Sending/receiving packets
*/
class NetworkUpdater
{
    public:
        NetworkUpdater();
        virtual ~NetworkUpdater();
        void update();

    private:
};

#endif
