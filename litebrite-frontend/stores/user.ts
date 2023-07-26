import { User } from '~/lib/domain/user'
import { defineStore } from 'pinia'

const defaultStoreState = (): User => ({
  id: -1,
  email: '',
  name: '',
  isAdmin: false,
  siteId: -1
});

export const useUserStore = defineStore('user', {
  state: defaultStoreState,
  actions: {
    setUser(user: User | null) {
      if (user === null) {
        Object.assign(this, defaultStoreState());
      } else {
        Object.assign(this, user);
      }
    }
  }
})
