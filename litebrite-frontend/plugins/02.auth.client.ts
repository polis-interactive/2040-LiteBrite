
import { useAppStore } from "~/stores/app";
import { useAuth0 } from "~/composables/useAuth"

export default defineNuxtPlugin(async (app) => {
  // installs auth0 client on page load
  const auth0 = await useAuth0();
  const appStore = useAppStore();

  let isAuth = await auth0.value.isAuthenticated();
  if (isAuth) {
    appStore.setAuthDone();
    return;
  }
  const query = window.location.search;
  if (!query.includes("code=") || !query.includes("state=")) {
    return;
  }
  await auth0.value.handleRedirectCallback();
  isAuth = await auth0.value.isAuthenticated();
  appStore.setAuthDone();
})
