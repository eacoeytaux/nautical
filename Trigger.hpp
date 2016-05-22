//
//  Trigger.hpp
//  Nautical
//
//  Created by Ethan Coeytaux on 5/2/16.
//  Copyright © 2016 EthanCo. All rights reserved.
//

#ifndef Trigger_hpp
#define Trigger_hpp

#include "Tagable.hpp"

#include "LinkedList.hpp"

namespace nautical {
    const std::string TRIGGER_TAG = "trigger";
    
    class Trigger : public Tagable {
    public:
        Trigger(bool enabled = true, bool autoDisable = true) :
        enabled(enabled),
        autoDisable(autoDisable) {
            appendTag(TRIGGER_TAG);
        }
        
        virtual ~Trigger();
        
        int getActivatedCount() const { return activatedCount; }
        bool isEnabled() const { return enabled; }
        
        Trigger & enable() {
            enabled = true;
            return *this;
        }
        
        Trigger & disable() {
            enabled = false;
            return *this;
        }
        
        bool activate() {
            if (!enabled) {
                return false;
            } else if (!trigger()) {
                return false;
            } else if (autoDisable) {
                disable();
            }
            
            activatedCount++;
            
            for (Iterator<Trigger*> * iterator = triggersToEnable.createIterator(); !iterator->complete(); iterator->next()) {
                iterator->current()->enable();
            }
            
            for (Iterator<Trigger*> * iterator = triggersToDisable.createIterator(); !iterator->complete(); iterator->next()) {
                iterator->current()->disable();
            }
            
            return true;
        };
        
        Trigger & addTriggerToEnable(Trigger * p_trigger) {
            triggersToEnable.insert(p_trigger);
            return *this;
        }
        
        Trigger & addTriggerToDisable(Trigger * p_trigger) {
            triggersToDisable.insert(p_trigger);
            return *this;
        }
        
    protected:
        virtual bool trigger() = 0;
        
    private:
        int activatedCount = 0;
        bool enabled, autoDisable;
        LinkedList<Trigger*> triggersToEnable;
        LinkedList<Trigger*> triggersToDisable;
    };
}

#endif /* Trigger_hpp */
