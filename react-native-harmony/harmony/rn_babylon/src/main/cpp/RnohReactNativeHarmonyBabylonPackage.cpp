/**
 * GUN License
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
#include "RnohReactNativeHarmonyBabylonPackage.h"
#include "ComponentDescriptors.h"
#include "NativeEngineViewJSIBinder.h"
#include "NativeEngineViewNapiBinder.h"
#include "RNBabylonModule.h"
#include "RNCNativeEngineViewComponentInstance.h"
#include "RNOH/ArkTSTurboModule.h"
#include <react/renderer/debug/SystraceSection.h>
using namespace rnoh;
using namespace facebook;

class BabylonPackageComponentInstanceFactoryDelegate : public ComponentInstanceFactoryDelegate {
public:
    using ComponentInstanceFactoryDelegate::ComponentInstanceFactoryDelegate;

    ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
        if (ctx.componentName == "NativeEngineView") {
            return std::make_shared<RNCNativeEngineViewComponentInstance>(std::move(ctx));
        }
        return nullptr;
    }
};


class BabylonTurboModuleFactoryDelegate : public TurboModuleFactoryDelegate {
public:
    SharedTurboModule createTurboModule(Context ctx, const std::string &name) const override {
        if (name == "NativeRNBabylonModule") {
            return std::make_shared<RNBabylonModule>(ctx, name);
        }
        return nullptr;
    };
};

std::unique_ptr<TurboModuleFactoryDelegate> RnohReactNativeHarmonyBabylonPackage::createTurboModuleFactoryDelegate() {
    return std::make_unique<BabylonTurboModuleFactoryDelegate>();
}

ComponentInstanceFactoryDelegate::Shared
RnohReactNativeHarmonyBabylonPackage::createComponentInstanceFactoryDelegate() {
    return std::make_shared<BabylonPackageComponentInstanceFactoryDelegate>();
}

std::vector<facebook::react::ComponentDescriptorProvider>
RnohReactNativeHarmonyBabylonPackage::createComponentDescriptorProviders() {
    return {facebook::react::concreteComponentDescriptorProvider<facebook::react::EngineViewComponentDescriptor>()};
}

ComponentJSIBinderByString RnohReactNativeHarmonyBabylonPackage::createComponentJSIBinderByName()  {
    return {
        {"NativeEngineView", std::make_shared<NativeEngineViewJSIBinder>()},
    };
}

ComponentNapiBinderByString RnohReactNativeHarmonyBabylonPackage::createComponentNapiBinderByName()  {
    return {
        {"NativeEngineView", std::make_shared<NativeEngineViewNapiBinder>()},
    };
}

class EngineViewEventEmitRequestHandler : public EventEmitRequestHandler {
    void handleEvent(EventEmitRequestHandler::Context const &ctx) override {
        DLOG(INFO) << "handleEvent eventName:" << ctx.eventName;
        
        if (ctx.eventName != "NativeEngineView") {
            return;
        }
        
        facebook::react::SystraceSection s("RNGH::BabylonEventEmitRequestHandler::handleEvent");
        auto eventEmitter = ctx.shadowViewRegistry->getEventEmitter<facebook::react::ViewEventEmitter>(ctx.tag);
        if (eventEmitter == nullptr) {
            return;
        }
        
        std::vector<std::string> supportedEventNames = {};
        if (std::find(supportedEventNames.begin(), supportedEventNames.end(), ctx.eventName) != supportedEventNames.end()) {
            eventEmitter->dispatchEvent(ctx.eventName, ArkJS(ctx.env).getDynamic(ctx.payload));
        }    
    }
};

EventEmitRequestHandlers RnohReactNativeHarmonyBabylonPackage::createEventEmitRequestHandlers() {
    return {
        std::make_shared<EngineViewEventEmitRequestHandler>(),
    };
}