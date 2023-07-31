import { useSiteStore } from "~/stores/site"
import { useUserStore } from "~/stores/user"

export default defineNuxtRouteMiddleware((to, from) => {
  if (process.server) {
    return;
  } 
  const app = useNuxtApp();
  const { hasNoSite, id: siteId } = useSiteStore(app.$pinia);
  const userStore = useUserStore(app.$pinia);
  const { siteId: userSiteId, isAdmin, hasNoUser } = userStore;
  const isAuthenticated = !hasNoSite && !hasNoUser && (isAdmin || userSiteId === siteId);
  if (!isAuthenticated) {
    userStore.setUser(null);
  }
  if (!isAuthenticated && to.path !== "/login") {
    return navigateTo('/login')
  } else if (isAuthenticated && to.path === "/login") {
    return navigateTo('/');
  }
})
