#include "ofMain.h"
#include "ofEvents.h"

class eventsObject{
    public:
        eventsObject();
        void enable();
        void disable();
        void update(ofEventArgs & args);

        ofEvent<float> newFloatEvent;
        ofEvent<int> newIntEvent;

    private:
        float counter;
};
