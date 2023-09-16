import { useSiteStore } from "~/stores/site"
import { useAppStore } from "~/stores/app"

import { InternalNavigateTo } from '~/lib/utils'

import { useAuth0 } from "~/composables/useAuth"

export default defineNuxtRouteMiddleware(async (to, from) => {
    // don't run on the server
  if (process.server) {
    return;
  }
  const appStore = useAppStore();
  if (Object.hasOwn(to.query, 'internal_guard')) {
    appStore.setHasInitialized();
    return;
  }
  if (!appStore.hasAuth) {
    if (to.path !== '/logout') {
      const auth0 = await useAuth0();
      await auth0.value.loginWithRedirect();
    } else {
      // shouldn't get here because auth0 will add the query for us
      appStore.setHasInitialized();
    }
    return;
  } else if (to.path === '/logout') {
    // should be handled by component, so shoulnd't get here really. Just a backup
    const auth0 = await useAuth0();
    await auth0.value.logout();
    // we can just return because of the redirect
    return;
  }
  const siteStore = useSiteStore();
  if (!appStore.hasIdentified || !siteStore.hasAnySites) {
    // should probably differentiate between hasIdentified failing (500) and hasAnySites (401)
    return InternalNavigateTo("/unauthroized");
  }
  // make sure user is redirected to their current site, or the application selection screen
  if (siteStore.hasCurrentSite) {
    const needsRoute = `/applications/${siteStore.currentSite?.slug}`
    if (!to.path.startsWith(needsRoute)) {
      return InternalNavigateTo(needsRoute);
    } 
  } else {
    const applicationsRoute = '/applications'
    if (to.path != applicationsRoute) {
      return InternalNavigateTo(applicationsRoute);
    }
  }
  appStore.setHasInitialized();
  return InternalNavigateTo(to.path)
});
