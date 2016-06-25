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
        
        void enable() {
            enabled = true;
        }
        
        void disable() {
            enabled = false;
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
        
        void addTriggerToEnable(Trigger * p_trigger) {
            triggersToEnable.insert(p_trigger);
        }
        
        void addTriggerToDisable(Trigger * p_trigger) {
            triggersToDisable.insert(p_trigger);
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
