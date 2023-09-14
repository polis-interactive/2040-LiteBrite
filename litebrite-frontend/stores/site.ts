
import { InternalNavigateTo } from '~/lib/utils'
import { Site } from '~/lib/domain/sites'
import { defineStore } from 'pinia'

export interface SiteStore {
  currentSiteId: number,
  availableSites: Array<Site>
};

export const useSiteStore = defineStore('site', {
  state: () => ({
    currentSiteId: -1,
    availableSites: []
  } as SiteStore),
  actions: {
    setAvailableSites(sites: Array<Site>) {
      this.availableSites = sites;
      if (sites.length === 1) {
        this.currentSiteId = sites[0].id
      }
    },
    async setCurrentSiteId(siteId: number) {
      const foundSite = this.availableSites.find(site => site.id === siteId);
      console.log(siteId);
      console.log(this.availableSites);
      console.log(foundSite);
      if (foundSite) {
        this.currentSiteId = foundSite.id;
        await InternalNavigateTo(`/applications/${foundSite.slug}`)
        
      } else {
        throw new Error("Invalid site id passed to siteStore.setCurrentSiteId");
      }
    }
  },
  getters: {
    hasAnySites: ({ availableSites }) => availableSites.length !== 0,
    hasCurrentSite: ({ currentSiteId }) => currentSiteId !== -1,
    currentSite: ({ availableSites, currentSiteId }) => 
      currentSiteId !== -1 ? availableSites.find(site => site.id == currentSiteId) : undefined,
    hasMultipleSites: ({ availableSites }) => availableSites.length > 1
  }
})
