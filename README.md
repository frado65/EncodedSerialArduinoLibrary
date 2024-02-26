# EncodedSerialArduinoLibrary
This library is highly useful for those needing to transmit binary data in serial mode, covering protocols such as UART/RS232, I2C/IIC, SPI, CAN, or any other protocol you may require. Its primary function is to encapsulate your message with a start byte and an end byte, ensuring you can decode your message with confidence, knowing it's fully intact. To achieve this, it utilizes three special bytes: a start byte, a stop byte, and an escape byte. Messages typically begin with a start byte and conclude with a length byte followed by the stop byte. If any byte within the message coincides with the start byte, stop byte, or escape byte, an escape byte is inserted before it to maintain the integrity of the data.
It has been developed to ensure compatibility with both C++ 11 and the Arduino IDE, offering the advantage of allowing you to test it on your PC before implementing it in an Arduino-compatible project.
You can see how to use it in the 'LibTester.h' file, where I simulate the send and receive process.
If you find it useful, I would appreciate it if you could notify me.
In the future, I plan to implement a data integrity technique to enhance the reliability of the system."

In the project, you can also find other useful features, for example, an interface for error management that allows you to display error messages with different hardware, such as LEDs, Serial, None, or std::cout if you are using C++/g++. You can switch from one to another by changing a "#define", or create your own (e.g., a display), or combine several options at once using the CErrorWrapper class.

It's a simple code, but it can be useful.

Every suggestion is welcome.
