import { User, TryGetUserFromObject } from "~/lib/domain/user";
import { Site, ValidateUserSite } from "~/lib/domain/sites";
import { useSiteStore } from "~/stores/site"
import { useUserStore } from "~/stores/user"

import jwt_decode from "jwt-decode";

const parseAndVerifyJwt = (site: Site): null | User => {
  const jwt = useCookie('lighting-session');
  if (!jwt.value) {
    return null;
  }
  try {
    const decoded = jwt_decode(jwt.value);
    const user = TryGetUserFromObject(decoded);
    if (user === null) {
      throw new TypeError("JWT received malformed user");
    }
    if (!ValidateUserSite(user, site)) {
      throw new EvalError("Invalid user credentials");
    }
    return user;
  } catch {
    jwt.value = null;
    return null;
  }
}

export default defineNuxtRouteMiddleware((to, from) => {
  const app = useNuxtApp();
  const { $state } = useSiteStore(app.$pinia);
  const user = parseAndVerifyJwt($state);
  const userStore = useUserStore(app.$pinia);
  userStore.setUser(user);
  if (user === null && to.path !== "/login") {
    return navigateTo('/login')
  } else if (user !== null && to.path === "/login") {
    return navigateTo('/');
  }
})
