
import { TryGetUserFromObject, User } from "./domain/user"
import { Site, TryGetSitesFromObjectArray } from "./domain/sites"
import { Display, DisplayFromDTO } from "./domain/display"

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

interface RawDisplayPayload {
    display: Object,
    is_default: boolean
}

export interface GetDisplayPayload {
    display: Display | null
    isDefault: boolean
}

export const GetDisplay = async (siteId: number): Promise<GetDisplayPayload> => {

    const { data, error } = await useApiFetch("/site/display", { 
        method: "POST",
        params: {
            'site_id': siteId
        }
    });

    const rawData = toRaw(data.value) as RawDisplayPayload;
    if (!Object.hasOwn(rawData, 'display') || !Object.hasOwn(rawData, 'is_default')) {
        console.error("api.GetIdentity: malformed response");
        return { display: null, isDefault: true };
    }

    const display = DisplayFromDTO(rawData.display);
    return { display, isDefault: rawData.is_default };
}