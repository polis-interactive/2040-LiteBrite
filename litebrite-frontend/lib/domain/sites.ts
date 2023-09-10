
export interface Site {
    id: number,
    name: string,
    slug: string
};

const tryGetSiteFromObject = (site: any): Site | null  => {
    const id = 'id' in site && typeof site.id === 'number' ? site.id: null;
    const name = 'name' in site && typeof site.name === 'string' ? site.name : null;
    const slug = 'slug' in site && typeof site.slug === 'string' ? site.slug: null;
    
    if (id === null || name === null || slug === null) {
      return null;
    } else {
      return { id, name, slug };
    }
}

export const TryGetSitesFromObjectArray = (sites: Array<any>): Array<Site> | null => {
    const sitesArray: Array<Site> = [];
    const successfulParse = sites.every((site: any) => {
        const typedSite = tryGetSiteFromObject(site);
        if (typedSite !== null) {
            sitesArray.push(typedSite);
            return true;
        } else {
            return false;
        }
    });
    if (successfulParse) {
        return sitesArray;
    } else {
        return null;
    }
}