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
#ifndef RN_BABYLON_SRC_MAIN_CPP_NATIVEENGINEVIEWJSIBINDER_H
#define RN_BABYLON_SRC_MAIN_CPP_NATIVEENGINEVIEWJSIBINDER_H

#include "RNOH/BaseComponentJSIBinder.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentJSIBinder.h"

namespace rnoh {
    class NativeEngineViewJSIBinder : public ViewComponentJSIBinder {
    protected:
        facebook::jsi::Object createNativeProps(facebook::jsi::Runtime &rt) override {
            auto object = ViewComponentJSIBinder::createNativeProps(rt);
            object.setProperty(rt, "isTransparent", "boolean");
            object.setProperty(rt, "antiAliasing", "number");
            object.setProperty(rt, "androidView", "string");
            return object;
        }

        facebook::jsi::Object createCommands(facebook::jsi::Runtime &rt) override {
            auto commands = ViewComponentJSIBinder::createCommands(rt);
            commands.setProperty(rt, "takeSnapshot", "takeSnapshot");
            return commands;
        }

        facebook::jsi::Object createBubblingEventTypes(facebook::jsi::Runtime &rt) override {
            facebook::jsi::Object events(rt);
            return events;
        }

        facebook::jsi::Object createDirectEventTypes(facebook::jsi::Runtime &rt) override {
            facebook::jsi::Object events(rt);
            events.setProperty(rt, "topSnapshotDataReturned", createDirectEvent(rt, "onSnapshotDataReturned"));
            return events;
        }
    };
}

#endif