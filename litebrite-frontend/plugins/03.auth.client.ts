
import { useAuth0 } from "~/composables/useAuth"

export default defineNuxtPlugin(async (app) => {
  // installs auth0 client on page load
  const auth0 = await useAuth0();
  const isAuth = await auth0.value.isAuthenticated();
  if (!isAuth) {
    const query = window.location.search;
    if (!query.includes("code=") || !query.includes("state=")) {
      return;
    }
    await auth0.value.handleRedirectCallback();
  }
  const didItWork = await auth0.value.isAuthenticated();
  console.log(didItWork)
})