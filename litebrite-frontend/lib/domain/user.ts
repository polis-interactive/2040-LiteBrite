
export interface User {
    email: string,
    name: string,
};

export const TryGetUserFromObject = (user: any): User | null  => {
    const email = 'email' in user && typeof user.email === 'string' ? user.email: null;
    const name = 'name' in user && typeof user.name === 'string' ? user.name : null;
    
    if (email === null || name === null) {
      return null;
    } else {
      return { email, name };
    }
  }