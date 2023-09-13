import { useSiteStore } from "~/stores/site"
import { useUserStore } from "~/stores/user"
import { useAppStore } from "~/stores/app"

import { useAuth0 } from "~/composables/useAuth"

export default defineNuxtRouteMiddleware(async (to, from) => {
  const router = useRouter();
  console.log("navigating")
  await router.push({
    query: "", hash: "",
  });
  // don't run on the server
  if (process.server || Object.hasOwn(to.query, 'internal_guard')) {
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
  return navigateTo("/unauthroized?internal_guard=true", { replace: true });
})
