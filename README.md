NetDraw
=======

A simple paint program where everyone draws on the same board!


Server
------

The server program must be running in order for clients to connect to it and be able to draw together.

#### Configuration file: server.cfg

* Width and height: the board size
* Port: which port clients should connect to


Client
------

This is the graphical program that you can use to draw on. It will connect to the server specified in the configuration file.

#### Controls

* Left click: Paint with current tool
* Right click: Erase with black
* Mouse wheel scroll: Change tool size
* Ctrl + scroll: Change colors
* Shift + scroll: Change alpha level (transparency)
* C: Clears the entire board
* N: Go to next color (hold for rainbow effect)
* S: Toggle showing your mouse pointer

#### Configuration file: client.cfg

* Colors: the list of available colors to choose from
* Server: address + port of server to connect to
* Username: an optional username to show next to your cursor
