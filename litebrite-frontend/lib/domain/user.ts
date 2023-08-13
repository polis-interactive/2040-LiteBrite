
export interface User {
    id: number,
    email: string,
    name: string,
    isAdmin: boolean,
    siteId: number
};

export const TryGetUserFromObject = (user: any): User | null  => {
    const id = 'id' in user && typeof user.id === 'number' ? user.id : null;
    const email = 'email' in user && typeof user.email === 'string' ? user.email: null;
    const name = 'name' in user && typeof user.name === 'string' ? user.name : null;
    const isAdmin = 'is_admin' in user && typeof user.is_admin === 'boolean' ? user.is_admin : null;
    const siteId = 'site_id' in user && typeof user.site_id === 'number' ? user.site_id : null;
    
    if (id === null || email === null || name === null || isAdmin === null || siteId === null) {
      return null;
    } else {
      return { id, email, name, isAdmin, siteId };
    }
  }