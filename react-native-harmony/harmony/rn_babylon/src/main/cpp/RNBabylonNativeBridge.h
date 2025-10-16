/**
 * MIT License
 *
 * Copyright (C) 2025 Huawei Device Co., Ltd.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef RN_BABYLON_SRC_MAIN_CPP_RNBABYLONNATIVEBRIDGE_H
#define RN_BABYLON_SRC_MAIN_CPP_RNBABYLONNATIVEBRIDGE_H

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
     }

     void BabySetTouchButtonState(int pointerId, bool isDown, float x, float y) {
          BabylonNative::SetTouchButtonState(static_cast<uint32_t>(pointerId), isDown, static_cast<uint32_t>(x),
          static_cast<uint32_t>(y));
     }

     void BabySetTouchPosition(int pointerId, float x, float y) {
          BabylonNative::SetTouchPosition(static_cast<uint32_t>(pointerId), static_cast<uint32_t>(x),
          static_cast<uint32_t>(y));
     }
};

#endif