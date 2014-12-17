#ifndef PACKETS_H_INCLUDED
#define PACKETS_H_INCLUDED

// All of the different packets that can be sent
namespace Packets
{
    enum Types
    {
        Unknown = 0,
        ToolUpdate,
        ToolId,
        DeleteTool,
        BoardUpdate,
        ClearBoard,
        NumTypes
    };
}

/*
ToolUpdate:
    Client/tool ID, Tool fields, ...
    There can be more than 1 tool update per packet.
    Clients are allowed to send this
ToolId:
    Server tells a client what its tool ID is
DeleteTool:
    Server tells all clients that a client left
BoardUpdate:
    Has the resolution of the board
    Contains the entire board texture
    Only sent initially from the server
ClearBoard:
    Clears the board with black
    Clients are allowed to send this
*/

#endif
