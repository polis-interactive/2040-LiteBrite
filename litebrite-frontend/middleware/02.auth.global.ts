import { useSiteStore } from "~/stores/site"

export default defineNuxtRouteMiddleware((to, from) => {
  const jwt = useCookie('lighting-session');
  const hasJwt = !!jwt.value;
  if (!hasJwt && to.path !== "/login") {
    return navigateTo('/login')
  } else if (hasJwt && to.path === "/login") {
    return navigateTo('/');
  }
  const app = useNuxtApp();
  const store = useSiteStore(app.$pinia);
})
