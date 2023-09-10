
import { useAppStore } from "~/stores/app";
import { useSiteStore } from "~/stores/site"
import { useUserStore } from "~/stores/user"

import { GetIdentity } from "~/lib/api"

export default defineNuxtPlugin(async (app) => {
  const appStore = useAppStore();
  if (!appStore.hasAuth) {
    return;
  }
  const { user, sites } = await GetIdentity();
  if (!user || !sites) {
    return;
  }
  const userStore = useUserStore();
  userStore.setUser(user);
  const siteStore = useSiteStore();
  siteStore.setAvailableSites(sites);
  appStore.setIdentifyDone();
  
})
