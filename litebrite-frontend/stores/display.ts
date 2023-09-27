import { 
  DisplayState, Display, DisplayComparators, DisplayType, 
  ChangeDisplayType, DisplayValue, GetDisplayValue
} from '~/lib/domain/display';
import { defineStore, storeToRefs } from 'pinia'

import { useSiteStore } from './site';
import { useToastStore } from './toast';
import { GetDisplay, SaveDisplay, DeleteDisplay } from '~/lib/api';

import _cloneDeep from 'lodash.clonedeep'
import { CRGB, CRGBW } from '~/lib/domain/color';
import { AsyncTimeout } from '~/lib/utils';
import { Toast } from '~/lib/domain/toast';


const defaultDisplayState = (): DisplayState => ({
  hasLoaded: false,
  loadingRemote: false,
  deletingRemote: false,
  savingRemote: false,
  localDisplay: null,
  remoteDisplay: null,
  remoteIsDefault: true
});

const hasDisplayChange = (a: Display | null, b: Display | null): boolean => {
  if (a === null || b === null) {
    return false;
  }
  const a_style = a.style.type;
  if (a_style !== b.style.type) {
    return true;
  }
  return DisplayComparators[a_style](a, b);
}



export const useDisplayStore = defineStore('display', () => {

  /* state */
  const defaultState = defaultDisplayState();
  const hasLoaded = ref(defaultState.hasLoaded);
  const loadingRemote = ref(defaultState.loadingRemote);
  const deletingRemote  = ref(defaultState.deletingRemote);
  const savingRemote  = ref(defaultState.savingRemote);
  const localDisplay: Ref<null | Display> = ref(defaultState.localDisplay);
  const remoteDisplay: Ref<null | Display> = ref(defaultState.remoteDisplay);
  const remoteIsDefault = ref(defaultState.remoteIsDefault);
  const resetHack = ref(0);

  function $reset() {
    const defaultState = defaultDisplayState();
    hasLoaded.value = defaultState.hasLoaded;
    loadingRemote.value = defaultState.loadingRemote;
    deletingRemote.value = defaultState.deletingRemote;
    savingRemote.value = defaultState.savingRemote;
    localDisplay.value = defaultState.localDisplay;
    remoteDisplay.value = defaultState.remoteDisplay;
    remoteIsDefault.value = defaultState.remoteIsDefault;
    resetHack.value += 1;
  }

  /* getters */
  const hasSaveableChange = computed(() => {
    const currentDisp = localDisplay.value;
    const remoteDisp = remoteDisplay.value;
    return hasDisplayChange(currentDisp, remoteDisp)
  });

  const localDisplayValueType: ComputedRef<DisplayValue | null> = computed(() => {
    if (localDisplay.value !== null) {
      return GetDisplayValue(localDisplay.value.style.type);
    }
    return null;
  })

  const displayIsLoading = computed(() => {
    return loadingRemote.value || deletingRemote.value || savingRemote.value;
  });

  /* local actions */

  const changeDisplayType = (newType: DisplayType) => {
    const oldDisplay: Display = toRaw(localDisplay.value as Display);
    const newDisplay = ChangeDisplayType(newType, oldDisplay);
    localDisplay.value = newDisplay
  }

  const changeRgbColor = (c: CRGB) => {
    if (localDisplay.value !== null && localDisplayValueType.value === 'RGB') {
      localDisplay.value.rgb_color = { ...c };
    }
  }

  const changeRgbwColor = (c: CRGBW) => {
    if (localDisplay.value !== null && localDisplayValueType.value === 'RGBW') {
      localDisplay.value.rgbw_color = { ...c };
    }
  }

  const resetCurrentDisplay = () => {
    const tmpRemoteDisplay: Display = toRaw(remoteDisplay.value as Display);
    localDisplay.value = _cloneDeep(tmpRemoteDisplay);
    resetHack.value += 1;
  }

  /* remote actions */

  const featchRemoteDisplay = async (): Promise<boolean> => {
    let retSuccess = false;
    try {
      loadingRemote.value = true;
      await AsyncTimeout(500);
      const siteStore = useSiteStore();
      const { display, isDefault } = await GetDisplay(siteStore.currentSiteId);
      /* display null is just a hanleded error... */
      if (display !== null) {
        remoteDisplay.value = _cloneDeep(display);
        localDisplay.value = _cloneDeep(display);
        remoteIsDefault.value = isDefault;
        hasLoaded.value = true;
        retSuccess = true;
      }
    } catch (e) {
      console.error(`useDisplayStore.fetchRemoteState: failed with error: ${e}`);
    } finally {
      loadingRemote.value = false;
      return retSuccess;
    }
  }

  const toastStore = useToastStore();

  const saveCurrentDisplay = async () => {
    try {
      savingRemote.value = true;
      await AsyncTimeout(500);
      const siteStore = useSiteStore();
      const tmpLocalDisplay = toRaw(localDisplay.value);
      const { success } = await SaveDisplay(tmpLocalDisplay as Display, siteStore.currentSiteId);
      if (success) {
        remoteDisplay.value = _cloneDeep(tmpLocalDisplay);
        remoteIsDefault.value = false;
        toastStore.pushQueue(new Toast('Successfully saved current display!', 'success'));
      }
    } catch (e) {
      console.error(`useDisplayStore.saveCurrentDisplay: failed with error: ${e}`);
      toastStore.pushQueue(new Toast('Failed to save display; refresh page and try again', 'error'));
    } finally {
      savingRemote.value = false;
    }
  }

  const deleteCurrentDisplay = async () => {
    try {
      deletingRemote.value = true;
      await AsyncTimeout(500);
      const siteStore = useSiteStore();
      const { display } = await DeleteDisplay(siteStore.currentSiteId);
      if (display !== null) {
        remoteDisplay.value = _cloneDeep(display);
        localDisplay.value = _cloneDeep(display);
        remoteIsDefault.value = true;
        resetHack.value += 1;
        toastStore.pushQueue(new Toast(
          'Successfully deleted saved display; fixture is now in default state', 'success'
        ));
      }
    } catch (e) {
      console.error(`useDisplayStore.deleteCurrentDisplay: failed with error: ${e}`);
      toastStore.pushQueue(new Toast('Failed to delete saved display; refresh page and try again', 'error'));
    } finally {
      deletingRemote.value = false;
    }
  }


  /* watchers */

  // on change site, empty the state
  const siteStore = useSiteStore();
  const { currentSiteId } = storeToRefs(siteStore)
  watch(currentSiteId, $reset);

  return {
    hasLoaded, loadingRemote, savingRemote, deletingRemote, localDisplay, remoteDisplay, remoteIsDefault,
      resetHack,
    hasSaveableChange, displayIsLoading, localDisplayValueType,
    changeDisplayType, resetCurrentDisplay, changeRgbColor, changeRgbwColor,
    featchRemoteDisplay, saveCurrentDisplay, deleteCurrentDisplay
  }
});