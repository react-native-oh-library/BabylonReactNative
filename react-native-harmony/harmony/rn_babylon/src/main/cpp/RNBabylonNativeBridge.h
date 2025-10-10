//
// Created on 2025/8/14.
//
// Node APIs are not fully supported. To solve the compilation error of the interface cannot be found,
// please include "napi/native_api.h".

#ifndef HARMONY_BABYLONNATIVE_H
#define HARMONY_BABYLONNATIVE_H

#include "stdint.h"
#include "BabylonNative.h"

namespace rnoh {

void BabyUpdateView(OHNativeWindow *window, uint64_t width, uint64_t height) {
        BabylonNative::UpdateView(window, width, height);
}

void BabyUpdateMSAA(int val) {
       BabylonNative::UpdateMSAA(static_cast<uint8_t>(val));
}

void BabyRenderView() {
    BabylonNative::RenderView();
}


void BabyResetView() {
        BabylonNative::ResetView();
}

void BabyUpdateXRView(OHNativeWindow *window) {
     BabylonNative::UpdateXRView(window);
}

bool BabyIsXRActive() {
    return BabylonNative::IsXRActive();
    return true;
}


void BabySetTouchButtonState(int pointerId, bool isDown, float x, float y) {
     BabylonNative::SetTouchButtonState(static_cast<uint32_t>(pointerId), isDown, static_cast<uint32_t>(x),
     static_cast<uint32_t>(y));
}

void BabySetTouchPosition(int pointerId, float x, float y) {
     BabylonNative::SetTouchPosition(static_cast<uint32_t>(pointerId), static_cast<uint32_t>(x),
     static_cast<uint32_t>(y));
}


}; // namespace rnoh


#endif // HARMONY_BABYLONNATIVE_H
