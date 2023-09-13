import { useSiteStore } from "~/stores/site"
import { GetSiteFromHost } from "~/lib/domain/sites"

export default defineNuxtPlugin((app) => {
    const host = window.location.host
    const site = GetSiteFromHost(host);
    const store = useSiteStore();
    store.setSite(site);
})