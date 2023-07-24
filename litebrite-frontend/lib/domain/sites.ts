
export interface Site {
    id: number,
    name: string,
    subdomain: string
};

const SiteMap: Array<Site> = [
    {
        "id": 0,
        "name": "Admin",
        "subdomain": "*"
    },
    {
        "id": 1,
        "name": "Thompson Lite Brite",
        "subdomain": "thompson"
    }
];

export const GetSiteFromHost = (host: string) => {
    const firstSubDomain = host.split('.')[0];
    const site = SiteMap.find((site) => site.subdomain === firstSubDomain ) || SiteMap[0];
    return { ...site };
}