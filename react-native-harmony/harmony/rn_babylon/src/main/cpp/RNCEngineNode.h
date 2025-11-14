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
#pragma once

#include "RNOH/arkui/ArkUINode.h"
#include <ace/xcomponent/native_interface_xcomponent.h>

namespace rnoh {
    class RNCEngineNode : public ArkUINode {
    public:
        RNCEngineNode();
        ~RNCEngineNode() override;
        
        // 普通
        static void OnSurfaceCreated(OH_NativeXComponent *component, void *window);
        static void OnSurfaceChanged(OH_NativeXComponent *component, void *window);
        static void OnSurfaceDestroyed(OH_NativeXComponent *component, void *window);
        static void DispatchTouchEvent(OH_NativeXComponent *component, void *window);

        // XR
        static void OnXRSurfaceCreated(OH_NativeXComponent *component, void *window);
        static void OnXRSurfaceChanged(OH_NativeXComponent *component, void *window);
        static void OnXRSurfaceDestroyed(OH_NativeXComponent *component, void *window);

        void setAntiAliasing(int32_t value);
        void setAndroidView(std::string androidView);
        void setIsTransparent(bool isTransparent);

    private:
        void setIsTransparentAndAndroidView(bool isTransparent, std::string androidView);
        void startRenderLoop();
        void stopRenderLoop();
        static void onFrameCallback(OH_NativeXComponent *component, uint64_t timestamp, uint64_t targetTimestamp);
        static RNCEngineNode *getNode(OH_NativeXComponent *component);
        void UpdateView(void *window, uint64_t width, uint64_t height);
        void UpdateXRView(void *window);
        void UpdateMSAA(int32_t value);
        bool isXRActive();
        void renderView();
        void reportMotionEvent(OH_NativeXComponent *component, void *window);
        void createGeneralXComponent();
        void createXRXComponent();
        
        OH_NativeXComponent_Callback m_xComponentCallback_;
        OH_NativeXComponent_Callback m_xrXComponentCallback;
        ArkUI_NodeHandle m_General_NodeHandle{nullptr};
        ArkUI_NodeHandle m_xrNodeHandle{nullptr};
        bool m_IsRendering{false};
        bool m_IsTransparent{false};
        std::string m_View{""};
        OH_NativeXComponent *m_nativeXComponent{nullptr};
        OH_NativeXComponent *m_xr_nativeXComponent{nullptr};

        static inline std::unordered_map<OH_NativeXComponent *, RNCEngineNode *> s_componentMap;
        static inline std::mutex s_Mutex;
    };
}