
import { Site, AvaliableSites } from '~/lib/domain/sites'
import { defineStore } from 'pinia'

export interface SiteStore {
  currentSite: number,
  availableSites: Array<Site>
};

export const useSiteStore = defineStore('site', {
  state: () => ({
    currentSite: -1,
    availableSites: []
  } as SiteStore),
  actions: {
    setAvailableSites(sites: Array<Site>) {
      this.availableSites = sites;
      if (sites.length === 1) {
        this.currentSite = sites[0].id
      }
    }
  },
  getters: {
    hasAnySites: ({ availableSites }) => availableSites.length !== 0,
    hasMultipleSites: ({ availableSites }) => availableSites.length > 1
  }
})
