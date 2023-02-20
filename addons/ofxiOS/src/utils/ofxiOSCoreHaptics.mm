// created by artificiel 2023-02-15

#include "ofxiOSCoreHaptics.h"

bool ofxiOSCoreHaptics::prepare_engine() {
    
    // this starts the engine and primes it with an instance of CHHapticAdvancedPatternPlayer
    // that will later be modulated by CHHapticDynamicParameters
    
    NSError * error;

    NSLog(@"Preparing Haptics Engine");
    auto init_intensity = [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticIntensity value:1.0f];
    auto init_sharpness = [[CHHapticEventParameter alloc] initWithParameterID:CHHapticEventParameterIDHapticSharpness value:0.0f];
    auto event = [[CHHapticEvent alloc] initWithEventType:CHHapticEventTypeHapticContinuous parameters:@[init_intensity, init_sharpness] relativeTime:CHHapticTimeImmediate duration:10000];
    auto pattern = [[CHHapticPattern alloc] initWithEvents:@[event] parameters:@[] error:&error];
    
    engine_ = [[CHHapticEngine alloc] initAndReturnError:&error];
    if (error != nil) {
        NSLog(@"ofxiOSCoreHaptics::prepare_engine [engine_ initAndReturnError:] %@", error);
        return false;
    }
    [engine_ setStoppedHandler:^(CHHapticEngineStoppedReason reason){
        switch (reason) {
            case CHHapticEngineStoppedReasonAudioSessionInterrupt:
                NSLog(@"ofxiOSCoreHaptics::stoppedHandler() engine stopped: CHHapticEngineStoppedReasonAudioSessionInterrupt");
                break;
            case CHHapticEngineStoppedReasonApplicationSuspended:
                NSLog(@"ofxiOSCoreHaptics::stoppedHandler() engine stopped: CHHapticEngineStoppedReasonApplicationSuspended");
                break;
            case CHHapticEngineStoppedReasonIdleTimeout:
                NSLog(@"ofxiOSCoreHaptics::stoppedHandler() engine stopped: CHHapticEngineStoppedReasonIdleTimeout");
                break;
            case CHHapticEngineStoppedReasonSystemError:
                NSLog(@"ofxiOSCoreHaptics::stoppedHandler() engine stopped: CHHapticEngineStoppedReasonSystemError");
                break;
        }
    }];
    
    player_ = [engine_ createAdvancedPlayerWithPattern:pattern error:&error];
    if (error != nil) {
        NSLog(@"ofxiOSCoreHaptics::prepare_engine() %@ [engine_ createAdvancedPlayerWithPattern:]", error);
        return false;
    }
    
    [engine_ startAndReturnError:&error];
    if (error != nil) {
        NSLog(@"ofxiOSCoreHaptics::prepare_engine [engine_ startAndReturnError:] %@", error);
        return false;
    }
    
    [player_ startAtTime:CHHapticTimeImmediate error:&error];
    if (error != nil) {
        NSLog(@"ofxiOSCoreHaptics::prepare_engine [player_ startAtTime:] %@", error);
        return false;
    }
    [player_ setLoopEnabled:TRUE];
    return true;
}

void ofxiOSCoreHaptics::setup() {
    is_vibrating_ = prepare_engine();
    if (!is_vibrating_) NSLog(@"ofxiOSCoreHaptics::setup() failed");
}

bool ofxiOSCoreHaptics::is_vibrating() {
    if (!is_vibrating_) setup();
    return is_vibrating_;
}

bool ofxiOSCoreHaptics::sendParameters(float i, float s) {
    
    if (is_vibrating()) {
        
        NSError * error;
        auto intensity = [[CHHapticDynamicParameter alloc] initWithParameterID:CHHapticDynamicParameterIDHapticIntensityControl value:i relativeTime:CHHapticTimeImmediate];
        auto sharpness = [[CHHapticDynamicParameter alloc] initWithParameterID:CHHapticDynamicParameterIDHapticSharpnessControl value:s relativeTime:CHHapticTimeImmediate];
        
        [player_ sendParameters:@[intensity, sharpness] atTime:CHHapticTimeImmediate error:&error];
        
        if (error != nil) {
            NSLog(@"ofxiOSCoreHaptics::sendParameters %@", error);
            return false;
        } else {
            return true;
        }
    }
    return false;
}
