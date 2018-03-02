//
//  ofxiOSEventAdapter.h
//
//  Created by Tal Lavi on 16/2/2015.
//
//

#include "ofxiOSEventAdapter.h"

Events m_events;

Targets getTargets(void* ofEvent){
    Targets result;
    
    if (m_events.find(ofEvent) != m_events.end()){
        result = m_events[ofEvent];
    }
    else{
        result = Targets(new std::list<Target>());
        m_events.insert(std::pair<void*, Targets>(ofEvent, result));
    }
    
    return result;
}


void removeTarget(void* ofEvent, id targetInstance, SEL targetSelector){
    if (m_events.find(ofEvent) == m_events.end())
        return;
    
    Targets targets = m_events[ofEvent];
    
    std::list<Target>::iterator iter;
    
    for(iter = targets->begin(); iter != targets->end(); ++iter){
        
        if (iter->get()->getTargetInstance() == targetInstance &&
            iter->get()->getTargetSelector() == targetSelector)
        {
            iter = targets->erase(iter);
        }
    }
    
    if (targets->empty())
    {
        m_events.erase(ofEvent);
    }
}