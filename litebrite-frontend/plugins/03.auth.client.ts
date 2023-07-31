import { useSiteStore } from "~/stores/site"
import { TryGetUserFromObject } from "~/lib/domain/user"

import { useUserStore } from "~/stores/user"

export default defineNuxtPlugin(async (app) => {
    const { hasNoUser, setUser } = useUserStore();
    if (!hasNoUser) {
        return;
    } 
    try {
        const { data, error } = await useBaseUrlFetch('/api/auth/identify');
        if (!error.value && data.value) {
            const user = TryGetUserFromObject(data.value);
            if (user !== null) {
                setUser(user);
            }
          }
    } catch (err) {
        console.error(err)
    }
})