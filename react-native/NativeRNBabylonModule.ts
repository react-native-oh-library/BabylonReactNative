import type { TurboModule } from 'react-native/Libraries/TurboModule/RCTExport';
import { TurboModuleRegistry } from 'react-native';

export interface Spec extends TurboModule {
    initialize(): Promise<void>;
    resetView(): Promise<void>;
}

export default TurboModuleRegistry.getEnforcing<Spec>('NativeRNBabylonModule');