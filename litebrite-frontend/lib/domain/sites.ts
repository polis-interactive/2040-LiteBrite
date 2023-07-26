import { User } from "./user";

export interface Site {
    id: number,
    name: string,
    subdomain: string
};

const SiteMap: Array<Site> = [
    {
        "id": -1,
        "name": "",
        "subdomain": ""
    },
    {
        "id": 0,
        "name": "Admin",
        "subdomain": "admin"
    },
    {
        "id": 1,
        "name": "Thompson Lite Brite",
        "subdomain": "thompson"
    }
];

export const AvaliableHosts: Array<Site> = SiteMap.slice(1).sort((a, b) => {
    if (a.name === "Admin") {
        return 1;
    } else if (b.name === "Admin") {
        return -1;
    } else if (a.name === b.name) {
        return 0;
    } else {
        return a.name > b.name ? 1 : -1;
    }
});

export const GetSiteFromHost = (host: string) => {
    const firstSubDomain = host.split('.')[0];
    const site = SiteMap.find((site) => site.subdomain === firstSubDomain ) || SiteMap[0];
    return { ...site };
}

export const ValidateUserSite = (user: User, site: Site): boolean => {
    return user.isAdmin || user.siteId === site.id;
}