import { User } from '~/lib/domain/user'
import { defineStore } from 'pinia'

const defaultStoreState = (): User => ({
  email: '',
  name: ''
});

export const useUserStore = defineStore('user', {
  state: defaultStoreState,
  actions: {
    setUser(user: User) {
      Object.assign(this, user);
    }
  },
  getters: {
    tryGetInitials: ({ name }) => {
      const nameSplit = name.split(" ");
      if (nameSplit.length !== 2) {
        return null;
      }
      return nameSplit.map((n)=>n[0].toUpperCase()).join("");
    }
  },
})
