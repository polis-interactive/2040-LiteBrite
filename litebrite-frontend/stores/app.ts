import { App, AppPage } from '~/lib/domain/app';
import { defineStore } from 'pinia'
import { InternalNavigateTo } from '~/lib/utils';
import { useSiteStore } from './site';

export const useAppStore = defineStore('app', {
  state: () => ({
    hasAuth: false,
    hasIdentified: false,
    hasInitialized: false,
    appPages: []
  } as App ),
  actions: {
    setAuthDone() {
      this.hasAuth = true;
    },
    setIdentifyDone() {
      this.hasIdentified = true;
    },
    setHasInitialized() {
      this.hasInitialized = true;
    },
    setAppPages(appPages: Array<AppPage> = []) {
      this.appPages = appPages
    },
    async setAppPage(pageSlug: string) {
      const siteStore = useSiteStore();
      const route = useRoute();
      const foundPage = this.appPages.find(page => page.slug === pageSlug);
      if (!foundPage) {
        await InternalNavigateTo('/unauthorized');
        return;
      } 
      const wantsPath = `${siteStore.siteSlug}/${pageSlug}`;
      if (!route.path.startsWith(wantsPath)) {
        await InternalNavigateTo(wantsPath);
      }

    }
  }
})
