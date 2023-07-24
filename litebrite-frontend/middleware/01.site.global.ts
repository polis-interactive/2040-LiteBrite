import { useSiteStore } from "~/stores/site"
import { GetSiteFromHost } from "~/lib/domain/sites"

export default defineNuxtRouteMiddleware((to, from) => {
    const app = useNuxtApp()
    let host = ''
    if(process.server) {
      host = app.ssrContext?.event.node.req.headers.host ?? ""
    } else {
      host = window.location.host
    }
    const site = GetSiteFromHost(host);
    const store = useSiteStore(app.$pinia);
    store.setSite(site);
})
