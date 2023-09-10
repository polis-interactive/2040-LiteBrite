import { useSiteStore } from "~/stores/site"
import { useUserStore } from "~/stores/user"
import { useAppStore } from "~/stores/app"

import { useAuth0 } from "~/composables/useAuth"

export default defineNuxtRouteMiddleware(async (to, from) => {
  // don't run on the server
  if (process.server) {
    return;
  } 
  const app = useNuxtApp();
  const appStore = useAppStore();
  if (!appStore.hasAuth) {
    const auth0 = await useAuth0();
    await auth0.value.loginWithRedirect();
    return;
  }
  // clear query params just in case
  const router = useRouter();
  router.replace({ query: "", hash: "" })
})
