/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef SMART_SRC_MAIN_CPP_SMARTREFRESHLAYOUTCOMPONENTNAPIBINDER_H
#define SMART_SRC_MAIN_CPP_SMARTREFRESHLAYOUTCOMPONENTNAPIBINDER_H
#include "Props.h"
#include "RNOH/BaseComponentNapiBinder.h"
#include "RNOHCorePackage/ComponentBinders/ViewComponentNapiBinder.h"


namespace rnoh {

class NativeEngineViewNapiBinder : public ViewComponentNapiBinder {
public:
    napi_value createProps(napi_env env, facebook::react::ShadowView const shadowView) override {
        napi_value napiBaseProps = ViewComponentNapiBinder::createProps(env, shadowView);
        if (auto props = std::dynamic_pointer_cast<const facebook::react::EngineViewProps>(shadowView.props)) {
            return ArkJS(env)
                .getObjectBuilder(napiBaseProps)
                .addProperty("isTransparent", props->isTransparent)
                .addProperty("antiAliasing", props->antiAliasing)
                .addProperty("androidView", props->androidView)
                .build();
        }
        return napiBaseProps;
    };
};

} // namespace rnoh
#endif