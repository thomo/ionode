#ifndef _max7300_h_
#define _max7300_h_

class Max7300 {
  public:
    typedef enum
    {
        MAX7300_PORT_OUTPUT = 1,
        MAX7300_PORT_INPUT,
        MAX7300_PORT_INPUT_PULLUP
    }PortType_t;

    Max7300();
};

#endif