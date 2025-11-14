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

#include "RNOH/Package.h"
#include "RNOH/ArkTSTurboModule.h"
#include "NativeRNBabylonModule.h"
#include "NativeEngineViewJSIBinder.h"
#include "BaseNativeEngineViewComponentInstance.h"
#include "ComponentDescriptors.h"

namespace rnoh {
    class BabylonPackageComponentInstanceFactoryDelegate : public ComponentInstanceFactoryDelegate {
    public:
        using ComponentInstanceFactoryDelegate::ComponentInstanceFactoryDelegate;
    
        ComponentInstance::Shared create(ComponentInstance::Context ctx) override {
            if (ctx.componentName == "NativeEngineView") {
                return std::make_shared<BaseNativeEngineViewComponentInstance>(std::move(ctx));
            }
            return nullptr;
        }
    };

    class BaseReactNativeOhTplBabylonjsReactNativeHarmonyPackageTurboModuleFactoryDelegate : public TurboModuleFactoryDelegate {
    public:
        SharedTurboModule createTurboModule(Context ctx, const std::string &name) const override {
            if (name == "NativeRNBabylonModule") {
                return std::make_shared<NativeRNBabylonModule>(ctx, name);
            }
            return nullptr;
        };
    };

    class BaseReactNativeOhTplBabylonjsReactNativeHarmonyPackageEventEmitRequestHandler : public EventEmitRequestHandler {
    public:
        void handleEvent(Context const &ctx) override {
            auto eventEmitter = ctx.shadowViewRegistry->getEventEmitter<facebook::react::EventEmitter>(ctx.tag);
            if (eventEmitter == nullptr) {
                return;
            }
    
            std::vector<std::string> supportedEventNames = {
                "snapshotDataReturned",
            };
            if (std::find(supportedEventNames.begin(), supportedEventNames.end(), ctx.eventName) != supportedEventNames.end()) {
                eventEmitter->dispatchEvent(ctx.eventName, ArkJS(ctx.env).getDynamic(ctx.payload));
            }    
        }
    };

    class BaseReactNativeOhTplBabylonjsReactNativeHarmonyPackage : public Package {
    public:
        BaseReactNativeOhTplBabylonjsReactNativeHarmonyPackage(Package::Context ctx) : Package(ctx){};
        
        ComponentInstanceFactoryDelegate::Shared createComponentInstanceFactoryDelegate() override {
            return std::make_shared<BabylonPackageComponentInstanceFactoryDelegate>();
        }
    
        std::unique_ptr<TurboModuleFactoryDelegate> createTurboModuleFactoryDelegate() override {
            return std::make_unique<BaseReactNativeOhTplBabylonjsReactNativeHarmonyPackageTurboModuleFactoryDelegate>();
        }
    
        std::vector<facebook::react::ComponentDescriptorProvider> createComponentDescriptorProviders() override {
            return {
                facebook::react::concreteComponentDescriptorProvider<facebook::react::NativeEngineViewComponentDescriptor>()
            };
        }
    
        ComponentJSIBinderByString createComponentJSIBinderByName() override {
            return {
                {"NativeEngineView", std::make_shared<NativeEngineViewJSIBinder>()}
            };
        }
        
        EventEmitRequestHandlers createEventEmitRequestHandlers() override {
            return {
                std::make_shared<BaseReactNativeOhTplBabylonjsReactNativeHarmonyPackageEventEmitRequestHandler>(),
            };
        }
    };
} // namespace rnoh