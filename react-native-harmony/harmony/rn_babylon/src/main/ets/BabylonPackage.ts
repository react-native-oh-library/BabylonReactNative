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

import type { UITurboModule, UITurboModuleContext } from '@rnoh/react-native-openharmony/ts';
import { RNPackage, UITurboModuleFactory } from '@rnoh/react-native-openharmony/ts';
import { RNBabylonModule } from './RNBabylonModule';
import { TM } from "./namespace/ts"

class BabylonPackageTurboModulesFactory extends UITurboModuleFactory {
  createTurboModule(name: string): UITurboModule | null {
    if (name === TM.RNBabylonModule.NAME) {
      console.log("jk----------------> RNBabylonModule 开始创建...")
      return new RNBabylonModule(this.ctx);
    }
    return null;
  }

  hasTurboModule(name: string): boolean {
    return name === TM.RNBabylonModule.NAME;
  }
}

export class BabylonPackage extends RNPackage {
  createTurboModulesFactory(ctx: UITurboModuleContext): UITurboModuleFactory {
    return new BabylonPackageTurboModulesFactory(ctx);
  }
}