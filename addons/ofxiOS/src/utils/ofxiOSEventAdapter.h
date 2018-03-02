//
//  ofxiOSEventAdapter.h
//
//  Created by Tal Lavi on 16/2/2015.
//
//

#pragma once

#import "ofxiOSExtras.h"
#include "ofEvents.h"
#include "ofConstants.h"
#include <map>
#include <list>

class ofEventAdapterBase{
    
protected:
    id m_targetInstance;
    SEL m_targetSelector;
    
public:
    
    ofEventAdapterBase(id targetInstance, SEL targetSelector):
    m_targetInstance(targetInstance),
    m_targetSelector(targetSelector)
    {}
    
    id getTargetInstance(){return m_targetInstance;}
    SEL getTargetSelector(){return m_targetSelector;}
};

template <typename ArgumentsType>
class ofEventAdapter : public ofEventAdapterBase {
    
    ofEvent<ArgumentsType>& m_ofEvent;
    
    
public:
    ofEventAdapter(ofEvent<ArgumentsType>& ofEvent, id targetInstance, SEL targetSelector):
    ofEventAdapterBase(targetInstance, targetSelector),
    m_ofEvent(ofEvent)
    {
        ofAddListener(m_ofEvent, this, &ofEventAdapter<ArgumentsType>::eventCallback);
    }
    
    ~ofEventAdapter(){
        ofRemoveListener(m_ofEvent, this, &ofEventAdapter::eventCallback);
    }
    
    void eventCallback(ArgumentsType& args){
        if (![m_targetInstance respondsToSelector:m_targetSelector]){
            return; //can't call selector!
        }
        
        [m_targetInstance performSelector:m_targetSelector withObject:(id)(&args)];
    }
};

typedef std::shared_ptr<ofEventAdapterBase> Target;
typedef std::shared_ptr<std::list<Target> > Targets;
typedef std::map<void*, Targets> Events;

Targets getTargets(void* ofEvent);
void removeTarget(void* ofEvent, id targetInstance, SEL targetSelector);

template <typename ArgumentsType>
void ofAddListener(ofEvent<ArgumentsType>& ofEvent, id targetInstance, SEL targetSelector){
    Targets targets = getTargets(&ofEvent);
    
    Target target(new ofEventAdapter<ArgumentsType>(ofEvent, targetInstance, targetSelector));
    
    targets->push_back(target);
}

template <typename ArgumentsType>
void ofRemoveListener(ofEvent<ArgumentsType>& ofEvent, id targetInstance, SEL targetSelector){
    removeTarget(&ofEvent, targetInstance, targetSelector);
}
