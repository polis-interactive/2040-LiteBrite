import { DisplayState, Display, DisplayComparators } from '~/lib/domain/display';
import { defineStore, storeToRefs } from 'pinia'

import { useSiteStore } from './site';
import { GetDisplay } from '~/lib/api';

import _cloneDeep from 'lodash.clonedeep'


const defaultDisplayState = (): DisplayState => ({
  hasLoaded: false,
  loadingRemote: false,
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
  const localDisplay: Ref<null | Display> = ref(defaultState.localDisplay);
  const remoteDisplay: Ref<null | Display> = ref(defaultState.remoteDisplay);
  const remoteIsDefault = ref(defaultState.remoteIsDefault);

  function $reset() {
    const defaultState = defaultDisplayState();
    hasLoaded.value = defaultState.hasLoaded;
    loadingRemote.value = defaultState.loadingRemote;
    localDisplay.value = defaultState.localDisplay;
    remoteDisplay.value = defaultState.remoteDisplay;
    remoteIsDefault.value = defaultState.remoteIsDefault;
  }

  /* getters */
  const hasSaveableChange = computed(() => {
    const currentDisp = localDisplay.value;
    const remoteDisp = remoteDisplay.value;
    return hasDisplayChange(currentDisp, remoteDisp)
  });

  /* actions */
  const fetchRemoteState = async () => {
    try {
      loadingRemote.value = true;
      const siteStore = useSiteStore();
      const { display, isDefault } = await GetDisplay(siteStore.currentSiteId);
      /* display null is just a hanleded error... */
      if (display !== null) {
        remoteDisplay.value = _cloneDeep(display);
        localDisplay.value = _cloneDeep(display);
        remoteIsDefault.value = _cloneDeep(isDefault);
        hasLoaded.value = true;
      }
    } catch (e) {
      console.error(`useDisplayStore.fetchRemoteState: failed with error: ${e}`);
    } finally {
      loadingRemote.value = false;
    }
  }

  const saveCurrentDisplay = async () => {

  }

  const deleteCurrentDisplay = async () => {
    
  }


  /* watchers */

  // on change site, empty the state
  const siteStore = useSiteStore();
  const { currentSiteId } = storeToRefs(siteStore)
  watch(currentSiteId, $reset);
})
  