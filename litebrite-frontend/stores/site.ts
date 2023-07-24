import { Site } from '~/lib/domain/sites'
import { defineStore } from 'pinia'

export const useSiteStore = defineStore('site', {
  state: () => ({
    id: -1,
    name: '',
    subdomain: ''
  } as Site),
  actions: {
    setSite(site: Site) {
      Object.assign(this, site);
    }
  }
})
