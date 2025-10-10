//
// Created on 2025/8/13.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_EVENTEMITTERS_H
#define HARMONY_EVENTEMITTERS_H
#include <jsi/jsi.h>
#include <react/renderer/components/view/ViewEventEmitter.h>

namespace facebook {
namespace react {
class JSI_EXPORT EngineViewEventEmitter : public ViewEventEmitter {
public:
    using ViewEventEmitter::ViewEventEmitter;

    struct SnapshotEventData {
        std::string data;
    };
    void onSnapshotDataReturned(SnapshotEventData value) const;
};
} // namespace react
} // namespace facebook

#endif // HARMONY_EVENTEMITTERS_H
