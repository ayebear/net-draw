#ifndef PACKETS_H_INCLUDED
#define PACKETS_H_INCLUDED

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
        Also, clients can only update their own tool!
BoardUpdate:
    Contains the entire board texture
    Only sent initially from the server
        Or maybe only once in a while
ToolId:
    Server tells a client what its tool is
*/

#endif
