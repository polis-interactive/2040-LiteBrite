
import { TryGetUserFromObject, User } from "./domain/user"
import { Site, TryGetSitesFromObjectArray } from "./domain/sites"
import { Display, DisplayFromDTO, DisplayToDTO } from "./domain/display"

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

interface RawGetDisplayPayload {
    display: Object,
    is_default: boolean
}

export interface GetDisplayPayload {
    display: Display | null
    isDefault: boolean
}

export const GetDisplay = async (siteId: number): Promise<GetDisplayPayload> => {

    const { data, error } = await useApiFetch("/site/display/fetch", { 
        method: "POST",
        body: {
            'site_id': siteId
        }
    });

    const rawData = toRaw(data.value) as RawGetDisplayPayload;
    if (!Object.hasOwn(rawData, 'display') || !Object.hasOwn(rawData, 'is_default')) {
        console.error("api.GetIdentity: malformed response");
        return { display: null, isDefault: true };
    }

    const display = DisplayFromDTO(rawData.display);
    return { display, isDefault: rawData.is_default };
}


interface RawDeleteDisplayPayload {
    display: Object
}

export interface DeleteDisplayPayload {
    display: Display | null
}

export const DeleteDisplay = async (siteId: number): Promise<DeleteDisplayPayload> => {

    const { data, error } = await useApiFetch("/site/display/delete", { 
        method: "POST",
        body: {
            'site_id': siteId
        }
    });

    const rawData = toRaw(data.value) as RawDeleteDisplayPayload;
    if (!Object.hasOwn(rawData, 'display')) {
        console.error("api.DeleteDisplay: malformed response");
        return { display: null };
    }

    const display = DisplayFromDTO(rawData.display);
    return { display };
}


export interface SaveDisplayPayload {
    success: boolean
}

export const SaveDisplay = async (display: Display, siteId: number): Promise<SaveDisplayPayload> => {

    const rawDisplay = DisplayToDTO(display);

    const { data, error } = await useApiFetch("/site/display/update", { 
        method: "POST",
        body: {
            'site_id': siteId,
            'display': rawDisplay
        }
    });

    const rawData = toRaw(data.value) as SaveDisplayPayload;
    if (!Object.hasOwn(rawData, 'success')) {
        console.error("api.SaveDisplay: malformed response");
        return { success: false };
    }

    return { success: rawData.success };
}