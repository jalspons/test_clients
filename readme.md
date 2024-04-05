# Test TCP clients

TCP communications for collecting data from signal sources. Data is reported on STDOUT in JSON format.

## Requirements

Two programs:
- `Client1` - collect data from connections and prints results to STDOUT in JSON format
- `Client2` - similarly as with client1, collect data and print to STDOUT in JSON format. As extension, establish a control connection to send commands to control system.  

Data is collected via TCP sockets periodically. TCP socket connections are kept connected, so that socket buffers can collect data over the sampling periods. Sockets should operate on non-blocking mode, so sockets without data are not blocking data collection.

Data collection period should be defined in milliseconds.

Collected data is reported to STDOUT using JSON format for further analysis.

## Implementation

As programs operate similarly in data collection, reusable module could be used. To mitigate differences of the programs, source parameters should be configurable. Also, the data collection could be designed as its own module, that could be run on a separate thread in the future, if more functionality, like remote management, is added to programs.

As programs are only intended for collecting data and, possibly, reacting to data values, single thread operation is sufficient enough.

Data collection itself should happen in periods. Data is collected from sources using TCP sockets and data is read periodically from the sockets. Sockets should be connected all the time to allow sampling over the period. If sockets have errors or source disconnects socket, programs should try to reconnect socket. Sockets should be operated in non-blocking mode to avoid blocking reads, if there is no data in the socket buffer.

Data is reported on STDOUT in JSON format. JSON is commonly used format and has large support in multiple languages. In this case, it is enough to form JSON string by just building directly stringified JSON. Stringified JSON could be sent directly as text data into different services, if needed. 


### Client1:

Create a system only collecting data from multiple connections periodically and printing the values to stdout.

## Details of data sources:

**Output 1**
- Sine wave
- Frequency: 0.5Hz
- Peak-to-Peak amplitude: 10 [-5, 5]

**Output 2**
- Triangle wave
- Frequency: 0.25Hz
- Peak-to-peak Amplitude: 10 [-5, 5]

**Output 3**
- Digital signal
- HIGH level: 5
- LOW level: 0
- Clock: 1Hz
