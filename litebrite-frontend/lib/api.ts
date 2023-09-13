
import { TryGetUserFromObject, User } from "./domain/user"
import { Site, TryGetSitesFromObjectArray } from "./domain/sites"

import { useApiFetch } from "~/composables/useApiFetch"

interface RawIdentityPayload {
    user: Object,
    sites: Array<Object>
}

export interface GetIdentityPayload {
    user: User | null,
    sites: Array<Site> | null
}

export const GetIdentity = async (): Promise<GetIdentityPayload> => {
    const { data, error } = await useApiFetch("/identify", { method: "POST" });
    if (error.value) {
        console.error("api.GetIdentity: Failed to fetch identity");
        return { user: null, sites: null };
    }
    const rawData = toRaw(data.value) as RawIdentityPayload;
    if (!Object.hasOwn(rawData, 'sites') || !Object.hasOwn(rawData, 'user')) {
        console.error("api.GetIdentity: malformed response");
        return { user: null, sites: null };
    }
    return {
        user: TryGetUserFromObject(rawData.user),
        sites: TryGetSitesFromObjectArray(rawData.sites)
    };
}