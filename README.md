# Project Name

## Description

This project aims to create a client-server application with the following specifications:

1. The client and the server shall not depend on a specific communication protocol and shall be able to communicate using TCP/IP and Serial/CAN protocols.
2. The maximum speed supported is 240 km/h.
3. Temperature is in the range of -60°C to 60°C.
4. Battery level is represented in percentage.

## Development and Collaboration

1. Git and GitHub are used for collaboration and version control.
2. Followed the GitHub Flow strategy.
3. Organized the project using GitHub Projects, using the Scrum methodology with a shared Scrum master, weekly sprints, sprint reviews, and sprint planning on Thursdays.
4. Used Slack channel for communication, linked to the repository.
5. Used the Visual Studio C++ style guide.

## User Interfaces

### Server GUI

- Includes sliders for speed, temperature, and battery level.
- Three checkboxes for light signals, with logic to enable/disable them based on selection.
- Error messages displayed in case of communication issues.
- Automatic reconnection on the client side.

### Client GUI

- Simple dashboard to display speed, temperature, battery level, light signals, and communication status.
- Icons with color changes based on temperature and battery level.
- Blinking light symbols and sound for selected light signals.
- Blinking lights when the warning checkbox on the server is checked.

## Communication Abstraction

- Communication between client and server is abstracted using an abstract base class to make the applications independent of a specific protocol.
- Supported communication protocols include TCP/IP and Serial/CAN 2.0B.
- The abstract class on the server provides buffering and functionalities to insert speed, temperature, battery level, light signal states, send the buffer over the chosen protocol, and get communication status.
- The abstract class on the client provides buffering and functionalities to extract data and get communication status.

## Implementation Details

- Used multithreading for communication protocols to protect shared resources.
- Error handling includes printing error messages on the server terminal and displaying error icons and messages on the client.
- Temperature and battery level color-coding is implemented.
- Communication of changes in server-side data to the client is supported.
- CMakeLists.txt file is included in the project root for building both the client and server programs.
- PlatformIO projects are provided for uploading to microcontrollers.

## Usage

To switch between communication protocols:

1. Define a variable and based on its value, define a macro.
2. In the main.cpp files of the client and server, check if the macro has been defined, then create and use an instance of the chosen   communication protocol implementation class.
3. Build both the client and server programs.
4. Upload the PlatformIO projects to the microcontrollers.