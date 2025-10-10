//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#include "EventEmitters.h"
namespace facebook {
namespace react {

void EngineViewEventEmitter::onSnapshotDataReturned(SnapshotEventData event) const {
    dispatchEvent("snapshotDataReturned", [event = std::move(event)](jsi::Runtime &runtime) {
        auto payload = jsi::Object(runtime);
        payload.setProperty(runtime, "data", event.data);
        return payload;
    });
}
} // namespace react
} // namespace facebook