import { App } from '~/lib/domain/app';
import { defineStore } from 'pinia'

export const useAppStore = defineStore('app', {
  state: () => ({
    hasAuth: false,
    hasIdentified: false
  } as App ),
  actions: {
    setAuthDone() {
      this.hasAuth = true;
    },
    setIdentifyDone() {
      this.hasIdentified = true;
    }
  }
})
