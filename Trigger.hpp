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

#include <vector>

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
            
            for (std::vector<Trigger*>::iterator it = triggersToEnable.begin(); it != triggersToEnable.end(); it++) {
                (*it)->enable();
            }
            
            for (std::vector<Trigger*>::iterator it = triggersToDisable.begin(); it != triggersToDisable.end(); it++) {
                (*it)->disable();
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
        std::vector<Trigger*> triggersToEnable;
        std::vector<Trigger*> triggersToDisable;
    };
}

#endif /* Trigger_hpp */
