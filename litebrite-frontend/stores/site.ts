
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
        this.currentSiteId = sites[0].id;
        return;
      }
      // ensure if a person is navigating back, they have access to where they want to go
      const foundSite = this.availableSites.find(site => site.id === this.currentSiteId);
      if (!foundSite) {
        this.currentSiteId = -1;
      }
    },
    async setCurrentSiteId(siteId: number) {
      const foundSite = this.availableSites.find(site => site.id === siteId);
      if (foundSite) {
        this.currentSiteId = foundSite.id;
        await InternalNavigateTo(`/applications/${foundSite.slug}`)
        
      } else {
        throw new Error("Invalid site id passed to siteStore.setCurrentSiteId");
      }
    },
    resetSiteId() {
      this.currentSiteId = -1; 
    }
  },
  getters: {
    hasAnySites: ({ availableSites }) => availableSites.length !== 0,
    doesThisResolve: () => false,
    hasCurrentSite: ({ currentSiteId }) => currentSiteId !== -1,
    currentSite({ availableSites, currentSiteId }): undefined | Site {
      if (currentSiteId === -1) {
        return undefined;
      }
      return availableSites.find(site => site.id == currentSiteId);
    },
    hasMultipleSites: ({ availableSites }) => availableSites.length > 1,
    siteSlug(): string {
      return `/applications/${this.currentSite?.slug}`
    }
  },
  persist: {
    paths: ['currentSiteId']
  }
})
