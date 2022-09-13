#ifndef _LABEL
#define _LABEL

#include "ofMain.h"

class label{

    public:
    label();
    ~label();

    void setup(float xpos, float ypos, float width, float height, std::string symbol);
    void update();
    void draw();

    float xpos;
    float ypos;
    float width;
    float height;
    std::string symbol;
};
#endif
