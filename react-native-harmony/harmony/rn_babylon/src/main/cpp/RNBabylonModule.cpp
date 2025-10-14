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
#include "RNBabylonModule.h"

namespace rnoh {
    using namespace facebook;

    jsi::Value initialize(facebook::jsi::Runtime &rt, react::TurboModule &turboModule, const facebook::jsi::Value *args,
                        size_t count) {
        auto self = static_cast<RNBabylonModule *>(&turboModule);
        auto jsDispatcher = self->createJsDispatcher(self->getContext());
        return facebook::react::createPromiseAsJSIValue(
            rt, [&jsDispatcher](jsi::Runtime &runtime, std::shared_ptr<facebook::react::Promise> promise) {
                BabylonNative::Initialize(runtime, jsDispatcher);
                promise->resolve(jsi::Value().null());
            });
    }

    jsi::Value resetView(facebook::jsi::Runtime &rt, react::TurboModule &turboModule, const facebook::jsi::Value *args,
                        size_t count) {
        auto self = static_cast<RNBabylonModule *>(&turboModule);
        return facebook::react::createPromiseAsJSIValue(
            rt, [self](jsi::Runtime &runtime, std::shared_ptr<facebook::react::Promise> promise) {
                auto jsDispatcher = self->createMainDispatcher(self->getContext());
                jsDispatcher([promise]() {
                    BabylonNative::ResetView();
                    promise->resolve(jsi::Value().null());
                });
            });
    }

    RNBabylonModule::RNBabylonModule(const ArkTSTurboModule::Context ctx, const std::string name)
        : ArkTSTurboModule(ctx, name) {

        methodMap_ = {
            {"initialize", {0, rnoh::initialize}},
            {"resetView", {0, rnoh::resetView}},
            ARK_METHOD_METADATA(addListener, 1),
            ARK_METHOD_METADATA(removeListeners, 1),
        };
        std::thread::id this_id = std::this_thread::get_id();
    }
}